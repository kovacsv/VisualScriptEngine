#include "NodeManager.hpp"
#include "Node.hpp"
#include "Debug.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"

namespace NE
{

SerializationInfo NodeManager::serializationInfo (ObjectId ("{89708300-2783-452E-89A1-D842283C3B3F}"), ObjectVersion (1));

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

	virtual bool IsNodeValueCalculated (const NodeId& nodeId) const override
	{
		return nodeValueCache.Contains (nodeId);
	}

	virtual ValuePtr GetCalculatedNodeValue (const NodeId& nodeId) const override
	{
		return nodeValueCache.Get (nodeId);
	}

	virtual void SetCalculatedNodeValue (const NodeId& nodeId, const ValuePtr& valuePtr) const override
	{
		nodeValueCache.Add (nodeId, valuePtr);
	}

private:
	const NodeManager&	nodeManager;
	NodeValueCache&		nodeValueCache;
};

class NodeManagerNodeEvaluatorSetter : public NodeEvaluatorSetter
{
public:
	NodeManagerNodeEvaluatorSetter (const NodeId& newNodeId, const NodeEvaluatorConstPtr& newNodeEvaluator) :
		newNodeId (newNodeId),
		newNodeEvaluator (newNodeEvaluator)
	{
		
	}

	virtual const NodeId& GetNodeId () const override
	{
		return newNodeId;
	}

	virtual const NodeEvaluatorConstPtr& GetNodeEvaluator () const override
	{
		return newNodeEvaluator;
	}

private:
	const NodeId&					newNodeId;
	const NodeEvaluatorConstPtr&	newNodeEvaluator;
};

class NodeManagerNodeEvaluatorSetterForAdd : public NodeManagerNodeEvaluatorSetter
{
public:
	NodeManagerNodeEvaluatorSetterForAdd (const NodeId& newNodeId, const NodeEvaluatorConstPtr& newNodeEvaluator) :
		NodeManagerNodeEvaluatorSetter (newNodeId, newNodeEvaluator)
	{
		
	}

	virtual bool NeedToRegisterSlots () const override
	{
		return true;
	}
};

class NodeManagerNodeEvaluatorSetterForRead : public NodeManagerNodeEvaluatorSetter
{
public:
	NodeManagerNodeEvaluatorSetterForRead (const NodeId& newNodeId, const NodeEvaluatorConstPtr& newNodeEvaluator) :
		NodeManagerNodeEvaluatorSetter (newNodeId, newNodeEvaluator)
	{
		
	}

	virtual bool NeedToRegisterSlots () const override
	{
		return false;
	}
};

NodeManager::NodeManager () :
	idGenerator (),
	nodeIdToNodeTable (),
	connectionManager (),
	nodeValueCache (),
	nodeEvaluator (new NodeManagerNodeEvaluator (*this, nodeValueCache))
{

}

NodeManager::~NodeManager ()
{

}

void NodeManager::Clear ()
{
	nodeIdToNodeTable.clear ();
	connectionManager.Clear ();
	nodeValueCache.Clear ();
}

bool NodeManager::IsEmpty () const
{
	return nodeIdToNodeTable.empty () && connectionManager.IsEmpty ();
}

size_t NodeManager::GetNodeCount () const
{
	return nodeIdToNodeTable.size ();
}

void NodeManager::EnumerateNodes (const std::function<bool (const NodePtr&)>& processor)
{
	for (auto& node : nodeIdToNodeTable) {
		if (!processor (node.second)) {
			break;
		}
	}
}

void NodeManager::EnumerateNodes (const std::function<bool (const NodeConstPtr&)>& processor) const
{
	for (const auto& node : nodeIdToNodeTable) {
		if (!processor (node.second)) {
			break;
		}
	}
}

bool NodeManager::ContainsNode (const NodeId& id) const
{
	return nodeIdToNodeTable.find (id) != nodeIdToNodeTable.end ();
}

NodeConstPtr NodeManager::GetNode (const NodeId& id) const
{
	auto foundNode = nodeIdToNodeTable.find (id);
	if (DBGERROR (foundNode == nodeIdToNodeTable.end ())) {
		return nullptr;
	}
	return foundNode->second;
}

NodePtr NodeManager::GetNode (const NodeId& id)
{
	auto foundNode = nodeIdToNodeTable.find (id);
	if (DBGERROR (foundNode == nodeIdToNodeTable.end ())) {
		return nullptr;
	}
	return foundNode->second;
}

NodePtr NodeManager::AddNode (const NodePtr& node)
{
	if (DBGERROR (node == nullptr || node->HasNodeEvaluator () || node->GetId () != NullNodeId)) {
		return nullptr;
	}

	NodeId newNodeId (idGenerator.GenerateUniqueId ());
	NodeManagerNodeEvaluatorSetterForAdd setter (newNodeId, nodeEvaluator);
	return AddNode (node, setter);
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
	if (DBGERROR (node->GetId () == NullNodeId || !node->HasNodeEvaluator ())) {
		return false;
	}

	if (DBGERROR (!ContainsNode (node->GetId ()))) {
		return false;
	}

	node->InvalidateValue ();

	node->EnumerateInputSlots ([this] (const InputSlotPtr& inputSlot) -> bool {
		connectionManager.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
		return true;
	});

	node->EnumerateOutputSlots ([this] (const OutputSlotConstPtr& outputSlot) -> bool {
		connectionManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
		return true;
	});

	nodeIdToNodeTable.erase (node->GetId ());
	node->ClearNodeEvaluator ();

	return true;
}

bool NodeManager::IsOutputSlotConnectedToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const
{
	if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
		return false;
	}
	return connectionManager.IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot);
}

bool NodeManager::CanConnectMoreOutputSlotToInputSlot (const InputSlotConstPtr& inputSlot) const
{
	return connectionManager.CanConnectMoreOutputSlotToInputSlot (inputSlot);
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
		return true;
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

bool NodeManager::ConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot)
{
	if (!CanConnectOutputSlotToInputSlot (outputSlot, inputSlot)) {
		return false;
	}

	InvalidateNodeValue (GetNode (inputSlot->GetOwnerNodeId ()));
	return connectionManager.ConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}

bool NodeManager::DisconnectOutputSlotFromInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot)
{
	InvalidateNodeValue (GetNode (inputSlot->GetOwnerNodeId ()));
	return connectionManager.DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot);
}

void NodeManager::EnumerateConnections (const std::function<void (const OutputSlotConstPtr&, const InputSlotConstPtr&)>& processor) const
{
	connectionManager.EnumerateConnections (processor);
}

void NodeManager::EnumerateConnections (const std::function<void (const NodeConstPtr&, const OutputSlotConstPtr&, const NodeConstPtr&, const InputSlotConstPtr&)>& processor) const
{
	connectionManager.EnumerateConnections ([&] (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) {
		if (DBGERROR (!ContainsNode (outputSlot->GetOwnerNodeId ()) || !ContainsNode (inputSlot->GetOwnerNodeId ()))) {
			return;
		}
		NodeConstPtr outputNode = GetNode (outputSlot->GetOwnerNodeId ());
		NodeConstPtr inputNode = GetNode (inputSlot->GetOwnerNodeId ());
		processor (outputNode, outputSlot, inputNode, inputSlot);
	});
}

void NodeManager::EnumerateConnections (const std::function<void (const NodeId&, const SlotId&, const NodeId&, const SlotId&)>& processor) const
{
	connectionManager.EnumerateConnections ([&] (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) {
		processor (outputSlot->GetOwnerNodeId (), outputSlot->GetId (), inputSlot->GetOwnerNodeId (), inputSlot->GetId ());
	});
}

bool NodeManager::HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const
{
	return connectionManager.HasConnectedOutputSlots (inputSlot);
}

size_t NodeManager::GetConnectedOutputSlotCount (const InputSlotConstPtr& inputSlot) const
{
	return connectionManager.GetConnectedOutputSlotCount (inputSlot);
}

void NodeManager::EnumerateConnectedInputSlots (const OutputSlotConstPtr& outputSlot, const std::function<void (const InputSlotConstPtr&)>& processor) const
{
	connectionManager.EnumerateConnectedInputSlots (outputSlot, processor);
}

void NodeManager::EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const
{
	connectionManager.EnumerateConnectedOutputSlots (inputSlot, processor);
}

void NodeManager::EvaluateAllNodes (EvaluationEnv& env) const
{
	EnumerateNodes ([&] (const NodeConstPtr& node) -> bool {
		node->Evaluate (env);
		return true;
	});
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
	node->EnumerateOutputSlots ([&] (const OutputSlotConstPtr& outputSlot) -> bool {
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

Stream::Status NodeManager::Read (InputStream& inputStream)
{
	if (DBGERROR (!IsEmpty ())) {
		return Stream::Status::Error;
	}

	ObjectHeader header (inputStream);
	idGenerator.Read (inputStream);

	size_t nodeCount = 0;
	inputStream.Read (nodeCount);
	for (size_t i = 0; i < nodeCount; ++i) {
		NodePtr node (ReadDynamicObject<Node> (inputStream));
		if (DBGERROR (node == nullptr || node->HasNodeEvaluator () || node->GetId () == NullNodeId)) {
			return Stream::Status::Error;
		}
		NodeManagerNodeEvaluatorSetterForRead setter (node->GetId (), nodeEvaluator);
		if (DBGERROR (AddNode (node, setter) == nullptr)) {
			return Stream::Status::Error;
		}
	}

	size_t connectionCount = 0;
	inputStream.Read (connectionCount);
	for (size_t i = 0; i < connectionCount; ++i) {
		NodeId outputNodeId;
		SlotId outputSlotId;
		NodeId inputNodeId;
		SlotId inputSlotId;
		
		outputNodeId.Read (inputStream);
		outputSlotId.Read (inputStream);
		inputNodeId.Read (inputStream);
		inputSlotId.Read (inputStream);
		
		NodePtr outputNode = GetNode (outputNodeId);
		NodePtr inputNode = GetNode (inputNodeId);
		if (DBGERROR (outputNode == nullptr || inputNode == nullptr)) {
			return Stream::Status::Error;
		}
		if (DBGERROR (!ConnectOutputSlotToInputSlot (outputNode->GetOutputSlot (outputSlotId), inputNode->GetInputSlot (inputSlotId)))) {
			return Stream::Status::Error;
		}
	}

	return inputStream.GetStatus ();
}

Stream::Status NodeManager::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	idGenerator.Write (outputStream);

	size_t nodeCount = nodeIdToNodeTable.size ();
	outputStream.Write (nodeCount);
	EnumerateNodes ([&] (const NodeConstPtr& node) {
		WriteDynamicObject (outputStream, node.get ());
		return true;
	});

	size_t connectionCount = connectionManager.GetConnectionCount ();
	outputStream.Write (connectionCount);
	EnumerateConnections ([&] (const NodeId& outputNodeId, const SlotId& outputSlotId, const NodeId& inputNodeId, const SlotId& inputSlotId) {
		outputNodeId.Write (outputStream);
		outputSlotId.Write (outputStream);
		inputNodeId.Write (outputStream);
		inputSlotId.Write (outputStream);
	});

	return outputStream.GetStatus ();
}

NodePtr NodeManager::AddNode (const NodePtr& node, const NodeEvaluatorSetter& setter)
{
	if (DBGERROR (ContainsNode (setter.GetNodeId ()))) {
		return nullptr;
	}
	node->SetNodeEvaluator (setter);
	nodeIdToNodeTable.insert ({ node->GetId (), node });
	return node;
}

}
