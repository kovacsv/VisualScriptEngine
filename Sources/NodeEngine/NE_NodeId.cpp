#include "NE_NodeId.hpp"

namespace NE
{

SERIALIZATION_INFO (NodeId, 1);

NodeId::NodeId () :
	id (0)
{

}

NodeId::NodeId (const NodeIdType& uniqueId) :
	id (uniqueId)
{

}

NodeId::~NodeId ()
{

}

NodeIdType NodeId::GetUniqueId () const
{
	return id;
}

void NodeId::SetUniqueId (NodeIdType uniqueId)
{
	id = uniqueId;
}

size_t NodeId::GenerateHashValue () const
{
	return std::hash<NodeIdType> {} (id);
}

bool NodeId::operator< (const NodeId& rhs) const
{
	return id < rhs.id;
}

bool NodeId::operator> (const NodeId& rhs) const
{
	return id > rhs.id;
}
	
bool NodeId::operator== (const NodeId& rhs) const
{
	return id == rhs.id;
}

bool NodeId::operator!= (const NodeId& rhs) const
{
	return !operator== (rhs);
}

Stream::Status NodeId::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	inputStream.Read (id);
	return inputStream.GetStatus ();
}

Stream::Status NodeId::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (id);
	return outputStream.GetStatus ();
}

const NodeId NullNodeId;

}
