#ifndef NODEMANAGER_HPP
#define NODEMANAGER_HPP

#include "NodeEngineTypes.hpp"
#include "Serializable.hpp"
#include "Node.hpp"
#include "NodeId.hpp"
#include "SlotId.hpp"
#include "Value.hpp"
#include "ConnectionManager.hpp"
#include "NodeValueCache.hpp"
#include <functional>

namespace NE
{

class NodeManager : public Serializable
{
	SERIALIZABLE;

public:
	NodeManager ();
	~NodeManager ();

	void						Clear ();
	bool						IsEmpty () const;
	size_t						GetNodeCount () const;

	void						EnumerateNodes (const std::function<bool (const NodePtr&)>& processor);
	void						EnumerateNodes (const std::function<bool (const NodeConstPtr&)>& processor) const;

	bool						ContainsNode (const NodeId& id) const;
	NodeConstPtr				GetNode (const NodeId& id) const;

	NodePtr						GetNode (const NodeId& id);
	NodePtr						AddNode (const NodePtr& node);
	bool						DeleteNode (const NodeId& id);
	bool						DeleteNode (const NodePtr& node);

	bool						IsOutputSlotConnectedToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	bool						CanConnectMoreOutputSlotToInputSlot (const InputSlotConstPtr& inputSlot) const;
	bool						CanConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	
	bool						ConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);
	bool						DisconnectOutputSlotFromInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);

	void						EnumerateConnections (const std::function<void (const OutputSlotConstPtr&, const InputSlotConstPtr&)>& processor) const;
	void						EnumerateConnections (const std::function<void (const NodeConstPtr&, const OutputSlotConstPtr&, const NodeConstPtr&, const InputSlotConstPtr&)>& processor) const;
	void						EnumerateConnections (const std::function<void (const NodeId&, const SlotId&, const NodeId&, const SlotId&)>& processor) const;

	bool						HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const;
	size_t						GetConnectedOutputSlotCount (const InputSlotConstPtr& inputSlot) const;
	void						EnumerateConnectedInputSlots (const OutputSlotConstPtr& outputSlot, const std::function<void (const InputSlotConstPtr&)>& processor) const;
	void						EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const;

	void						EvaluateAllNodes (EvaluationEnv& env) const;
	void						InvalidateNodeValue (const NodeId& nodeId) const;
	void						InvalidateNodeValue (const NodeConstPtr& node) const;
	
	void						EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const;
	void						EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const;

	void						EnumerateDependentNodes (const NodePtr& node, const std::function<void (const NodePtr&)>& processor);
	void						EnumerateDependentNodesRecursive (const NodePtr& node, const std::function<void (const NodePtr&)>& processor);

	void						EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const;
	void						EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const;

	virtual Stream::Status		Read (InputStream& inputStream) override;
	virtual Stream::Status		Write (OutputStream& outputStream) const override;

private:
	NodePtr						AddNode (const NodePtr& node, const NodeEvaluatorSetter& setter);

	NodeIdGenerator							idGenerator;
	std::unordered_map<NodeId, NodePtr>		nodeIdToNodeTable;
	ConnectionManager						connectionManager;

	mutable NodeValueCache					nodeValueCache;
	mutable NodeEvaluatorConstPtr			nodeEvaluator;
};

}

#endif
