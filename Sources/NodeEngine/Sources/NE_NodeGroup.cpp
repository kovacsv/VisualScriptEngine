#include "NE_NodeGroup.hpp"

namespace NE
{

SerializationInfo NodeGroup::serializationInfo (ObjectVersion (1));

NodeGroup::NodeGroup ()
{

}

NodeGroup::NodeGroup (const NodeCollection& nodes) :
	nodes (nodes)
{

}

NodeGroup::~NodeGroup ()
{

}

bool NodeGroup::IsEmpty () const
{
	return nodes.IsEmpty ();
}

bool NodeGroup::ContainsNode (const NodeId& nodeId) const
{
	return nodes.Contains (nodeId);
}

bool NodeGroup::AddNode (const NodeId& nodeId)
{
	if (nodes.Contains (nodeId)) {
		return false;
	}
	nodes.Insert (nodeId);
	return true;
}

bool NodeGroup::DeleteNode (const NodeId& nodeId)
{
	if (!nodes.Contains (nodeId)) {
		return false;
	}
	nodes.Erase (nodeId);
	return true;
}

const NodeCollection& NodeGroup::GetNodes () const
{
	return nodes;
}

Stream::Status NodeGroup::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	nodes.Read (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status NodeGroup::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	nodes.Write (outputStream);
	return outputStream.GetStatus ();
}

}
