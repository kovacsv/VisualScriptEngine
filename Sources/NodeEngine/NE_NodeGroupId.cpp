#include "NE_NodeGroupId.hpp"

namespace NE
{

SERIALIZATION_INFO (NodeGroupId, 1);

NodeGroupId::NodeGroupId () :
	id (0)
{

}

NodeGroupId::NodeGroupId (const NodeGroupIdType& uniqueId) :
	id (uniqueId)
{

}

NodeGroupId::~NodeGroupId ()
{

}

NodeGroupIdType NodeGroupId::GetUniqueId () const
{
	return id;
}

void NodeGroupId::SetUniqueId (NodeGroupIdType uniqueId)
{
	id = uniqueId;
}

size_t NodeGroupId::GenerateHashValue () const
{
	return std::hash<NodeGroupIdType> {} (id);
}

bool NodeGroupId::operator< (const NodeGroupId& rhs) const
{
	return id < rhs.id;
}

bool NodeGroupId::operator> (const NodeGroupId& rhs) const
{
	return id > rhs.id;
}
	
bool NodeGroupId::operator== (const NodeGroupId& rhs) const
{
	return id == rhs.id;
}

bool NodeGroupId::operator!= (const NodeGroupId& rhs) const
{
	return !operator== (rhs);
}

Stream::Status NodeGroupId::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	inputStream.Read (id);
	return inputStream.GetStatus ();
}

Stream::Status NodeGroupId::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (id);
	return outputStream.GetStatus ();
}

const NodeGroupId NullNodeGroupId;

}
