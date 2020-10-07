#include "NE_NodeGroupList.hpp"
#include <algorithm>

namespace NE
{

NodeGroupList::NodeGroupList () :
	groups (),
	groupIdToNodes (),
	nodeToGroup ()
{

}

NodeGroupList::~NodeGroupList ()
{

}

bool NodeGroupList::IsEmpty () const
{
	return groups.IsEmpty ();
}

bool NodeGroupList::Contains (const NodeGroupId& groupId) const
{
	return groups.Contains (groupId);
}

size_t NodeGroupList::Count () const
{
	return groups.Count ();
}

NE::NodeGroupPtr NodeGroupList::GetGroup (const NodeGroupId& groupId)
{
	if (DBGERROR (!groups.Contains (groupId))) {
		return nullptr;
	}
	return groups.GetValue (groupId);
}

NE::NodeGroupConstPtr NodeGroupList::GetGroup (const NodeGroupId& groupId) const
{
	if (DBGERROR (!groups.Contains (groupId))) {
		return nullptr;
	}
	return groups.GetValue (groupId);
}

bool NodeGroupList::AddGroup (const NodeGroupPtr& group)
{
	const NodeGroupId& groupId = group->GetId ();
	if (DBGERROR (groupId == NullNodeGroupId)) {
		return false;
	}

	if (DBGERROR (groups.Contains (groupId))) {
		return false;
	}

	groups.Insert (groupId, group);
	groupIdToNodes.insert ({ groupId, NodeCollection () });

	return true;
}

void NodeGroupList::DeleteGroup (const NodeGroupId& groupId)
{
	if (DBGERROR (!groups.Contains (groupId))) {
		return;
	}

	const NodeCollection& nodes = GetGroupNodes (groupId);
	nodes.Enumerate ([&] (const NodeId& nodeId) {
		nodeToGroup.erase (nodeId);
		return true;
	});

	groups.Erase (groupId);
	groupIdToNodes.erase (groupId);
}

void NodeGroupList::AddNodeToGroup (const NodeGroupId& groupId, const NodeId& nodeId)
{
	if (DBGERROR (!groups.Contains (groupId))) {
		return;
	}

	NodeGroupId existingGroupId = GetNodeGroupId (nodeId);
	if (existingGroupId != NullNodeGroupId && groupId == existingGroupId) {
		return;
	}

	RemoveNodeFromGroup (nodeId);
	groupIdToNodes[groupId].Insert (nodeId);
	nodeToGroup.insert ({ nodeId, groupId });
}

const NodeCollection& NodeGroupList::GetGroupNodes (const NodeGroupId& groupId) const
{
	return groupIdToNodes.at (groupId);
}

void NodeGroupList::RemoveNodeFromGroup (const NodeId& nodeId)
{
	NodeGroupId existingGroupId = GetNodeGroupId (nodeId);
	if (existingGroupId == NullNodeGroupId) {
		return;
	}

	NodeCollection& nodes = groupIdToNodes[existingGroupId];
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
	groups.Clear ();
	groupIdToNodes.clear ();
	nodeToGroup.clear ();
}

void NodeGroupList::Enumerate (const std::function<bool (const NodeGroupConstPtr&)>& processor) const
{
	groups.Enumerate ([&] (const NodeGroupPtr& group) {
		return processor (group);
	});
}

void NodeGroupList::Enumerate (const std::function<bool (const NodeGroupPtr&)>& processor)
{
	groups.Enumerate ([&] (const NodeGroupPtr& group) {
		return processor (group);
	});
}

}
