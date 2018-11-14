#ifndef NE_NODEGROUPLSIT_HPP
#define NE_NODEGROUPLSIT_HPP

#include "NE_NodeGroup.hpp"

#include <memory>
#include <unordered_map>

namespace NE
{

class NodeGroupList
{
	SERIALIZABLE;

public:
	NodeGroupList ();
	~NodeGroupList ();

	void			Enumerate (const std::function<bool (const NodeGroupConstPtr&)>& processor) const;
	void			Enumerate (const std::function<bool (const NodeGroupPtr&)>& processor);

	bool			AddGroup (const NodeGroupPtr& group);
	void			DeleteGroup (const NodeGroupPtr& group);
	NodeGroupPtr	GetGroup (const NodeId& nodeId);
	void			RemoveNodeFromGroup (const NodeId& nodeId);
	void			Clear ();

	Stream::Status	Read (InputStream& inputStream);
	Stream::Status	Write (OutputStream& outputStream) const;

private:
	std::vector<NodeGroupPtr>					groups;
	std::unordered_map<NodeId, NodeGroupPtr>	nodeToGroup;	
};

}

#endif
