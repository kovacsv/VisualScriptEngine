#include "NE_NodeList.hpp"
#include "NE_Debug.hpp"

namespace NE
{

NodeList::NodeList () :
	nodes ()
{

}

NodeList::~NodeList ()
{

}

bool NodeList::IsEmpty () const
{
	return nodes.IsEmpty ();
}

size_t NodeList::Count () const
{
	return nodes.Count ();
}

bool NodeList::ContainsNode (const NodeId& nodeId) const
{
	return nodes.Contains (nodeId);
}

NE::NodePtr NodeList::GetNode (const NodeId& nodeId)
{
	if (DBGERROR (!nodes.Contains (nodeId))) {
		return nullptr;
	}
	return nodes.GetValue (nodeId);
}

NE::NodeConstPtr NodeList::GetNode (const NodeId& nodeId) const
{
	if (DBGERROR (!nodes.Contains (nodeId))) {
		return nullptr;
	}
	return nodes.GetValue (nodeId);
}

bool NodeList::AddNode (const NodeId& nodeId, const NodePtr& nodePtr)
{
	if (DBGERROR (nodeId == NullNodeId)) {
		return false;
	}
	return nodes.Insert (nodeId, nodePtr);
}

bool NodeList::DeleteNode (const NodeId& nodeId)
{
	return nodes.Erase (nodeId);
}

void NodeList::Clear ()
{
	nodes.Clear ();
}

void NodeList::Enumerate (const std::function<bool (const NodePtr&)>& processor)
{
	nodes.Enumerate ([&] (const NodePtr& node) {
		return processor (node);
	});
}

void NodeList::Enumerate (const std::function<bool (const NodeConstPtr&)>& processor) const
{
	nodes.Enumerate ([&] (const NodePtr& node) {
		return processor (node);
	});
}

}
