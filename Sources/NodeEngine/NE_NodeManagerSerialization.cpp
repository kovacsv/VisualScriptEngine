#include "NE_NodeManagerSerialization.hpp"

namespace NE
{

Stream::Status NodeManagerSerialization::Read (NodeManager& nodeManager, InputStream& inputStream)
{
	if (DBGERROR (!nodeManager.IsEmpty ())) {
		return Stream::Status::Error;
	}

	ObjectHeader header (inputStream);
	nodeManager.idGenerator.Read (inputStream);

	Stream::Status nodeStatus = ReadNodes (nodeManager, inputStream, header.GetVersion ());
	if (DBGERROR (nodeStatus != Stream::Status::NoError)) {
		return nodeStatus;
	}

	Stream::Status connectionStatus = ReadConnections (nodeManager, inputStream, header.GetVersion ());
	if (DBGERROR (connectionStatus != Stream::Status::NoError)) {
		return connectionStatus;
	}

	Stream::Status groupStatus = ReadGroups (nodeManager, inputStream, header.GetVersion ());
	if (DBGERROR (groupStatus != Stream::Status::NoError)) {
		return groupStatus;
	}

	if (header.GetVersion () < 4) {
		nodeManager.MakeNodesAndGroupsSorted ();
	}

	ReadEnum (inputStream, nodeManager.updateMode);

	return inputStream.GetStatus ();
}

Stream::Status NodeManagerSerialization::Write (const NodeManager& nodeManager, OutputStream& outputStream)
{
	ObjectHeader header (outputStream, nodeManager.serializationInfo);
	nodeManager.idGenerator.Write (outputStream);

	Stream::Status nodeStatus = WriteNodes (nodeManager, outputStream);
	if (DBGERROR (nodeStatus != Stream::Status::NoError)) {
		return nodeStatus;
	}

	Stream::Status connectionStatus = WriteConnections (nodeManager, outputStream);
	if (DBGERROR (connectionStatus != Stream::Status::NoError)) {
		return connectionStatus;
	}

	Stream::Status groupStatus = WriteGroups (nodeManager, outputStream);
	if (DBGERROR (groupStatus != Stream::Status::NoError)) {
		return groupStatus;
	}

	WriteEnum (outputStream, nodeManager.updateMode);

	return outputStream.GetStatus ();
}

Stream::Status NodeManagerSerialization::ReadNodes (NodeManager& nodeManager, InputStream& inputStream, const ObjectVersion&)
{
	DBGASSERT (nodeManager.GetNodeCount () == 0);

	size_t nodeCount = 0;
	inputStream.Read (nodeCount);
	for (size_t i = 0; i < nodeCount; ++i) {
		NodePtr node (ReadDynamicObject<Node> (inputStream));
		NodeId oldNodeId = node->GetId ();
		NodePtr addedNode = nodeManager.AddNode (node, NodeManager::IdPolicy::KeepOriginal, NodeManager::InitPolicy::DoNotInitialize);
		DBGASSERT (oldNodeId == addedNode->GetId ());
		if (DBGERROR (addedNode == nullptr)) {
			return Stream::Status::Error;
		}
	}

	return inputStream.GetStatus ();
}

Stream::Status NodeManagerSerialization::ReadConnections (NodeManager& nodeManager, InputStream& inputStream, const ObjectVersion& version)
{
	DBGASSERT (nodeManager.GetConnectionCount () == 0);

	size_t connectionCount = 0;
	inputStream.Read (connectionCount);
	for (size_t i = 0; i < connectionCount; ++i) {
		ConnectionInfo connection;
		if (version < 3) {
			NodeId outputNodeId;
			SlotId outputSlotId;
			NodeId inputNodeId;
			SlotId inputSlotId;

			outputNodeId.Read (inputStream);
			outputSlotId.Read (inputStream);
			inputNodeId.Read (inputStream);
			inputSlotId.Read (inputStream);

			SlotInfo outputSlotInfo (outputNodeId, outputSlotId);
			SlotInfo inputSlotInfo (inputNodeId, inputSlotId);
			connection = ConnectionInfo (outputSlotInfo, inputSlotInfo);
		} else {
			connection.Read (inputStream);
		}

		NodePtr outputNode = nodeManager.GetNode (connection.GetOutputNodeId ());
		NodePtr inputNode = nodeManager.GetNode (connection.GetInputNodeId ());
		if (DBGERROR (outputNode == nullptr || inputNode == nullptr)) {
			return Stream::Status::Error;
		}
		OutputSlotConstPtr outputSlot = outputNode->GetOutputSlot (connection.GetOutputSlotId ());
		InputSlotConstPtr inputSlot = inputNode->GetInputSlot (connection.GetInputSlotId ());
		if (DBGERROR (!nodeManager.ConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
			return Stream::Status::Error;
		}
	}

	return inputStream.GetStatus ();
}

Stream::Status NodeManagerSerialization::ReadGroups (NodeManager& nodeManager, InputStream& inputStream, const ObjectVersion& version)
{
	DBGASSERT (nodeManager.GetNodeGroupCount () == 0);

	if (version < 2) {
		ObjectHeader legacyGroupListHeader (inputStream);
		if (DBGERROR (legacyGroupListHeader.GetVersion () != 1)) {
			return Stream::Status::Error;
		}
	}

	size_t groupCount = 0;
	inputStream.Read (groupCount);
	for (size_t i = 0; i < groupCount; i++) {
		NodeGroupPtr group (ReadDynamicObject<NodeGroup> (inputStream));
		if (version < 2) {
			nodeManager.AddNodeGroup (group, NodeManager::IdPolicy::GenerateNew);
		} else {
			nodeManager.AddNodeGroup (group, NodeManager::IdPolicy::KeepOriginal);
		}

		NodeCollection nodes;
		nodes.Read (inputStream);
		nodes.Enumerate ([&] (const NodeId& nodeId) {
			nodeManager.AddNodeToGroup (group->GetId (), nodeId);
			return true;
		});
	}

	return inputStream.GetStatus ();
}

Stream::Status NodeManagerSerialization::WriteNodes (const NodeManager& nodeManager, OutputStream& outputStream)
{
	outputStream.Write (nodeManager.GetNodeCount ());
	nodeManager.EnumerateNodes ([&] (NodeConstPtr node) {
		WriteDynamicObject (outputStream, node.get ());
		return true;
	});

	return outputStream.GetStatus ();
}

Stream::Status NodeManagerSerialization::WriteConnections (const NodeManager& nodeManager, OutputStream& outputStream)
{
	outputStream.Write (nodeManager.GetConnectionCount ());
	nodeManager.EnumerateConnections ([&] (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) {
		ConnectionInfo connection (
			SlotInfo (outputSlot->GetOwnerNodeId (), outputSlot->GetId ()),
			SlotInfo (inputSlot->GetOwnerNodeId (), inputSlot->GetId ())
		);
		connection.Write (outputStream);
	});

	return outputStream.GetStatus ();
}

Stream::Status NodeManagerSerialization::WriteGroups (const NodeManager& nodeManager, OutputStream& outputStream)
{
	outputStream.Write (nodeManager.GetNodeGroupCount ());
	nodeManager.EnumerateNodeGroups ([&] (NodeGroupConstPtr group) {
		WriteDynamicObject (outputStream, group.get ());
		const NodeCollection& nodes = nodeManager.GetGroupNodes (group->GetId ());
		nodes.Write (outputStream);
		return true;
	});
	return outputStream.GetStatus ();
}

}
