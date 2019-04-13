#ifndef NUIE_NODEREGISTRY_HPP
#define NUIE_NODEREGISTRY_HPP

#include "NUIE_UINode.hpp"

namespace NUIE
{

using NodeRegistryId = size_t;
using CreatorFunction = std::function<NUIE::UINodePtr (const NUIE::Point& position)>;

class NodeRegistry
{
public:
	class NodeData
	{
	public:
		NodeData (NodeRegistryId id, const std::wstring& name, const CreatorFunction& creator);

		NodeRegistryId			GetNodeId () const;
		const std::wstring&		GetNodeName  () const;
		const CreatorFunction&	GetCreatorFunction () const;
		NUIE::UINodePtr			CreateNode (const NUIE::Point& position) const;

	private:
		NodeRegistryId		id;
		std::wstring		name;
		CreatorFunction		creator;
	};

	class GroupData
	{
	public:
		GroupData (const std::wstring& name);

		void								AddNode (NodeRegistryId id);
		const std::wstring&					GetName () const;
		const std::vector<NodeRegistryId>&	GetNodes () const;

	private:
		std::wstring					name;
		std::vector<NodeRegistryId>		nodes;
	};

	NodeRegistry ();

	void				RegisterNode (const std::wstring& group, const std::wstring& name, const CreatorFunction& creator);

	bool				Contains (size_t id) const;
	bool				Contains (const std::wstring& name) const;

	const NodeData*		GetNodeData (size_t id) const;
	const NodeData*		GetNodeData (const std::wstring& name) const;

	void				EnumerateGroups (const std::function<void (const std::wstring&)>& processor) const;
	void				EnumerateGroupNodes (const std::wstring& groupName, const std::function<void (const NodeData&)>& processor) const;

private:
	std::vector<GroupData>						groupRegistry;
	std::vector<NodeData>						nodeRegistry;
	std::unordered_map<NodeRegistryId, size_t>	idToNodeData;
	std::unordered_map<std::wstring, size_t>	nameToNodeData;
	std::unordered_map<std::wstring, size_t>	nameToGroupData;
	NodeRegistryId								nextNodeId;
};

}

#endif
