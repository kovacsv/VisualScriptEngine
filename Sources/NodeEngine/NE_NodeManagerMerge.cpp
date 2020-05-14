#include "NE_NodeManagerMerge.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

NodeFilter::NodeFilter ()
{

}

NodeFilter::~NodeFilter ()
{

}

AllNodesFilter::AllNodesFilter ()
{

}

AllNodesFilter::~AllNodesFilter ()
{

}

bool AllNodesFilter::NeedToProcessNode (const NodeId&) const
{
	return true;
}

AppendEventHandler::AppendEventHandler ()
{

}

AppendEventHandler::~AppendEventHandler ()
{

}

EmptyAppendEventHandler::EmptyAppendEventHandler ()
{

}

EmptyAppendEventHandler::~EmptyAppendEventHandler ()
{

}

void EmptyAppendEventHandler::NodeAdded (const NE::NodeId&)
{

}

UpdateEventHandler::UpdateEventHandler ()
{
	
}

UpdateEventHandler::~UpdateEventHandler ()
{

}

EmptyUpdateEventHandler::EmptyUpdateEventHandler ()
{

}

EmptyUpdateEventHandler::~EmptyUpdateEventHandler ()
{

}

void EmptyUpdateEventHandler::BeforeNodeDelete (const NE::NodeId&)
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

static void EnumerateInputConnectionsOrdered (const NodeManager& nodeManager, const std::vector<NodeConstPtr>& nodes, const std::function<void (const ConnectionInfo&)>& processor)
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
	std::sort (nodes.begin (), nodes.end (), [&] (const NodeId& a, const NodeId& b) {
		return a < b;
	});
}

static void Sort (std::vector<NodeConstPtr>& nodes)
{
	std::sort (nodes.begin (), nodes.end (), [&] (const NodeConstPtr& a, const NodeConstPtr& b) {
		return a->GetId () < b->GetId ();
	});
}

bool NodeManagerMerge::AppendNodeManager (const NodeManager& source, NodeManager& target, const NE::NodeCollection& nodeCollection, AppendEventHandler& eventHandler)
{
	class NodeCollectionFilter : public NE::NodeFilter
	{
	public:
		NodeCollectionFilter (const NE::NodeCollection& nodeCollection) :
			nodeCollection (nodeCollection)
		{

		}

		virtual bool NeedToProcessNode (const NE::NodeId& nodeId) const override
		{
			return nodeCollection.Contains (nodeId);
		}

	private:
		const NE::NodeCollection& nodeCollection;
	};

	NodeCollectionFilter filter (nodeCollection);
	return NE::NodeManagerMerge::AppendNodeManager (source, target, filter, eventHandler);
}

bool NodeManagerMerge::AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter, AppendEventHandler& eventHandler)
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
		eventHandler.NodeAdded (cloned->GetId ());
	}

	// maintain connections between added nodes
	bool success = true;
	EnumerateInputConnectionsOrdered (source, nodesToClone, [&] (const ConnectionInfo& connection) {
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

bool NodeManagerMerge::UpdateNodeManager (const NodeManager& source, NodeManager& target, UpdateEventHandler& eventHandler)
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
		for (const SlotInfo& slotInfo : outputSlots) {
			NodeConstPtr outputNode = target.GetNode (slotInfo.GetNodeId ());
			OutputSlotConstPtr outputSlot = outputNode->GetOutputSlot (slotInfo.GetSlotId ());
			target.ConnectOutputSlotToInputSlot (outputSlot, inputSlot);
		}
	}

	// recreate groups in target
	target.DeleteAllNodeGroups ();
	source.EnumerateNodeGroups ([&] (const NodeGroupConstPtr& sourceGroup) {
		NodeGroupPtr targetGroup (NodeGroup::Clone (sourceGroup));
		target.AddNodeGroup (targetGroup);
		const NodeCollection& sourceGroupNodes = source.GetGroupNodes (sourceGroup);
		sourceGroupNodes.Enumerate ([&] (const NodeId& sourceNodeId) {
			if (target.ContainsNode (sourceNodeId)) {
				target.AddNodeToGroup (targetGroup, sourceNodeId);
			}
			return true;
		});
		return true;
	});

	return true;
}

}
