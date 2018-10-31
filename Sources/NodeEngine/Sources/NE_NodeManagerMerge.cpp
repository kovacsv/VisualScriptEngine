#include "NE_NodeManagerMerge.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

MergeEventHandler::MergeEventHandler ()
{
	
}

MergeEventHandler::~MergeEventHandler ()
{

}

static std::vector<SlotInfo> GetConnectedOutputSlots (const NodeManager& nodeManager, const InputSlotConstPtr& inputSlot)
{
	std::vector<SlotInfo> result;
	nodeManager.EnumerateConnectedOutputSlots (inputSlot, [&] (const OutputSlotConstPtr& outputSlot) {
		result.push_back (SlotInfo (outputSlot->GetOwnerNodeId (), outputSlot->GetId ()));
	});
	return result;
}

static void EnumerateConnectionsOrdered (const NodeManager& nodeManager, const std::vector<NodeConstPtr>& nodes, const std::function<void (const ConnectionInfo&)>& processor)
{
	for (const NodeConstPtr& node : nodes) {
		node->EnumerateInputSlots ([&] (const InputSlotConstPtr& inputSlot) {
			SlotInfo inputSlotInfo (inputSlot->GetOwnerNodeId (), inputSlot->GetId ());
			std::vector<SlotInfo> outputSlots = GetConnectedOutputSlots (nodeManager, inputSlot);
			for (const SlotInfo& outputSlotInfo : outputSlots) {
				ConnectionInfo connection (outputSlotInfo, inputSlotInfo);
				processor (connection);
			}
			return true;
		});
	}
}

static void Sort (std::vector<NodeId>& nodes)
{
	std::sort (nodes.begin (), nodes.end (), [&] (const NodeId& a, const NodeId& b) -> bool {
		return a < b;
	});
}

static void Sort (std::vector<NodeConstPtr>& nodes)
{
	std::sort (nodes.begin (), nodes.end (), [&] (const NodeConstPtr& a, const NodeConstPtr& b) -> bool {
		return a->GetId () < b->GetId ();
	});
}

NodeFilter::NodeFilter ()
{

}

NodeFilter::~NodeFilter ()
{

}

bool NodeManagerMerge::AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter)
{
	// collect nodes to create
	std::vector<NodeConstPtr> nodesToClone;
	source.EnumerateNodes ([&] (const NodeConstPtr& node) {
		if (!nodeFilter.NeedToProcessNode (node->GetId ())) {
			return true;
		}
		nodesToClone.push_back (node);
		return true;
	});
	Sort (nodesToClone);

	// add nodes
	std::unordered_map<NodeId, NodeId> oldToNewNodeIdTable;
	for (const NodeConstPtr& node : nodesToClone) {
		NodePtr cloned = Node::Clone (node);
		target.AddInitializedNode (cloned, NodeManager::IdHandlingPolicy::GenerateNewId);
		oldToNewNodeIdTable.insert ({ node->GetId (), cloned->GetId () });
	}

	// maintain connections between added nodes
	bool success = true;
	EnumerateConnectionsOrdered (source, nodesToClone, [&] (const ConnectionInfo& connection) {
		if (!nodeFilter.NeedToProcessNode (connection.GetOutputNodeId ())) {
			return;
		}
		NodeConstPtr outputNode = target.GetNode (oldToNewNodeIdTable[connection.GetOutputNodeId ()]);
		NodeConstPtr inputNode = target.GetNode (oldToNewNodeIdTable[connection.GetInputNodeId ()]);
		if (DBGERROR (outputNode == nullptr || inputNode == nullptr)) {
			success = false;
			return;
		}
		OutputSlotConstPtr outputSlot = outputNode->GetOutputSlot (connection.GetOutputSlotId ());
		InputSlotConstPtr inputSlot = inputNode->GetInputSlot (connection.GetInputSlotId ());
		if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
			success = false;
			return;
		}
		if (DBGERROR (!target.ConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
			success = false;
			return;
		}
	});

	return success;
}

bool NodeManagerMerge::UpdateNodeManager (const NodeManager& source, NodeManager& target, MergeEventHandler& eventHandler)
{
	// collect nodes to create or delete
	std::vector<NodeId> nodesToCreate;
	std::vector<NodeId> nodesToDelete;
	source.EnumerateNodes ([&] (const NodeConstPtr& sourceNode) {
		if (target.ContainsNode (sourceNode->GetId ())) {
			NodeConstPtr targetNode = target.GetNode (sourceNode->GetId ());
			if (!Node::IsEqual (sourceNode, targetNode)) {
				nodesToDelete.push_back (targetNode->GetId ());
				nodesToCreate.push_back (sourceNode->GetId ());
			}
		} else {
			nodesToCreate.push_back (sourceNode->GetId ());
		}
		return true;
	});
	target.EnumerateNodes ([&] (const NodeConstPtr& targetNode) {
		if (!source.ContainsNode (targetNode->GetId ())) {
			nodesToDelete.push_back (targetNode->GetId ());
		}
		return true;
	});
	Sort (nodesToCreate);
	Sort (nodesToDelete);

	// delete and create nodes
	for (const NodeId& nodeId : nodesToDelete) {
		eventHandler.BeforeNodeDelete (nodeId);
		target.DeleteNode (nodeId);
	}
	for (const NodeId& nodeId : nodesToCreate) {
		NodePtr cloned = Node::Clone (source.GetNode (nodeId));
		target.AddInitializedNode (cloned, NodeManager::IdHandlingPolicy::KeepOriginalId);
	}

	// collect input slots with changed connections
	std::unordered_map<InputSlotConstPtr, std::vector<SlotInfo>> inputSlotsToReconnect;
	target.EnumerateNodes ([&] (const NodeConstPtr& targetNode) {
		if (!source.ContainsNode (targetNode->GetId ())) {
			return true;
		}
		NodeConstPtr sourceNode = source.GetNode (targetNode->GetId ());
		targetNode->EnumerateInputSlots ([&] (const InputSlotConstPtr& targetInputSlot) {
			if (!sourceNode->HasInputSlot (targetInputSlot->GetId ())) {
				return true;
			}
			InputSlotConstPtr sourceInputSlot = sourceNode->GetInputSlot (targetInputSlot->GetId ());
			std::vector<SlotInfo> sourceOutputSlots = GetConnectedOutputSlots (source, sourceInputSlot);
			std::vector<SlotInfo> targetOutputSlots = GetConnectedOutputSlots (target, targetInputSlot);
			if (sourceOutputSlots != targetOutputSlots) {
				inputSlotsToReconnect.insert ({ targetInputSlot, sourceOutputSlots });
			}
			return true;
		});
		return true;
	});

	// reconnect changed input slots
	for (const auto& inputSlotData : inputSlotsToReconnect) {
		const InputSlotConstPtr& inputSlot = inputSlotData.first;
		const std::vector<SlotInfo>& outputSlots = inputSlotData.second;
		target.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
		if (outputSlots.empty ()) {
			target.InvalidateNodeValue (inputSlot->GetOwnerNodeId ());
		} else {
			for (const SlotInfo& slotInfo : outputSlots) {
				NodeConstPtr outputNode = target.GetNode (slotInfo.GetNodeId ());
				OutputSlotConstPtr outputSlot = outputNode->GetOutputSlot (slotInfo.GetSlotId ());
				target.ConnectOutputSlotToInputSlot (outputSlot, inputSlot);
			}
		}
	}

	// recreate groups if there are different groups
	std::unordered_set<NodeGroupConstPtr> sourceGroups;
	std::unordered_set<NodeGroupConstPtr> targetGroups;
	source.EnumerateNodeGroups ([&] (const NodeGroupConstPtr& sourceGroup) {
		sourceGroups.insert (sourceGroup);
		return true;
	});
	target.EnumerateNodeGroups ([&] (const NodeGroupConstPtr& targetGroup) {
		targetGroups.insert (targetGroup);
		return true;
	});

	bool recreateGroups = false;
	if (sourceGroups.size () != targetGroups.size ()) {
		recreateGroups = true;
	} else {
		for (const NodeGroupConstPtr& sourceGroup : sourceGroups) {
			bool hasSameGroup = false;
			for (const NodeGroupConstPtr& targetGroup : targetGroups) {
				if (NodeGroup::IsEqual (sourceGroup, targetGroup)) {
					hasSameGroup = true;
					targetGroups.erase (targetGroup);
					break;
				}
			}
			if (!hasSameGroup) {
				recreateGroups = true;
				break;
			}
		}
	}
	
	if (recreateGroups) {
		target.DeleteAllNodeGroups ();
		for (const NodeGroupConstPtr& sourceGroup : sourceGroups) {
			target.AddNodeGroup (NodeGroup::Clone (sourceGroup));
		}
	}

	return true;
}

}
