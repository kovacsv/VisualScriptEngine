#include "NE_NodeCollection.hpp"

#include <algorithm>

namespace NE
{

SERIALIZATION_INFO (NodeCollection, 1);

NodeCollection::NodeCollection ()
{

}

NodeCollection::NodeCollection (const std::vector<NodeId>& nodeIds)
{
	for (const NodeId& nodeId : nodeIds) {
		Insert (nodeId);
	}
}

NodeCollection::~NodeCollection ()
{

}

bool NodeCollection::IsEmpty () const
{
	DBGASSERT (nodes.empty () == nodeSet.empty ());
	return nodes.empty ();
}

bool NodeCollection::Contains (const NodeId& nodeId) const
{
	return nodeSet.find (nodeId) != nodeSet.end ();
}

size_t NodeCollection::Count () const
{
	return nodes.size ();
}

const NodeId& NodeCollection::Get (size_t index) const
{
	return nodes[index];
}

void NodeCollection::Enumerate (const std::function<bool (const NodeId&)>& processor) const
{
	for (const NodeId& nodeId : nodes) {
		if (!processor (nodeId)) {
			return;
		}
	}
}

void NodeCollection::Insert (const NodeId& nodeId)
{
	nodes.push_back (nodeId);
	nodeSet.insert (nodeId);
}

void NodeCollection::Erase (const NodeId& nodeId)
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

Stream::Status NodeCollection::Read (InputStream& inputStream)
{
	DBGASSERT (IsEmpty ());
	ObjectHeader header (inputStream);
	size_t nodeCount = 0;
	inputStream.Read (nodeCount);
	for (size_t i = 0; i < nodeCount; i++) {
		NodeId nodeId;
		nodeId.Read (inputStream);
		Insert (nodeId);
	}
	return inputStream.GetStatus ();
}

Stream::Status NodeCollection::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (nodes.size ());
	for (const NodeId& nodeId : nodes) {
		nodeId.Write (outputStream);
	}
	return outputStream.GetStatus ();
}

const NodeCollection EmptyNodeCollection;

}
