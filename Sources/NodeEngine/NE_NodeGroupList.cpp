#include "NE_NodeGroupList.hpp"
#include <algorithm>

namespace NE
{

NodeGroupList::NodeGroupList () :
	groupIdList (),
	groupIdToGroup (),
	groupIdToNodes (),
	nodeToGroup ()
{

}

NodeGroupList::~NodeGroupList ()
{

}

bool NodeGroupList::IsEmpty () const
{
	return groupIdList.empty ();
}

bool NodeGroupList::Contains (const NodeGroupId& groupId) const
{
	return groupIdToGroup.find (groupId) != groupIdToGroup.end ();
}

size_t NodeGroupList::Count () const
{
	return groupIdList.size ();
}

NE::NodeGroupPtr NodeGroupList::GetGroup (const NodeGroupId& groupId)
{
	auto found = groupIdToGroup.find (groupId);
	if (found == groupIdToGroup.end ()) {
		return nullptr;
	}
	return found->second;
}

NE::NodeGroupConstPtr NodeGroupList::GetGroup (const NodeGroupId& groupId) const
{
	auto found = groupIdToGroup.find (groupId);
	if (found == groupIdToGroup.end ()) {
		return nullptr;
	}
	return found->second;
}

void NodeGroupList::Enumerate (const std::function<bool (const NodeGroupConstPtr&)>& processor) const
{
	DBGASSERT (groupIdList.size () == groupIdToGroup.size ());
	for (const NodeGroupId& groupId : groupIdList) {
		if (!processor (groupIdToGroup.at (groupId))) {
			break;
		}
	}
}

void NodeGroupList::Enumerate (const std::function<bool (const NodeGroupPtr&)>& processor)
{
	DBGASSERT (groupIdList.size () == groupIdToGroup.size ());
	for (const NodeGroupId& groupId : groupIdList) {
		if (!processor (groupIdToGroup.at (groupId))) {
			break;
		}
	}
}

bool NodeGroupList::AddGroup (const NodeGroupPtr& group)
{
	const NodeGroupId& groupId = group->GetId ();

	if (DBGERROR (groupId == NullNodeGroupId)) {
		return false;
	}
	if (DBGERROR (Contains (groupId))) {
		return false;
	}

	groupIdList.push_back (group->GetId ());
	groupIdToGroup.insert ({ group->GetId (), group });
	groupIdToNodes.insert ({ group->GetId (), NodeCollection () });

	return true;
}

void NodeGroupList::DeleteGroup (const NodeGroupId& groupId)
{
	auto foundInList = std::find (groupIdList.begin (), groupIdList.end (), groupId);
	if (DBGERROR (foundInList == groupIdList.end ())) {
		return;
	}

	auto foundInGroups = groupIdToGroup.find (groupId);
	if (DBGERROR (foundInGroups == groupIdToGroup.end ())) {
		return;
	}

	const NodeCollection& nodes = GetGroupNodes (groupId);
	nodes.Enumerate ([&] (const NodeId& nodeId) {
		nodeToGroup.erase (nodeId);
		return true;
	});

	groupIdList.erase (foundInList);
	groupIdToGroup.erase (foundInGroups);
	groupIdToNodes.erase (groupId);
}

void NodeGroupList::AddNodeToGroup (const NodeGroupId& groupId, const NodeId& nodeId)
{
	if (DBGERROR (!Contains (groupId))) {
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
	groupIdList.clear ();
	groupIdToGroup.clear ();
	groupIdToNodes.clear ();
	nodeToGroup.clear ();
}

}
