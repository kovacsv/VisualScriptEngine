#include "NUIE_NodeCollection.hpp"

#include <algorithm>

namespace NUIE
{

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

}
