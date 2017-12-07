#include "NodeValueCache.hpp"
#include "Debug.hpp"

namespace NE
{

NodeValueCache::NodeValueCache ()
{

}

NodeValueCache::~NodeValueCache ()
{

}

bool NodeValueCache::Add (const NodeId& id, const ValuePtr& value)
{
	if (DBGERROR (Contains (id))) {
		return false;
	}
	cache.insert ({ id, value });
	return true;
}

bool NodeValueCache::Remove (const NodeId& id)
{
	if (DBGERROR (!Contains (id))) {
		return false;
	}
	cache.erase (id);
	return true;
}

void NodeValueCache::Clear ()
{
	return cache.clear ();
}

bool NodeValueCache::Contains (const NodeId& id) const
{
	return cache.find (id) != cache.end ();
}

const ValuePtr& NodeValueCache::Get (const NodeId& id) const
{
	return cache.find (id)->second;
}

}
