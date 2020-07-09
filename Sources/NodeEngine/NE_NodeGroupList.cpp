#include "NE_NodeGroupList.hpp"
#include <algorithm>

namespace NE
{

SERIALIZATION_INFO (NodeGroupList, 1);

NodeGroupList::NodeGroupList ()
{

}

NodeGroupList::~NodeGroupList ()
{

}

void NodeGroupList::Enumerate (const std::function<bool (const NodeGroupConstPtr&)>& processor) const
{
	for (const NodeGroupPtr& group : groups) {
		if (!processor (group)) {
			break;
		}
	}
}

void NodeGroupList::Enumerate (const std::function<bool (const NodeGroupPtr&)>& processor)
{
	for (const NodeGroupPtr& group : groups) {
		if (!processor (group)) {
			break;
		}
	}
}

bool NodeGroupList::AddGroup (const NodeGroupPtr& group)
{
	groups.push_back (group);
	groupToNodes.insert ({ group, NodeCollection () });
	return true;
}

void NodeGroupList::DeleteGroup (const NodeGroupConstPtr& group)
{
	const NodeCollection& nodes = GetGroupNodes (group);
	nodes.Enumerate ([&] (const NodeId& nodeId) {
		nodeToGroup.erase (nodeId);
		return true;
	});
	auto foundInGroups = std::find (groups.begin (), groups.end (), group);
	groups.erase (foundInGroups);
	groupToNodes.erase (group);
}

void NodeGroupList::AddNodeToGroup (const NodeGroupPtr& group, const NodeId& nodeId)
{
	NodeGroupConstPtr existingGroup = GetNodeGroup (nodeId);
	if (existingGroup != nullptr && group == existingGroup) {
		return;
	}
	RemoveNodeFromGroup (nodeId);
	groupToNodes[group].Insert (nodeId);
	nodeToGroup.insert ({ nodeId, group });
}

const NodeCollection& NodeGroupList::GetGroupNodes (const NodeGroupConstPtr& group) const
{
	return groupToNodes.at (group);
}

void NodeGroupList::RemoveNodeFromGroup (const NodeId& nodeId)
{
	NodeGroupConstPtr group = GetNodeGroup (nodeId);
	if (group == nullptr) {
		return;
	}

	NodeCollection& nodes = groupToNodes[group];
	nodes.Erase (nodeId);
	nodeToGroup.erase (nodeId);
	if (nodes.IsEmpty ()) {
		DeleteGroup (group);
	}
}

NodeGroupConstPtr NodeGroupList::GetNodeGroup (const NodeId& nodeId) const
{
	auto found = nodeToGroup.find (nodeId);
	if (found == nodeToGroup.end ()) {
		return nullptr;
	}
	return found->second;
}

void NodeGroupList::Clear ()
{
	groups.clear ();
	groupToNodes.clear ();
	nodeToGroup.clear ();
}

Stream::Status NodeGroupList::Read (InputStream& inputStream)
{
	DBGASSERT (groups.empty ());
	ObjectHeader header (inputStream);
	size_t groupCount = 0;
	inputStream.Read (groupCount);
	for (size_t i = 0; i < groupCount; i++) {
		NodeGroupPtr group (ReadDynamicObject<NodeGroup> (inputStream));
		NodeCollection nodes;
		nodes.Read (inputStream);
		AddGroup (group);
		nodes.Enumerate ([&] (const NodeId& nodeId) {
			AddNodeToGroup (group, nodeId);
			return true;
		});
	}
	return inputStream.GetStatus ();
}

Stream::Status NodeGroupList::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (groups.size ());
	for (const NodeGroupPtr& group : groups) {
		WriteDynamicObject (outputStream, group.get ());
		const NodeCollection& nodes = groupToNodes.at (group);
		nodes.Write (outputStream);
	}
	return outputStream.GetStatus ();
}

}
