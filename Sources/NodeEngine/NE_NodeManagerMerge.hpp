#ifndef NE_NODEMANAGERMERGRE_HPP
#define NE_NODEMANAGERMERGRE_HPP

#include "NE_NodeManager.hpp"

namespace NE
{

class NodeFilter
{
public:
	NodeFilter ();
	virtual ~NodeFilter ();

	virtual bool NeedToProcessNode (const NodeId& nodeId) const = 0;
};

class AllNodesFilter : public NodeFilter
{
public:
	AllNodesFilter ();
	virtual ~AllNodesFilter ();

	virtual bool NeedToProcessNode (const NodeId&) const override;
};

class AppendEventHandler
{
public:
	AppendEventHandler ();
	virtual ~AppendEventHandler ();

	virtual void NodeAdded (const NodeId& nodeId) = 0;
};

class EmptyAppendEventHandler : public AppendEventHandler
{
public:
	EmptyAppendEventHandler ();
	virtual ~EmptyAppendEventHandler ();

	virtual void NodeAdded (const NodeId& nodeId) override;
};

class UpdateEventHandler
{
public:
	UpdateEventHandler ();
	virtual ~UpdateEventHandler ();

	virtual void BeforeNodeDelete (const NodeId& nodeId) = 0;
};

class EmptyUpdateEventHandler : public UpdateEventHandler
{
public:
	EmptyUpdateEventHandler ();
	virtual ~EmptyUpdateEventHandler ();

	virtual void BeforeNodeDelete (const NodeId& nodeId) override;
};

class NodeManagerMerge
{
public:
	static bool AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeCollection& nodeCollection, AppendEventHandler& eventHandler);
	static bool AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter, AppendEventHandler& eventHandler);
	static bool UpdateNodeManager (const NodeManager& source, NodeManager& target, UpdateEventHandler& eventHandler);
};

}

#endif
