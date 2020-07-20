#include "NE_NodeList.hpp"
#include "NE_Debug.hpp"

namespace NE
{

NodeList::NodeList () :
	nodeIdList (),
	nodeIdToNodeTable ()
{

}

NodeList::~NodeList ()
{

}

bool NodeList::IsEmpty () const
{
	DBGASSERT (nodeIdList.empty () == nodeIdToNodeTable.empty ());
	return nodeIdList.empty ();
}

size_t NodeList::Count () const
{
	DBGASSERT (nodeIdList.size () == nodeIdToNodeTable.size ());
	return nodeIdList.size ();
}

bool NodeList::ContainsNode (const NodeId& nodeId) const
{
	return nodeIdToNodeTable.find (nodeId) != nodeIdToNodeTable.end ();
}

NE::NodeConstPtr NodeList::GetNode (const NodeId& nodeId) const
{
	auto foundNode = nodeIdToNodeTable.find (nodeId);
	if (DBGERROR (foundNode == nodeIdToNodeTable.end ())) {
		return nullptr;
	}
	return foundNode->second;
}

NE::NodePtr NodeList::GetNode (const NodeId& nodeId)
{
	auto foundNode = nodeIdToNodeTable.find (nodeId);
	if (DBGERROR (foundNode == nodeIdToNodeTable.end ())) {
		return nullptr;
	}
	return foundNode->second;
}

bool NodeList::AddNode (const NodeId& nodeId, const NodePtr& nodePtr)
{
	if (DBGERROR (nodeIdToNodeTable.find (nodeId) != nodeIdToNodeTable.end ())) {
		return false;
	}
	nodeIdList.push_back (nodeId);
	nodeIdToNodeTable.insert ({ nodeId, nodePtr });
	return true;
}

bool NodeList::DeleteNode (const NodeId& nodeId)
{
	if (DBGERROR (nodeIdToNodeTable.find (nodeId) == nodeIdToNodeTable.end ())) {
		return false;
	}
	auto foundInList = std::find (nodeIdList.begin (), nodeIdList.end (), nodeId);
	if (DBGERROR (foundInList == nodeIdList.end ())) {
		return false;
	}
	nodeIdList.erase (foundInList);
	nodeIdToNodeTable.erase (nodeId);
	return true;
}

void NodeList::Clear ()
{
	nodeIdList.clear ();
	nodeIdToNodeTable.clear ();
}

void NodeList::EnumerateNodes (const std::function<bool (const NodePtr&)>& processor)
{
	DBGASSERT (nodeIdList.size () == nodeIdToNodeTable.size ());
	for (const NodeId& nodeId : nodeIdList) {
		processor (nodeIdToNodeTable.at (nodeId));
	}
}

void NodeList::EnumerateNodes (const std::function<bool (const NodeConstPtr&)>& processor) const
{
	DBGASSERT (nodeIdList.size () == nodeIdToNodeTable.size ());
	for (const NodeId& nodeId : nodeIdList) {
		processor (nodeIdToNodeTable.at (nodeId));
	}
}

}
