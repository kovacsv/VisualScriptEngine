#ifndef NE_NODEGROUPLSIT_HPP
#define NE_NODEGROUPLSIT_HPP

#include "NE_NodeGroup.hpp"

#include <memory>
#include <unordered_map>

namespace NE
{

class NodeGroupList
{
public:
	NodeGroupList ();
	~NodeGroupList ();

	bool					IsEmpty () const;
	bool					Contains (const NodeGroupId& groupId) const;
	size_t					Count () const;

	NodeGroupPtr			GetGroup (const NodeGroupId& groupId);
	NodeGroupConstPtr		GetGroup (const NodeGroupId& groupId) const;

	void					Enumerate (const std::function<bool (const NodeGroupConstPtr&)>& processor) const;
	void					Enumerate (const std::function<bool (const NodeGroupPtr&)>& processor);

	bool					AddGroup (const NodeGroupPtr& group);
	void					DeleteGroup (const NodeGroupId& groupId);

	void					AddNodeToGroup (const NodeGroupId& groupId, const NodeId& nodeId);
	void					RemoveNodeFromGroup (const NodeId& nodeId);
	const NodeCollection&	GetGroupNodes (const NodeGroupId& groupId) const;
	NodeGroupId				GetNodeGroupId (const NodeId& nodeId) const;
	NodeGroupConstPtr		GetNodeGroup (const NodeId& nodeId) const;

	void					Clear ();

private:
	std::vector<NodeGroupId>							groupIdList;
	std::unordered_map<NodeGroupId, NodeGroupPtr>		groupIdToGroup;
	std::unordered_map<NodeGroupId, NodeCollection>		groupIdToNodes;
	std::unordered_map<NodeId, NodeGroupId>				nodeToGroup;
};

}

#endif
