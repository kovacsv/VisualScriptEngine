#include "NUIE_NodeCollection.hpp"

#include <algorithm>

namespace NUIE
{

NE::SerializationInfo NodeCollection::serializationInfo (NE::ObjectVersion (1));

NodeCollection::NodeCollection ()
{

}

NodeCollection::NodeCollection (const NE::NodeId& nodeId) :
	nodes ({ nodeId }),
	nodeSet ({ nodeId })
{

}

NodeCollection::~NodeCollection ()
{

}

bool NodeCollection::IsEmpty () const
{
	DBGASSERT (nodes.empty () == nodeSet.empty ());
	return nodes.empty ();
}

bool NodeCollection::Contains (const NE::NodeId& nodeId) const
{
	return nodeSet.find (nodeId) != nodeSet.end ();
}

size_t NodeCollection::Count () const
{
	return nodes.size ();
}

NE::NodeId NodeCollection::GetLast () const
{
	if (nodes.empty ()) {
		return NE::NullNodeId;
	}
	return nodes.back ();
}

void NodeCollection::Enumerate (const std::function<bool (const NE::NodeId&)>& processor) const
{
	for (const NE::NodeId& nodeId : nodes) {
		if (!processor (nodeId)) {
			return;
		}
	}
}

void NodeCollection::Insert (const NE::NodeId& nodeId)
{
	nodes.push_back (nodeId);
	nodeSet.insert (nodeId);
}

void NodeCollection::Erase (const NE::NodeId& nodeId)
{
	if (Contains (nodeId)) {
		nodes.erase (std::find (nodes.begin (), nodes.end (), nodeId));
		nodeSet.erase (nodeId);
	}
}

void NodeCollection::Clear ()
{
	nodes.clear ();
	nodeSet.clear ();
}

bool NodeCollection::operator== (const NodeCollection& rhs) const
{
	return nodes == rhs.nodes && nodeSet == rhs.nodeSet;
}

bool NodeCollection::operator!= (const NodeCollection& rhs) const
{
	return !operator== (rhs);
}

NE::Stream::Status NodeCollection::Read (NE::InputStream& inputStream)
{
	DBGASSERT (IsEmpty ());
	NE::ObjectHeader header (inputStream);
	size_t nodeCount = 0;
	inputStream.Read (nodeCount);
	for (size_t i = 0; i < nodeCount; i++) {
		NE::NodeId nodeId;
		nodeId.Read (inputStream);
		Insert (nodeId);
	}
	return inputStream.GetStatus ();
}

NE::Stream::Status NodeCollection::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (nodes.size ());
	for (const NE::NodeId& nodeId : nodes) {
		nodeId.Write (outputStream);
	}
	return outputStream.GetStatus ();
}

}
