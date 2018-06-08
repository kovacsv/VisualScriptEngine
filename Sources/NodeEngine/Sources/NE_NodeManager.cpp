#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_Debug.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

SerializationInfo NodeManager::serializationInfo (ObjectVersion (1));

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

bool AllNodesFilter::NeedToProcessNode (const NodeId&) const
{
	return true;
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
	NodeManagerNodeEvaluatorSetter (const NodeId& newNodeId, const NodeEvaluatorConstPtr& newNodeEvaluator, InitializationMode initMode) :
		newNodeId (newNodeId),
		newNodeEvaluator (newNodeEvaluator),
		initMode (initMode)
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

	virtual InitializationMode GetInitializationMode () const override
	{
		return initMode;
	}

private:
	const NodeId&					newNodeId;
	const NodeEvaluatorConstPtr&	newNodeEvaluator;
	InitializationMode				initMode;
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

size_t NodeManager::GetConnectionCount () const
{
	return connectionManager.GetConnectionCount ();
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
	return AddUninitializedNode (node);
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

	Stream::Status nodeStatus = ReadNodes (inputStream, IdHandlingPolicy::KeepOriginalId);
	if (DBGERROR (nodeStatus != Stream::Status::NoError)) {
		return nodeStatus;
	}

	return inputStream.GetStatus ();
}

Stream::Status NodeManager::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	idGenerator.Write (outputStream);

	AllNodesFilter allNodesFilter;
	Stream::Status nodeStatus = WriteNodes (outputStream, allNodesFilter);
	if (DBGERROR (nodeStatus != Stream::Status::NoError)) {
		return nodeStatus;
	}

	return outputStream.GetStatus ();
}

bool NodeManager::Append (const NodeManager& source, const NodeFilter& nodeFilter)
{
	std::vector<NodeConstPtr> nodesToClone;
	source.EnumerateNodes ([&] (const NodeConstPtr& node) {
		if (!nodeFilter.NeedToProcessNode (node->GetId ())) {
			return true;
		}
		nodesToClone.push_back (node);
		return true;
	});

	std::unordered_map<NodeId, NodeId> oldToNewNodeIdTable;
	for (const NodeConstPtr& node : nodesToClone) {
		NodePtr cloned = Node::Clone (node);
		AddInitializedNode (cloned, IdHandlingPolicy::GenerateNewId);
		oldToNewNodeIdTable.insert ({ node->GetId (), cloned->GetId () });
	}

	bool success = true;
	EnumerateConnectionsOrdered (source, nodesToClone, [&] (const ConnectionInfo& connection) {
		if (!nodeFilter.NeedToProcessNode (connection.GetOutputNodeId ())) {
			return;
		}
		NodeConstPtr outputNode = GetNode (oldToNewNodeIdTable[connection.GetOutputNodeId ()]);
		NodeConstPtr inputNode = GetNode (oldToNewNodeIdTable[connection.GetInputNodeId ()]);
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
		if (DBGERROR (!ConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
			success = false;
			return;
		}
	});

	return success;
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

NodePtr NodeManager::AddUninitializedNode (const NodePtr& node)
{
	if (DBGERROR (node == nullptr || node->HasNodeEvaluator () || node->GetId () != NullNodeId)) {
		return nullptr;
	}

	NodeId newNodeId (idGenerator.GenerateUniqueId ());
	NodeManagerNodeEvaluatorSetter setter (newNodeId, nodeEvaluator, InitializationMode::Initialize);
	return AddNode (node, setter);
}

NodePtr NodeManager::AddInitializedNode (const NodePtr& node, IdHandlingPolicy idHandling)
{
	if (DBGERROR (node == nullptr || node->HasNodeEvaluator () || node->GetId () == NullNodeId)) {
		return nullptr;
	}

	NodeId newNodeId;
	if (idHandling == IdHandlingPolicy::KeepOriginalId) {
		newNodeId = node->GetId ();
	} else if (idHandling == IdHandlingPolicy::GenerateNewId) {
		newNodeId = idGenerator.GenerateUniqueId ();
	}

	NodeManagerNodeEvaluatorSetter setter (newNodeId, nodeEvaluator, InitializationMode::DoNotInitialize);
	return AddNode (node, setter);
}

Stream::Status NodeManager::ReadNodes (InputStream& inputStream, IdHandlingPolicy idHandling)
{
	std::unordered_map<NodeId, NodeId> oldToNewNodeIdTable;

	size_t nodeCount = 0;
	inputStream.Read (nodeCount);
	for (size_t i = 0; i < nodeCount; ++i) {
		NodePtr node (ReadDynamicObject<Node> (inputStream));
		NodeId oldNodeId = node->GetId ();
		NodePtr addedNode = AddInitializedNode (node, idHandling);
		if (DBGERROR (addedNode == nullptr)) {
			return Stream::Status::Error;
		}
		oldToNewNodeIdTable.insert ({ oldNodeId, addedNode->GetId () });
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

		NodePtr outputNode = GetNode (oldToNewNodeIdTable[outputNodeId]);
		NodePtr inputNode = GetNode (oldToNewNodeIdTable[inputNodeId]);
		if (DBGERROR (outputNode == nullptr || inputNode == nullptr)) {
			return Stream::Status::Error;
		}
		if (DBGERROR (!ConnectOutputSlotToInputSlot (outputNode->GetOutputSlot (outputSlotId), inputNode->GetInputSlot (inputSlotId)))) {
			return Stream::Status::Error;
		}
	}

	return inputStream.GetStatus ();
}

Stream::Status NodeManager::WriteNodes (OutputStream& outputStream, const NodeFilter& nodeFilter) const
{
	std::vector<NodeConstPtr> nodesToWrite;
	EnumerateNodes ([&] (const NodeConstPtr& node) {
		if (nodeFilter.NeedToProcessNode (node->GetId ())) {
			nodesToWrite.push_back (node);
		}
		return true;
	});
	std::sort (nodesToWrite.begin (), nodesToWrite.end (), [&] (const NodeConstPtr& a, const NodeConstPtr& b) -> bool {
		return a->GetId () < b->GetId ();
	});

	outputStream.Write (nodesToWrite.size ());
	for (const NodeConstPtr& node : nodesToWrite) {
		WriteDynamicObject (outputStream, node.get ());
	};

	std::vector<ConnectionInfo> connectionsToWrite;
	EnumerateConnectionsOrdered (*this, nodesToWrite, [&] (const ConnectionInfo& connection) {
		if (!nodeFilter.NeedToProcessNode (connection.GetOutputNodeId ())) {
			return;
		}
		connectionsToWrite.push_back (connection);
	});

	outputStream.Write (connectionsToWrite.size ());
	for (const ConnectionInfo& connection : connectionsToWrite) {
		connection.GetOutputNodeId ().Write (outputStream);
		connection.GetOutputSlotId ().Write (outputStream);
		connection.GetInputNodeId ().Write (outputStream);
		connection.GetInputSlotId ().Write (outputStream);
	}

	return outputStream.GetStatus ();
}

}
