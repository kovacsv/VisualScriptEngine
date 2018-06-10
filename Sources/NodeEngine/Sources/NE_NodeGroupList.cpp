#include "NE_NodeGroupList.hpp"
#include <algorithm>

namespace NE
{

SerializationInfo NodeGroupList::serializationInfo (ObjectVersion (1));

NodeGroupList::NodeGroupList ()
{

}

NodeGroupList::~NodeGroupList ()
{

}

void NodeGroupList::Enumerate (const std::function<bool (const NodeGroupPtr&)>& processor) const
{
	for (const NodeGroupPtr& group : groups) {
		if (!processor (group)) {
			break;
		}
	}
}

bool NodeGroupList::AddGroup (const NodeGroupPtr& group)
{
	NodeCollection nodes = group->GetNodes ();
	nodes.Enumerate ([&] (const NodeId& nodeId) {
		RemoveNodeFromGroup (nodeId);
		nodeToGroup.insert ({ nodeId, group });
		return true;
	});

	groups.push_back (group);
	return true;
}

void NodeGroupList::DeleteGroup (const NodeGroupPtr& group)
{
	NodeCollection nodes = group->GetNodes ();
	nodes.Enumerate ([&] (const NodeId& nodeId) {
		RemoveNodeFromGroup (nodeId);
		return true;
	});
}

NodeGroupPtr NodeGroupList::GetGroup (const NodeId& nodeId)
{
	auto found = nodeToGroup.find (nodeId);
	if (found == nodeToGroup.end ()) {
		return nullptr;
	}
	return found->second;
}

void NodeGroupList::RemoveNodeFromGroup (const NodeId& nodeId)
{
	NodeGroupPtr group = GetGroup (nodeId);
	if (group == nullptr) {
		return;
	}
	group->DeleteNode (nodeId);
	if (group->IsEmpty ()) {
		auto foundInGroups = std::find (groups.begin (), groups.end (), group);
		DBGASSERT (foundInGroups != groups.end ());
		groups.erase (foundInGroups);
	}
	nodeToGroup.erase (nodeId);
}

void NodeGroupList::Clear ()
{
	groups.clear ();
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
		const NodeCollection& nodes = group->GetNodes ();
		groups.push_back (group);
		nodes.Enumerate ([&] (const NodeId& nodeId) {
			nodeToGroup.insert ({ nodeId, group });
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
	}
	return outputStream.GetStatus ();
}

}
