#ifndef NODEVALUECACHE_HPP
#define NODEVALUECACHE_HPP

#include "NE_NodeId.hpp"
#include "NE_Value.hpp"
#include <unordered_map>

namespace NE
{

class NodeValueCache
{
public:
	NodeValueCache ();
	~NodeValueCache ();

	bool				Add (const NodeId& id, const ValuePtr& value);
	bool				Remove (const NodeId& id);
	void				Clear ();
	
	bool				Contains (const NodeId& id) const;
	const ValuePtr&		Get (const NodeId& id) const;

private:
	std::unordered_map<NodeId, ValuePtr>	cache;
};

}

#endif
