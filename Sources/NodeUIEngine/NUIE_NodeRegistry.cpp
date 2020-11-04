#include "NUIE_NodeRegistry.hpp"

namespace NUIE
{

NodeRegistry::NodeData::NodeData (NodeRegistryId id, const std::wstring& name, const CreatorFunction& creator) :
	id (id),
	name (name),
	creator (creator)
{
}

NodeRegistry::GroupData::GroupData (const std::wstring& name) :
	name (name),
	nodes ()
{
}

void NodeRegistry::GroupData::AddNode (NodeRegistryId id)
{
	nodes.push_back (id);
}

const std::wstring& NodeRegistry::GroupData::GetName () const
{
	return name;
}

const std::vector<NodeRegistryId>& NodeRegistry::GroupData::GetNodes () const
{
	return nodes;
}

NodeRegistryId NodeRegistry::NodeData::GetNodeId () const
{
	return id;
}

const std::wstring& NodeRegistry::NodeData::GetNodeName  () const
{
	return name;
}

const CreatorFunction & NodeRegistry::NodeData::GetCreatorFunction () const
{
	return creator;
}

UINodePtr NodeRegistry::NodeData::CreateNode (const Point& position) const
{
	return creator (position);
}

NodeRegistry::NodeRegistry () :
	nextNodeId (0)
{

}

void NodeRegistry::RegisterNode (const std::wstring& group, const std::wstring& name, const CreatorFunction& creator)
{
	auto foundGroup = nameToGroupData.find (group);
	if (foundGroup == nameToGroupData.end ()) {
		GroupData groupData (group);
		nameToGroupData.insert ({ group, groupRegistry.size () });
		groupRegistry.push_back (groupData);
	}

	NodeRegistryId id = nextNodeId++;
	NodeData nodeData = {
		id,
		name,
		creator
	};

	idToNodeData.insert ({ id, nodeRegistry.size () });
	nameToNodeData.insert ({ name, nodeRegistry.size () });
	groupRegistry[nameToGroupData[group]].AddNode (nodeRegistry.size ());
	nodeRegistry.push_back (nodeData);
}

bool NodeRegistry::Contains (size_t id) const
{
	return GetNodeData (id) != nullptr;
}

bool NodeRegistry::Contains (const std::wstring& name) const
{
	return GetNodeData (name) != nullptr;
}

const NodeRegistry::NodeData* NodeRegistry::GetNodeData (size_t id) const
{
	auto found = idToNodeData.find (id);
	if (found == idToNodeData.end ()) {
		return nullptr;
	}
	return &nodeRegistry.at (found->second);
}

const NodeRegistry::NodeData* NodeRegistry::GetNodeData (const std::wstring& name) const
{
	auto found = nameToNodeData.find (name);
	if (found == nameToNodeData.end ()) {
		return nullptr;
	}
	return &nodeRegistry.at (found->second);
}

void NodeRegistry::EnumerateGroups (const std::function<void (const std::wstring&)>& processor) const
{
	for (const GroupData& group : groupRegistry) {
		processor (group.GetName ());
	}
}

void NodeRegistry::EnumerateGroupNodes (const std::wstring& groupName, const std::function<void (const NodeData&)>& processor) const
{
	auto found = nameToGroupData.find (groupName);
	if (found == nameToGroupData.end ()) {
		return;
	}
	const GroupData& groupData = groupRegistry[found->second];
	for (NodeRegistryId nodeId : groupData.GetNodes ()) {
		processor (nodeRegistry[nodeId]);
	}
}

}
