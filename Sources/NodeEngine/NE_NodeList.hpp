#ifndef NE_NODELIST_HPP
#define NE_NODELIST_HPP

#include "NE_Node.hpp"
#include "NE_OrderedMap.hpp"

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

	NodePtr			GetNode (const NodeId& nodeId);
	NodeConstPtr	GetNode (const NodeId& nodeId) const;

	bool			AddNode (const NodeId& nodeId, const NodePtr& nodePtr);
	bool			DeleteNode (const NodeId& nodeId);
	void			MakeSorted ();
	void			Clear ();

	void			Enumerate (const std::function<bool (NodePtr)>& processor);
	void			Enumerate (const std::function<bool (NodeConstPtr)>& processor) const;

private:
	OrderedMap<NodeId, NodePtr>		nodes;
};

}

#endif
