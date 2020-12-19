#include "NE_UniqueIdGenerator.hpp"

namespace NE
{

SERIALIZATION_INFO (UniqueIdGenerator, 2);

UniqueIdGenerator::UniqueIdGenerator () :
	nextNodeId (1),
	nextNodeGroupId (1)
{

}

UniqueIdGenerator::~UniqueIdGenerator ()
{

}

void UniqueIdGenerator::Clear ()
{
	nextNodeId = 1;
	nextNodeGroupId = 1;
}

NodeId UniqueIdGenerator::GenerateNodeId ()
{
	IdType newNodeId = nextNodeId++;
	return NodeId (newNodeId);
}

NodeGroupId UniqueIdGenerator::GenerateNodeGroupId ()
{
	IdType newNodeGroupId = nextNodeGroupId++;
	return NodeGroupId (newNodeGroupId);
}

Stream::Status UniqueIdGenerator::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	IdType nextNodeIdFromStream = 0;
	inputStream.Read (nextNodeIdFromStream);
	nextNodeId = nextNodeIdFromStream;
	if (header.GetVersion () > 1) {
		IdType nextNodeGroupIdFromStream = 0;
		inputStream.Read (nextNodeGroupIdFromStream);
		nextNodeGroupId = nextNodeGroupIdFromStream;
	}
	return inputStream.GetStatus ();
}

Stream::Status UniqueIdGenerator::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (nextNodeId);
	outputStream.Write (nextNodeGroupId);
	return outputStream.GetStatus ();
}

}
