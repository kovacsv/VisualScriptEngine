#include "NE_NodeGroupList.hpp"
#include <algorithm>

namespace NE
{

NodeGroupList::NodeGroupList ()
{

}

NodeGroupList::~NodeGroupList ()
{

}

bool NodeGroupList::IsEmpty () const
{
	return groups.empty ();
}

bool NodeGroupList::Contains (const NodeGroupId& groupId) const
{
	return groups.find (groupId) != groups.end ();
}

size_t NodeGroupList::Count () const
{
	return groups.size ();
}

NE::NodeGroupPtr NodeGroupList::GetGroup (const NodeGroupId& groupId)
{
	auto found = groups.find (groupId);
	if (found == groups.end ()) {
		return nullptr;
	}
	return found->second;
}

NE::NodeGroupConstPtr NodeGroupList::GetGroup (const NodeGroupId& groupId) const
{
	auto found = groups.find (groupId);
	if (found == groups.end ()) {
		return nullptr;
	}
	return found->second;
}

void NodeGroupList::Enumerate (const std::function<bool (const NodeGroupConstPtr&)>& processor) const
{
	for (const auto& group : groups) {
		if (!processor (group.second)) {
			break;
		}
	}
}

void NodeGroupList::Enumerate (const std::function<bool (const NodeGroupPtr&)>& processor)
{
	for (const auto& group : groups) {
		if (!processor (group.second)) {
			break;
		}
	}
}

bool NodeGroupList::AddGroup (const NodeGroupPtr& group)
{
	if (DBGERROR (group->GetId () == NullNodeGroupId)) {
		return false;
	}
	groups.insert ({ group->GetId (), group });
	groupToNodes.insert ({ group->GetId (), NodeCollection () });
	return true;
}

void NodeGroupList::DeleteGroup (const NodeGroupId& groupId)
{
	auto foundInGroups = groups.find (groupId);
	if (DBGERROR (foundInGroups == groups.end ())) {
		return;
	}

	const NodeCollection& nodes = GetGroupNodes (groupId);
	nodes.Enumerate ([&] (const NodeId& nodeId) {
		nodeToGroup.erase (nodeId);
		return true;
	});

	groups.erase (foundInGroups);
	groupToNodes.erase (groupId);
}

void NodeGroupList::AddNodeToGroup (const NodeGroupId& groupId, const NodeId& nodeId)
{
	auto foundInGroups = groups.find (groupId);
	if (DBGERROR (foundInGroups == groups.end ())) {
		return;
	}

	NodeGroupId existingGroupId = GetNodeGroupId (nodeId);
	if (existingGroupId != NullNodeGroupId && groupId == existingGroupId) {
		return;
	}

	RemoveNodeFromGroup (nodeId);
	groupToNodes[groupId].Insert (nodeId);
	nodeToGroup.insert ({ nodeId, groupId });
}

const NodeCollection& NodeGroupList::GetGroupNodes (const NodeGroupId& groupId) const
{
	return groupToNodes.at (groupId);
}

void NodeGroupList::RemoveNodeFromGroup (const NodeId& nodeId)
{
	NodeGroupId existingGroupId = GetNodeGroupId (nodeId);
	if (existingGroupId == NullNodeGroupId) {
		return;
	}

	NodeCollection& nodes = groupToNodes[existingGroupId];
	nodes.Erase (nodeId);
	nodeToGroup.erase (nodeId);
	if (nodes.IsEmpty ()) {
		DeleteGroup (existingGroupId);
	}
}

NE::NodeGroupId NodeGroupList::GetNodeGroupId (const NodeId& nodeId) const
{
	auto found = nodeToGroup.find (nodeId);
	if (found == nodeToGroup.end ()) {
		return NullNodeGroupId;
	}
	return found->second;
}

NodeGroupConstPtr NodeGroupList::GetNodeGroup (const NodeId& nodeId) const
{
	auto found = nodeToGroup.find (nodeId);
	if (found == nodeToGroup.end ()) {
		return nullptr;
	}
	return GetGroup (found->second);
}

void NodeGroupList::Clear ()
{
	groups.clear ();
	groupToNodes.clear ();
	nodeToGroup.clear ();
}

}
