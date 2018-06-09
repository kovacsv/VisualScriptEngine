#include "NE_NodeManagerMerge.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

static void EnumerateConnectionsOrdered (const NodeManager& nodeManager, const std::vector<NodeConstPtr>& nodes, const std::function<void (const ConnectionInfo&)>& processor)
{
	for (const NodeConstPtr& node : nodes) {
		node->EnumerateInputSlots ([&] (const InputSlotConstPtr& inputSlot) {
			nodeManager.EnumerateConnectedOutputSlots (inputSlot, [&] (const OutputSlotConstPtr& outputSlot) {
				ConnectionInfo connection (
					SlotInfo (outputSlot->GetOwnerNodeId (), outputSlot->GetId ()),
					SlotInfo (inputSlot->GetOwnerNodeId (), inputSlot->GetId ())
				);
				processor (connection);
			});
			return true;
		});
	}
}

bool NodeManagerMerge::AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter)
{
	std::vector<NodeConstPtr> nodesToClone;
	source.EnumerateNodes ([&] (const NodeConstPtr& node) {
		if (!nodeFilter.NeedToProcessNode (node->GetId ())) {
			return true;
		}
		nodesToClone.push_back (node);
		return true;
	});

	std::sort (nodesToClone.begin (), nodesToClone.end (), [&] (const NodeConstPtr& a, const NodeConstPtr& b) -> bool {
		return a->GetId () < b->GetId ();
	});

	std::unordered_map<NodeId, NodeId> oldToNewNodeIdTable;
	for (const NodeConstPtr& node : nodesToClone) {
		NodePtr cloned = Node::Clone (node);
		target.AddInitializedNode (cloned, NodeManager::IdHandlingPolicy::GenerateNewId);
		oldToNewNodeIdTable.insert ({ node->GetId (), cloned->GetId () });
	}

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

bool NodeManagerMerge::MergeNodeManager (const NodeManager& /*source*/, NodeManager& /*target*/)
{
	// TODO
	return false;
}

}
