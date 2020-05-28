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

	virtual bool NeedToProcessSourceNode (const NodeId& nodeId) const = 0;
};

class AllNodesFilter : public NodeFilter
{
public:
	AllNodesFilter ();
	virtual ~AllNodesFilter ();

	virtual bool NeedToProcessSourceNode (const NodeId&) const override;
};

class NodeCollectionFilter : public NodeFilter
{
public:
	NodeCollectionFilter (const NodeCollection& nodeCollection);
	~NodeCollectionFilter ();

	virtual bool NeedToProcessSourceNode (const NodeId& nodeId) const override;

private:
	const NodeCollection& nodeCollection;
};

class AppendEventHandler
{
public:
	AppendEventHandler ();
	virtual ~AppendEventHandler ();

	virtual void TargetNodeAdded (const NodeId& nodeId) = 0;
};

class EmptyAppendEventHandler : public AppendEventHandler
{
public:
	EmptyAppendEventHandler ();
	virtual ~EmptyAppendEventHandler ();

	virtual void TargetNodeAdded (const NodeId& nodeId) override;
};

class NodeCollectorAppendEventHandler : public AppendEventHandler
{
public:
	NodeCollectorAppendEventHandler ();
	virtual ~NodeCollectorAppendEventHandler ();

	virtual void				TargetNodeAdded (const NodeId& nodeId) override;
	const NE::NodeCollection&	GetAddedTargetNodes () const;

private:
	NE::NodeCollection targetNodes;
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
	static bool AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter, AppendEventHandler& eventHandler);
	static bool UpdateNodeManager (const NodeManager& source, NodeManager& target, UpdateEventHandler& eventHandler);
};

}

#endif
