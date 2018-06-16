#ifndef NODEMANAGERMERGRE_HPP
#define NODEMANAGERMERGRE_HPP

#include "NE_NodeManager.hpp"

namespace NE
{

class MergeEventHandler
{
public:
	MergeEventHandler ();
	virtual ~MergeEventHandler ();

	virtual void BeforeDelete (const NE::NodeId& nodeId) = 0;
};

class NodeFilter
{
public:
	NodeFilter ();
	virtual ~NodeFilter ();

	virtual bool NeedToProcessNode (const NodeId& nodeId) const = 0;
};

class NodeManagerMerge
{
public:
	static bool AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter);
	static bool UpdateNodeManager (const NodeManager& source, NodeManager& target, MergeEventHandler& eventHandler);
};


}

#endif
