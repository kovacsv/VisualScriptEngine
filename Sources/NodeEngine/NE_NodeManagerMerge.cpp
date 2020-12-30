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

bool AllNodesFilter::NeedToProcessSourceNode (const NodeId&) const
{
	return true;
}

NodeCollectionFilter::NodeCollectionFilter (const NodeCollection& nodeCollection) :
	nodeCollection (nodeCollection)
{

}

NodeCollectionFilter::~NodeCollectionFilter ()
{

}

bool NodeCollectionFilter::NeedToProcessSourceNode (const NodeId& nodeId) const
{
	return nodeCollection.Contains (nodeId);
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

void EmptyAppendEventHandler::TargetNodeAdded (const NodeId&)
{

}

NodeCollectorAppendEventHandler::NodeCollectorAppendEventHandler () :
	targetNodes ()
{

}

NodeCollectorAppendEventHandler::~NodeCollectorAppendEventHandler ()
{

}

void NodeCollectorAppendEventHandler::TargetNodeAdded (const NodeId& nodeId)
{
	targetNodes.Insert (nodeId);
}

const NodeCollection& NodeCollectorAppendEventHandler::GetAddedTargetNodes () const
{
	return targetNodes;
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

bool NodeManagerMerge::AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter, AppendEventHandler& eventHandler)
{
	// collect nodes to create
	NodeCollection nodesToClone;
	source.EnumerateNodes ([&] (NodeConstPtr node) {
		NodeId nodeId = node->GetId ();
		if (!nodeFilter.NeedToProcessSourceNode (nodeId)) {
			return true;
		}
		nodesToClone.Insert (nodeId);
		return true;
	});

	// add nodes
	std::unordered_map<NodeId, NodeId> oldToNewNodeIdTable;
	nodesToClone.Enumerate ([&] (const NodeId& nodeId) {
		NodeConstPtr sourceNode = source.GetNode (nodeId);
		NodePtr targetNode = Node::Clone (sourceNode);
		target.AddNode (targetNode, NodeManager::IdPolicy::GenerateNew, NodeManager::InitPolicy::DoNotInitialize);
		oldToNewNodeIdTable.insert ({ sourceNode->GetId (), targetNode->GetId () });
		eventHandler.TargetNodeAdded (targetNode->GetId ());
		return true;
	});

	// maintain connections between added nodes
	bool success = true;
	source.EnumerateConnections (nodesToClone, [&] (const OutputSlotConstPtr& oldOutputSlot, const InputSlotConstPtr& oldInputSlot) {
		NodeConstPtr outputNode = target.GetNode (oldToNewNodeIdTable[oldOutputSlot->GetOwnerNodeId ()]);
		NodeConstPtr inputNode = target.GetNode (oldToNewNodeIdTable[oldInputSlot->GetOwnerNodeId ()]);
		if (DBGERROR (outputNode == nullptr || inputNode == nullptr)) {
			success = false;
			return;
		}
		OutputSlotConstPtr outputSlot = outputNode->GetOutputSlot (oldOutputSlot->GetId ());
		InputSlotConstPtr inputSlot = inputNode->GetInputSlot (oldInputSlot->GetId ());
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
	source.EnumerateNodes ([&] (NodeConstPtr sourceNode) {
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
	target.EnumerateNodes ([&] (NodeConstPtr targetNode) {
		if (!source.ContainsNode (targetNode->GetId ())) {
			nodesToDelete.push_back (targetNode->GetId ());
		}
		return true;
	});

	// delete and create nodes
	for (const NodeId& nodeId : nodesToDelete) {
		eventHandler.BeforeNodeDelete (nodeId);
		target.DeleteNode (nodeId);
	}
	for (const NodeId& nodeId : nodesToCreate) {
		NodePtr cloned = Node::Clone (source.GetNode (nodeId));
		target.AddNode (cloned, NodeManager::IdPolicy::KeepOriginal, NodeManager::InitPolicy::DoNotInitialize);
	}

	// collect input slots with changed connections
	std::unordered_map<InputSlotConstPtr, std::vector<SlotInfo>> inputSlotsToReconnect;
	target.EnumerateNodes ([&] (NodeConstPtr targetNode) {
		if (!source.ContainsNode (targetNode->GetId ())) {
			return true;
		}
		NodeConstPtr sourceNode = source.GetNode (targetNode->GetId ());
		targetNode->EnumerateInputSlots ([&] (InputSlotConstPtr targetInputSlot) {
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
	source.EnumerateNodeGroups ([&] (NodeGroupConstPtr sourceGroup) {
		NodeGroupPtr targetGroup (NodeGroup::Clone (sourceGroup));
		target.AddNodeGroup (targetGroup, NodeManager::IdPolicy::KeepOriginal);
		const NodeCollection& sourceGroupNodes = source.GetGroupNodes (sourceGroup->GetId ());
		sourceGroupNodes.Enumerate ([&] (const NodeId& sourceNodeId) {
			if (target.ContainsNode (sourceNodeId)) {
				target.AddNodeToGroup (targetGroup->GetId (), sourceNodeId);
			}
			return true;
		});
		return true;
	});

	target.MakeNodesAndGroupsSorted ();
	return true;
}

}
