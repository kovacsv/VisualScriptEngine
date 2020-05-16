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

void EmptyAppendEventHandler::NodeAdded (const NodeId&)
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

void EmptyUpdateEventHandler::BeforeNodeDelete (const NodeId&)
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

static void EnumerateInternalConnections (const NodeManager& nodeManager, const NodeCollection& nodes, const std::function<void (const ConnectionInfo&)>& processor)
{
	nodes.Enumerate ([&] (const NodeId& nodeId) {
		NodeConstPtr node = nodeManager.GetNode (nodeId);
		node->EnumerateInputSlots ([&] (const InputSlotConstPtr& inputSlot) {
			SlotInfo inputSlotInfo (inputSlot->GetOwnerNodeId (), inputSlot->GetId ());
			nodeManager.EnumerateConnectedOutputSlots (inputSlot, [&] (const OutputSlotConstPtr& outputSlot) {
				if (nodes.Contains (outputSlot->GetOwnerNodeId ())) {
					SlotInfo outputSlotInfo (outputSlot->GetOwnerNodeId (), outputSlot->GetId ());
					ConnectionInfo connection (outputSlotInfo, inputSlotInfo);
					processor (connection);
				}
			});
			return true;
		});
		return true;
	});
}

bool NodeManagerMerge::AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeCollection& nodeCollection, AppendEventHandler& eventHandler)
{
	class NodeCollectionFilter : public NodeFilter
	{
	public:
		NodeCollectionFilter (const NodeCollection& nodeCollection) :
			nodeCollection (nodeCollection)
		{

		}

		virtual bool NeedToProcessNode (const NodeId& nodeId) const override
		{
			return nodeCollection.Contains (nodeId);
		}

	private:
		const NodeCollection& nodeCollection;
	};

	NodeCollectionFilter filter (nodeCollection);
	return NodeManagerMerge::AppendNodeManager (source, target, filter, eventHandler);
}

bool NodeManagerMerge::AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter, AppendEventHandler& eventHandler)
{
	// collect nodes to create
	NodeCollection nodesToClone;
	source.EnumerateNodes ([&] (const NodeConstPtr& node) {
		NodeId nodeId = node->GetId ();
		if (!nodeFilter.NeedToProcessNode (nodeId)) {
			return true;
		}
		nodesToClone.Insert (nodeId);
		return true;
	});
	nodesToClone.MakeSorted ();

	// add nodes
	std::unordered_map<NodeId, NodeId> oldToNewNodeIdTable;
	nodesToClone.Enumerate ([&] (const NodeId& nodeId) {
		NodeConstPtr node = source.GetNode (nodeId);
		NodePtr cloned = Node::Clone (node);
		target.AddInitializedNode (cloned, NodeManager::IdHandlingPolicy::GenerateNewId);
		oldToNewNodeIdTable.insert ({ node->GetId (), cloned->GetId () });
		eventHandler.NodeAdded (cloned->GetId ());
		return true;
	});

	// maintain connections between added nodes
	bool success = true;
	EnumerateInternalConnections (source, nodesToClone, [&] (const ConnectionInfo& connection) {
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
	std::sort (nodesToCreate.begin (), nodesToCreate.end ());
	std::sort (nodesToDelete.begin (), nodesToDelete.end ());

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
