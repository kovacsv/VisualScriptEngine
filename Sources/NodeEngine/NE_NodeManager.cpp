#include "NE_NodeManager.hpp"
#include "NE_Utils.hpp"
#include "NE_Node.hpp"
#include "NE_Debug.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_MemoryStream.hpp"
#include "NE_NodeManagerSerialization.hpp"

namespace NE
{

SERIALIZATION_INFO (NodeManager, 4);

template <typename SlotListType, typename SlotType>
static bool HasDuplicates (const SlotListType& slots)
{
	bool hasDuplicates = false;
	std::unordered_set<SlotType> slotSet;
	slots.Enumerate ([&] (const SlotType& slot) {
		if (slotSet.find (slot) != slotSet.end ()) {
			hasDuplicates = true;
		}
		slotSet.insert (slot);
		return !hasDuplicates;
	});
	return hasDuplicates;
}

class NodeManagerNodeEvaluator : public NodeEvaluator
{
public:
	NodeManagerNodeEvaluator (const NodeManager& nodeManager, NodeValueCache& nodeValueCache) :
		nodeManager (nodeManager),
		nodeValueCache (nodeValueCache)
	{
	
	}

	virtual void InvalidateNodeValue (const NodeId& nodeId) const override
	{
		nodeManager.InvalidateNodeValue (nodeId);
	}

	virtual bool HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const override
	{
		return nodeManager.HasConnectedOutputSlots (inputSlot);
	}

	virtual void EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const override
	{
		return nodeManager.EnumerateConnectedOutputSlots (inputSlot, processor);
	}

	virtual bool IsCalculationEnabled () const override
	{
		return nodeManager.IsCalculationEnabled ();
	}

	virtual bool HasCalculatedNodeValue (const NodeId& nodeId) const override
	{
		return nodeValueCache.Contains (nodeId);
	}

	virtual ValueConstPtr GetCalculatedNodeValue (const NodeId& nodeId) const override
	{
		return nodeValueCache.Get (nodeId);
	}

	virtual void SetCalculatedNodeValue (const NodeId& nodeId, const ValueConstPtr& valuePtr) const override
	{
		nodeValueCache.Add (nodeId, valuePtr);
	}

private:
	const NodeManager&	nodeManager;
	NodeValueCache&		nodeValueCache;
};

OutputSlotList::OutputSlotList ()
{

}

OutputSlotList::~OutputSlotList ()
{

}

bool OutputSlotList::IsEmpty () const
{
	return GetSize () == 0;
}

InputSlotList::InputSlotList ()
{

}

InputSlotList::~InputSlotList ()
{

}

bool InputSlotList::IsEmpty () const
{
	return GetSize () == 0;
}

NodeManager::NodeManager () :
	idGenerator (),
	nodeList (),
	connectionManager (),
	nodeGroupList (),
	updateMode (UpdateMode::Automatic),
	nodeValueCache (),
	nodeEvaluator (nullptr),
	isForceCalculate (false)
{
	nodeEvaluator.reset (new NodeManagerNodeEvaluator (*this, nodeValueCache));
}

NodeManager::~NodeManager ()
{

}

void NodeManager::Clear ()
{
	idGenerator.Clear ();
	nodeList.Clear ();
	connectionManager.Clear ();
	nodeGroupList.Clear ();
	updateMode = UpdateMode::Automatic;

	nodeValueCache.Clear ();
	nodeEvaluator.reset (new NodeManagerNodeEvaluator (*this, nodeValueCache));
	isForceCalculate = false;
}

bool NodeManager::IsEmpty () const
{
	return nodeList.IsEmpty () && nodeGroupList.IsEmpty () && connectionManager.IsEmpty ();
}

size_t NodeManager::GetNodeCount () const
{
	return nodeList.Count ();
}

size_t NodeManager::GetNodeGroupCount () const
{
	return nodeGroupList.Count ();
}

size_t NodeManager::GetConnectionCount () const
{
	return connectionManager.GetConnectionCount ();
}

void NodeManager::EnumerateNodes (const std::function<bool (NodePtr)>& processor)
{
	nodeList.Enumerate (processor);
}

void NodeManager::EnumerateNodes (const std::function<bool (NodeConstPtr)>& processor) const
{
	nodeList.Enumerate (processor);
}

bool NodeManager::ContainsNode (const NodeId& id) const
{
	return nodeList.ContainsNode (id);
}

NodeConstPtr NodeManager::GetNode (const NodeId& id) const
{
	return nodeList.GetNode (id);
}

NodePtr NodeManager::GetNode (const NodeId& id)
{
	return nodeList.GetNode (id);
}

NodePtr NodeManager::AddNode (const NodePtr& node)
{
	return AddNode (node, IdPolicy::GenerateNew, InitPolicy::Initialize);
}

bool NodeManager::DeleteNode (const NodeId& id)
{
	if (DBGERROR (!ContainsNode (id))) {
		return false;
	}
	return DeleteNode (GetNode (id));
}

bool NodeManager::DeleteNode (const NodePtr& node)
{
	if (DBGERROR (node == nullptr || !node->IsEvaluatorSet ())) {
		return false;
	}

	if (DBGERROR (!ContainsNode (node->GetId ()))) {
		return false;
	}

	nodeGroupList.RemoveNodeFromGroup (node->GetId ());
	node->InvalidateValue ();

	node->EnumerateInputSlots ([&] (InputSlotConstPtr inputSlot) {
		connectionManager.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
		return true;
	});

	node->EnumerateOutputSlots ([&] (OutputSlotConstPtr outputSlot) {
		connectionManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
		return true;
	});

	nodeList.DeleteNode (node->GetId ());
	node->ClearEvaluator ();

	return true;
}

bool NodeManager::IsOutputSlotConnectedToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const
{
	if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
		return false;
	}
	return connectionManager.IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot);
}

bool NodeManager::CanConnectOutputSlotToInputSlot (const InputSlotConstPtr& inputSlot) const
{
	return connectionManager.CanConnectOutputSlotToInputSlot (inputSlot);
}

bool NodeManager::CanConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const
{
	if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
		return false;
	}

	if (!connectionManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot)) {
		return false;
	}

	NodeConstPtr outputNode = GetNode (outputSlot->GetOwnerNodeId ());
	NodeConstPtr inputNode = GetNode (inputSlot->GetOwnerNodeId ());
	if (DBGERROR (outputNode == nullptr || inputNode == nullptr)) {
		return false;
	}

	bool willCreateCycle = false;
	if (outputNode == inputNode) {
		willCreateCycle = true;
	} else {
		EnumerateDependentNodesRecursive (inputNode, [&] (const NodeConstPtr& dependentNode) {
			if (outputNode == dependentNode) {
				willCreateCycle = true;
			}
		});
	}

	if (willCreateCycle) {
		return false;
	}

	return true;
}

bool NodeManager::CanConnectOutputSlotsToInputSlot (const OutputSlotList& outputSlots, const InputSlotConstPtr& inputSlot) const
{
	if (DBGERROR (outputSlots.IsEmpty () || inputSlot == nullptr)) {
		return false;
	}

	if (HasDuplicates<OutputSlotList, OutputSlotConstPtr> (outputSlots)) {
		return false;
	}

	if (outputSlots.GetSize () > 1 && inputSlot->GetOutputSlotConnectionMode () != OutputSlotConnectionMode::Multiple) {
		return false;
	}

	bool canConnect = true;
	outputSlots.Enumerate ([&] (const OutputSlotConstPtr& outputSlot) {
		if (!CanConnectOutputSlotToInputSlot (outputSlot, inputSlot)) {
			canConnect = false;
		}
		return canConnect;
	});

	return canConnect;
}

bool NodeManager::CanConnectOutputSlotToInputSlots (const OutputSlotConstPtr& outputSlot, const InputSlotList& inputSlots) const
{
	if (DBGERROR (outputSlot == nullptr || inputSlots.IsEmpty ())) {
		return false;
	}

	if (HasDuplicates<InputSlotList, InputSlotConstPtr> (inputSlots)) {
		return false;
	}

	bool canConnect = true;
	inputSlots.Enumerate ([&] (const InputSlotConstPtr& inputSlot) {
		if (!CanConnectOutputSlotToInputSlot (outputSlot, inputSlot)) {
			canConnect = false;
		}
		return canConnect;
	});
	
	return canConnect;
}

bool NodeManager::ConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot)
{
	if (DBGERROR (!CanConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
		return false;
	}

	InvalidateNodeValue (GetNode (inputSlot->GetOwnerNodeId ()));
	return connectionManager.ConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}

bool NodeManager::ConnectOutputSlotsToInputSlot (const OutputSlotList& outputSlots, const InputSlotConstPtr& inputSlot)
{
	if (DBGERROR (!CanConnectOutputSlotsToInputSlot (outputSlots, inputSlot))) {
		return false;
	}

	bool success = true;
	outputSlots.Enumerate ([&] (OutputSlotConstPtr outputSlot) {
		if (DBGERROR (!ConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
			success = false;
		}
		return success;
	});

	return success;
}

bool NodeManager::ConnectOutputSlotToInputSlots (const OutputSlotConstPtr& outputSlot, const InputSlotList& inputSlots)
{
	if (DBGERROR (!CanConnectOutputSlotToInputSlots (outputSlot, inputSlots))) {
		return false;
	}

	bool success = true;
	inputSlots.Enumerate ([&] (InputSlotConstPtr inputSlot) {
		if (DBGERROR (!ConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
			success = false;
		}
		return success;
	});

	return success;
}

bool NodeManager::DisconnectOutputSlotFromInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot)
{
	if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
		return false;
	}
	
	if (DBGERROR (!IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot))) {
		return false;
	}

	InvalidateNodeValue (GetNode (inputSlot->GetOwnerNodeId ()));
	return connectionManager.DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot);
}

bool NodeManager::DisconnectOutputSlotsFromInputSlot (const OutputSlotList& outputSlots, const InputSlotConstPtr& inputSlot)
{
	if (DBGERROR (outputSlots.IsEmpty () || inputSlot == nullptr)) {
		return false;
	}

	bool hasDuplicates = HasDuplicates<OutputSlotList, OutputSlotConstPtr> (outputSlots);
	if (DBGERROR (hasDuplicates)) {
		return false;
	}

	bool canDisconnect = true;
	outputSlots.Enumerate ([&] (OutputSlotConstPtr outputSlot) {
		if (DBGERROR (!IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot))) {
			canDisconnect = false;
		}
		return canDisconnect;
	});
	if (DBGERROR (!canDisconnect)) {
		return false;
	}

	bool success = true;
	outputSlots.Enumerate ([&] (OutputSlotConstPtr outputSlot) {
		if (DBGERROR (!DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot))) {
			success = false;
		}
		return success;
	});

	return success;
}

bool NodeManager::DisconnectOutputSlotFromInputSlots (const OutputSlotConstPtr& outputSlot, const InputSlotList& inputSlots)
{
	if (DBGERROR (outputSlot == nullptr || inputSlots.IsEmpty ())) {
		return false;
	}

	bool hasDuplicates = HasDuplicates<InputSlotList, InputSlotConstPtr> (inputSlots);
	if (DBGERROR (hasDuplicates)) {
		return false;
	}

	bool canDisconnect = true;
	inputSlots.Enumerate ([&] (InputSlotConstPtr inputSlot) {
		if (DBGERROR (!IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot))) {
			canDisconnect = false;
		}
		return canDisconnect;
	});
	if (DBGERROR (!canDisconnect)) {
		return false;
	}

	bool success = true;
	inputSlots.Enumerate ([&] (InputSlotConstPtr inputSlot) {
		if (DBGERROR (!DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot))) {
			success = false;
		}
		return success;
	});

	return success;
}

bool NodeManager::DisconnectAllInputSlotsFromOutputSlot (const OutputSlotConstPtr& outputSlot)
{
	InvalidateNodeValue (GetNode (outputSlot->GetOwnerNodeId ()));
	return connectionManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
}

bool NodeManager::DisconnectAllOutputSlotsFromInputSlot (const InputSlotConstPtr& inputSlot)
{
	InvalidateNodeValue (GetNode (inputSlot->GetOwnerNodeId ()));
	return connectionManager.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
}

bool NodeManager::HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const
{
	return connectionManager.HasConnectedOutputSlots (inputSlot);
}

bool NodeManager::HasConnectedInputSlots (const OutputSlotConstPtr& outputSlot) const
{
	return connectionManager.HasConnectedInputSlots (outputSlot);
}

size_t NodeManager::GetConnectedOutputSlotCount (const InputSlotConstPtr& inputSlot) const
{
	return connectionManager.GetConnectedOutputSlotCount (inputSlot);
}

size_t NodeManager::GetConnectedInputSlotCount (const OutputSlotConstPtr& outputSlot) const
{
	return connectionManager.GetConnectedInputSlotCount (outputSlot);
}

void NodeManager::EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const
{
	connectionManager.EnumerateConnectedOutputSlots (inputSlot, processor);
}

void NodeManager::EnumerateConnectedInputSlots (const OutputSlotConstPtr& outputSlot, const std::function<void (const InputSlotConstPtr&)>& processor) const
{
	connectionManager.EnumerateConnectedInputSlots (outputSlot, processor);
}

void NodeManager::EnumerateConnections (const std::function<void (const OutputSlotConstPtr&, const InputSlotConstPtr&)>& processor) const
{
	nodeList.Enumerate ([&] (const NodeConstPtr& node) {
		node->EnumerateOutputSlots ([&] (OutputSlotConstPtr outputSlot) {
			connectionManager.EnumerateConnectedInputSlots (outputSlot, [&] (const InputSlotConstPtr& inputSlot) {
				processor (outputSlot, inputSlot);
			});
			return true;
		});
		return true;
	});
}

void NodeManager::EnumerateConnections (const NodeCollection& nodes, const std::function<void (const OutputSlotConstPtr&, const InputSlotConstPtr&)>& processor) const
{
	nodes.Enumerate ([&] (const NodeId& nodeId) {
		NodeConstPtr node = GetNode (nodeId);
		node->EnumerateOutputSlots ([&] (OutputSlotConstPtr outputSlot) {
			connectionManager.EnumerateConnectedInputSlots (outputSlot, [&] (const InputSlotConstPtr& inputSlot) {
				if (nodes.Contains (inputSlot->GetOwnerNodeId ())) {
					processor (outputSlot, inputSlot);
				}
			});
			return true;
		});
		return true;
	});
}

void NodeManager::EvaluateAllNodes (EvaluationEnv& env) const
{
	EnumerateNodes ([&] (NodeConstPtr node) {
		node->Evaluate (env);
		return true;
	});
}

void NodeManager::ForceEvaluateAllNodes (EvaluationEnv& env) const
{
	ValueGuard<bool> isForceCalculateGuard (isForceCalculate, true);
	std::vector<NodeConstPtr> nodesToRecalculate;
	EnumerateNodes ([&] (NodeConstPtr node) {
		Node::CalculationStatus calcStatus = node->GetCalculationStatus ();
		DBGASSERT (calcStatus != Node::CalculationStatus::NeedToCalculateButDisabled);
		if (calcStatus == Node::CalculationStatus::NeedToCalculate) {
			nodesToRecalculate.push_back (node);
		}
		return true;
	});
	for (const NodeConstPtr& node : nodesToRecalculate) {
		InvalidateNodeValue (node);
	}
	EvaluateAllNodes (env);
}

void NodeManager::InvalidateNodeValue (const NodeId& nodeId) const
{
	NodeConstPtr node = GetNode (nodeId);
	InvalidateNodeValue (node);
}

void NodeManager::InvalidateNodeValue (const NodeConstPtr& node) const
{
	const NodeId& nodeId = node->GetId ();
	if (nodeValueCache.Contains (nodeId)) {
		nodeValueCache.Remove (nodeId);
	}
	EnumerateDependentNodes (node, [&] (const NodeConstPtr& dependentNode) {
		InvalidateNodeValue (dependentNode);
	});
}

void NodeManager::EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const
{
	node->EnumerateOutputSlots ([&] (OutputSlotConstPtr outputSlot) {
		if (connectionManager.HasConnectedInputSlots (outputSlot)) {
			connectionManager.EnumerateConnectedInputSlots (outputSlot, [&] (const InputSlotConstPtr& inputSlot) {
				processor (inputSlot->GetOwnerNodeId ());
			});
		}
		return true;
	});
}

void NodeManager::EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const
{
	EnumerateDependentNodes (node, [&] (const NodeId& dependentNodeId) {
		NodeConstPtr dependentNode = GetNode (dependentNodeId);
		processor (dependentNodeId);
		EnumerateDependentNodesRecursive (dependentNode, processor);
	});
}

void NodeManager::EnumerateDependentNodes (const NodePtr& node, const std::function<void (const NodePtr&)>& processor)
{
	EnumerateDependentNodes (node, [&] (const NodeId& dependentNodeId) {
		processor (GetNode (dependentNodeId));
	});
}

void NodeManager::EnumerateDependentNodesRecursive (const NodePtr& node, const std::function<void (const NodePtr&)>& processor)
{
	EnumerateDependentNodesRecursive (node, [&] (const NodeId& dependentNodeId) {
		processor (GetNode (dependentNodeId));
	});
}

void NodeManager::EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const
{
	EnumerateDependentNodes (node, [&] (const NodeId& dependentNodeId) {
		processor (GetNode (dependentNodeId));
	});
}

void NodeManager::EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const
{
	EnumerateDependentNodesRecursive (node, [&] (const NodeId& dependentNodeId) {
		processor (GetNode (dependentNodeId));
	});
}

bool NodeManager::ContainsNodeGroup (const NodeGroupId& groupId) const
{
	return nodeGroupList.Contains (groupId);
}

NodeGroupPtr NodeManager::AddNodeGroup (const NodeGroupPtr& group)
{
	return AddNodeGroup (group, IdPolicy::GenerateNew);
}

void NodeManager::MakeNodesAndGroupsSorted ()
{
	nodeList.MakeSorted ();
	nodeGroupList.MakeSorted ();
}

void NodeManager::DeleteNodeGroup (const NodeGroupId& groupId)
{
	return nodeGroupList.DeleteGroup (groupId);
}

void NodeManager::AddNodeToGroup (const NodeGroupId& groupId, const NodeId& nodeId)
{
	DBGASSERT (ContainsNode (nodeId));
	nodeGroupList.AddNodeToGroup (groupId, nodeId);
}

void NodeManager::RemoveNodeFromGroup (const NodeId& nodeId)
{
	nodeGroupList.RemoveNodeFromGroup (nodeId);
}

NodeGroupConstPtr NodeManager::GetNodeGroup (const NodeId& nodeId) const
{
	return nodeGroupList.GetNodeGroup (nodeId);
}

const NodeCollection& NodeManager::GetGroupNodes (const NodeGroupId& groupId) const
{
	return nodeGroupList.GetGroupNodes (groupId);
}

void NodeManager::EnumerateNodeGroups (const std::function<bool (NodeGroupConstPtr)>& processor) const
{
	nodeGroupList.Enumerate (processor);
}

void NodeManager::EnumerateNodeGroups (const std::function<bool (NodeGroupPtr)>& processor)
{
	nodeGroupList.Enumerate (processor);
}

void NodeManager::DeleteAllNodeGroups ()
{
	nodeGroupList.Clear ();
}

bool NodeManager::IsCalculationEnabled () const
{
	return updateMode == UpdateMode::Automatic || isForceCalculate;
}

NodeManager::UpdateMode NodeManager::GetUpdateMode () const
{
	return updateMode;
}

void NodeManager::SetUpdateMode (UpdateMode newUpdateMode)
{
	updateMode = newUpdateMode;
}

Stream::Status NodeManager::Read (InputStream& inputStream)
{
	return NodeManagerSerialization::Read (*this, inputStream);
}

Stream::Status NodeManager::Write (OutputStream& outputStream) const
{
	return NodeManagerSerialization::Write (*this, outputStream);
}

bool NodeManager::Clone (const NodeManager& source, NodeManager& target)
{
	if (DBGERROR (!target.IsEmpty ())) {
		return false;
	}

	MemoryOutputStream outputStream;
	if (DBGERROR (source.Write (outputStream) != Stream::Status::NoError)) {
		return false;
	}

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	if (DBGERROR (target.Read (inputStream) != Stream::Status::NoError)) {
		return false;
	}

	return true;
}

bool NodeManager::ReadFromBuffer (NodeManager& nodeManager, const std::vector<char>& buffer)
{
	MemoryInputStream inputStream (buffer);
	if (DBGERROR (nodeManager.Read (inputStream) != Stream::Status::NoError)) {
		return false;
	}
	return true;
}

bool NodeManager::WriteToBuffer (const NodeManager& nodeManager, std::vector<char>& buffer)
{
	MemoryOutputStream outputStream;
	if (DBGERROR (nodeManager.Write (outputStream) != Stream::Status::NoError)) {
		return false;
	}
	buffer = outputStream.GetBuffer ();
	return true;
}

NE::NodePtr NodeManager::AddNode (const NodePtr& node, IdPolicy idPolicy, InitPolicy initPolicy)
{
	if (DBGERROR (node == nullptr || node->IsEvaluatorSet ())) {
		return nullptr;
	}

	NodeId nodeId;
	if (idPolicy == IdPolicy::KeepOriginal) {
		nodeId = node->GetId ();
	} else if (idPolicy == IdPolicy::GenerateNew) {
		nodeId = idGenerator.GenerateNodeId ();
	} else {
		DBGBREAK ();
		return nullptr;
	}

	if (DBGERROR (ContainsNode (nodeId))) {
		return nullptr;
	}

	node->SetId (nodeId);
	node->SetEvaluator (nodeEvaluator);
	if (initPolicy == InitPolicy::Initialize) {
		node->Initialize ();
	}

	if (DBGERROR (!nodeList.AddNode (node->GetId (), node))) {
		return nullptr;
	}

	return node;
}

NE::NodeGroupPtr NodeManager::AddNodeGroup (const NodeGroupPtr& group, IdPolicy idHandling)
{
	if (DBGERROR (group == nullptr)) {
		return nullptr;
	}

	NodeGroupId groupId;
	if (idHandling == IdPolicy::KeepOriginal) {
		groupId = group->GetId ();
	} else if (idHandling == IdPolicy::GenerateNew) {
		groupId = idGenerator.GenerateNodeGroupId ();
	} else {
		DBGBREAK ();
		return nullptr;
	}

	if (DBGERROR (ContainsNodeGroup (groupId))) {
		return nullptr;
	}

	group->SetId (groupId);
	if (DBGERROR (!nodeGroupList.AddGroup (group))) {
		return nullptr;
	}

	return group;
}

}
