#ifndef NODEMANAGER_HPP
#define NODEMANAGER_HPP

#include "NE_NodeEngineTypes.hpp"
#include "NE_Serializable.hpp"
#include "NE_Node.hpp"
#include "NE_NodeId.hpp"
#include "NE_SlotId.hpp"
#include "NE_Value.hpp"
#include "NE_ConnectionManager.hpp"
#include "NE_NodeValueCache.hpp"
#include <functional>

namespace NE
{

class NodeFilter
{
public:
	virtual bool NeedToProcessNode (const NodeId& nodeId) const = 0;
};

class AllNodesFilter : public NodeFilter
{
public:
	virtual bool NeedToProcessNode (const NodeId& nodeId) const override;
};

class NodeManager
{
	SERIALIZABLE;

public:
	NodeManager ();
	NodeManager (const NodeManager& src) = delete;
	~NodeManager ();

	void				Clear ();
	bool				IsEmpty () const;
	size_t				GetNodeCount () const;
	size_t				GetConnectionCount () const;

	void				EnumerateNodes (const std::function<bool (const NodePtr&)>& processor);
	void				EnumerateNodes (const std::function<bool (const NodeConstPtr&)>& processor) const;

	bool				ContainsNode (const NodeId& id) const;
	NodeConstPtr		GetNode (const NodeId& id) const;

	NodePtr				GetNode (const NodeId& id);
	NodePtr				AddNode (const NodePtr& node);
	bool				DeleteNode (const NodeId& id);
	bool				DeleteNode (const NodePtr& node);

	bool				IsOutputSlotConnectedToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	bool				CanConnectMoreOutputSlotToInputSlot (const InputSlotConstPtr& inputSlot) const;
	bool				CanConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	
	bool				ConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);
	bool				DisconnectOutputSlotFromInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);
	bool				DisconnectAllInputSlotsFromOutputSlot (const OutputSlotConstPtr& outputSlot);
	bool				DisconnectAllOutputSlotsFromInputSlot (const InputSlotConstPtr& inputSlot);

	void				EnumerateConnections (const std::function<void (const OutputSlotConstPtr&, const InputSlotConstPtr&)>& processor) const;
	void				EnumerateConnections (const std::function<void (const NodeConstPtr&, const OutputSlotConstPtr&, const NodeConstPtr&, const InputSlotConstPtr&)>& processor) const;
	void				EnumerateConnections (const std::function<void (const NodeId&, const SlotId&, const NodeId&, const SlotId&)>& processor) const;

	bool				HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const;
	bool				HasConnectedInputSlots (const OutputSlotConstPtr& outputSlot) const;
	size_t				GetConnectedOutputSlotCount (const InputSlotConstPtr& inputSlot) const;
	size_t				GetConnectedInputSlotCount (const OutputSlotConstPtr& outputSlot) const;
	void				EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const;
	void				EnumerateConnectedInputSlots (const OutputSlotConstPtr& outputSlot, const std::function<void (const InputSlotConstPtr&)>& processor) const;

	void				EvaluateAllNodes (EvaluationEnv& env) const;
	void				InvalidateNodeValue (const NodeId& nodeId) const;
	void				InvalidateNodeValue (const NodeConstPtr& node) const;
	
	void				EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const;
	void				EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const;

	void				EnumerateDependentNodes (const NodePtr& node, const std::function<void (const NodePtr&)>& processor);
	void				EnumerateDependentNodesRecursive (const NodePtr& node, const std::function<void (const NodePtr&)>& processor);

	void				EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const;
	void				EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const;

	bool				MergeTo (NodeManager& targetNodeManager, const NodeFilter& nodeFilter) const;

	Stream::Status		Read (InputStream& inputStream);
	Stream::Status		Write (OutputStream& outputStream) const;

private:
	enum class IdHandlingPolicy
	{
		KeepOriginalId,
		GenerateNewId
	};

	NodePtr						AddNode (const NodePtr& node, const NodeEvaluatorSetter& setter);

	Stream::Status				ReadNodes (InputStream& inputStream, IdHandlingPolicy idHandling);
	Stream::Status				WriteNodes (OutputStream& outputStream, const NodeFilter& nodeFilter) const;

	NodeIdGenerator							idGenerator;
	std::unordered_map<NodeId, NodePtr>		nodeIdToNodeTable;
	ConnectionManager						connectionManager;

	mutable NodeValueCache					nodeValueCache;
	mutable NodeEvaluatorConstPtr			nodeEvaluator;
};

}

#endif
