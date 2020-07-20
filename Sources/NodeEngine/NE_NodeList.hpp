#ifndef NE_NODELIST_HPP
#define NE_NODELIST_HPP

#include <NE_Node.hpp>

namespace NE
{

class NodeList
{
public:
	NodeList ();
	~NodeList ();

	bool			IsEmpty () const;
	size_t			Count () const;

	bool			ContainsNode (const NodeId& nodeId) const;
	NodeConstPtr	GetNode (const NodeId& nodeId) const;

	NodePtr			GetNode (const NodeId& nodeId);
	bool			AddNode (const NodeId& nodeId, const NodePtr& nodePtr);
	bool			DeleteNode (const NodeId& nodeId);
	void			Clear ();

	void			EnumerateNodes (const std::function<bool (const NodePtr&)>& processor);
	void			EnumerateNodes (const std::function<bool (const NodeConstPtr&)>& processor) const;

private:
	std::vector<NodeId>						nodeIdList;
	std::unordered_map<NodeId, NodePtr>		nodeIdToNodeTable;
};

}

#endif
