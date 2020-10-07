#ifndef NE_NODEMANAGER_HPP
#define NE_NODEMANAGER_HPP

#include "NE_NodeEngineTypes.hpp"
#include "NE_Serializable.hpp"
#include "NE_Node.hpp"
#include "NE_NodeId.hpp"
#include "NE_SlotId.hpp"
#include "NE_Value.hpp"
#include "NE_ConnectionInfo.hpp"
#include "NE_ConnectionManager.hpp"
#include "NE_NodeList.hpp"
#include "NE_NodeGroupList.hpp"
#include "NE_NodeValueCache.hpp"
#include "NE_UniqueIdGenerator.hpp"
#include <functional>

namespace NE
{

class NodeManager
{
	SERIALIZABLE;
	friend class NodeManagerMerge;

public:
	enum class UpdateMode
	{
		Automatic,
		Manual
	};

	NodeManager ();
	NodeManager (const NodeManager& src) = delete;
	NodeManager (NodeManager&& src) = delete;
	~NodeManager ();

	NodeManager&			operator= (const NodeManager& rhs) = delete;
	NodeManager&			operator= (NodeManager&& rhs) = delete;

	void					Clear ();
	bool					IsEmpty () const;
	size_t					GetNodeCount () const;
	size_t					GetConnectionCount () const;

	void					EnumerateNodes (const std::function<bool (const NodePtr&)>& processor);
	void					EnumerateNodes (const std::function<bool (const NodeConstPtr&)>& processor) const;

	bool					ContainsNode (const NodeId& id) const;
	NodeConstPtr			GetNode (const NodeId& id) const;

	NodePtr					GetNode (const NodeId& id);
	NodePtr					AddNode (const NodePtr& node);
	bool					DeleteNode (const NodeId& id);
	bool					DeleteNode (const NodePtr& node);

	bool					IsOutputSlotConnectedToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	bool					CanConnectMoreOutputSlotToInputSlot (const InputSlotConstPtr& inputSlot) const;
	bool					CanConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	
	bool					ConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);
	bool					DisconnectOutputSlotFromInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);
	bool					DisconnectAllInputSlotsFromOutputSlot (const OutputSlotConstPtr& outputSlot);
	bool					DisconnectAllOutputSlotsFromInputSlot (const InputSlotConstPtr& inputSlot);

	bool					HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const;
	bool					HasConnectedInputSlots (const OutputSlotConstPtr& outputSlot) const;
	size_t					GetConnectedOutputSlotCount (const InputSlotConstPtr& inputSlot) const;
	size_t					GetConnectedInputSlotCount (const OutputSlotConstPtr& outputSlot) const;
	void					EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const;
	void					EnumerateConnectedInputSlots (const OutputSlotConstPtr& outputSlot, const std::function<void (const InputSlotConstPtr&)>& processor) const;

	void					EvaluateAllNodes (EvaluationEnv& env) const;
	void					ForceEvaluateAllNodes (EvaluationEnv& env) const;
	void					InvalidateNodeValue (const NodeId& nodeId) const;
	void					InvalidateNodeValue (const NodeConstPtr& node) const;
	
	void					EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const;
	void					EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const;

	void					EnumerateDependentNodes (const NodePtr& node, const std::function<void (const NodePtr&)>& processor);
	void					EnumerateDependentNodesRecursive (const NodePtr& node, const std::function<void (const NodePtr&)>& processor);

	void					EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const;
	void					EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const;

	NodeGroupPtr			AddNodeGroup (const NodeGroupPtr& group);
	void					DeleteNodeGroup (const NodeGroupId& groupId);
	void					AddNodeToGroup (const NodeGroupId& groupId, const NodeId& nodeId);
	void					RemoveNodeFromGroup (const NodeId& nodeId);

	NodeGroupConstPtr		GetNodeGroup (const NodeId& nodeId) const;
	const NodeCollection&	GetGroupNodes (const NodeGroupId& groupId) const;

	void					EnumerateNodeGroups (const std::function<bool (const NodeGroupConstPtr&)>& processor) const;
	void					EnumerateNodeGroups (const std::function<bool (const NodeGroupPtr&)>& processor);
	void					DeleteAllNodeGroups ();

	bool					IsCalculationEnabled () const;
	UpdateMode				GetUpdateMode () const;
	void					SetUpdateMode (UpdateMode newUpdateMode);

	Stream::Status			Read (InputStream& inputStream);
	Stream::Status			Write (OutputStream& outputStream) const;

	static bool				Clone (const NodeManager& source, NodeManager& target);
	static bool				ReadFromBuffer (NodeManager& nodeManager, const std::vector<char>& buffer);
	static bool				WriteToBuffer (const NodeManager& nodeManager, std::vector<char>& buffer);

private:
	enum class IdHandlingPolicy
	{
		KeepOriginalId,
		GenerateNewId
	};

	NodePtr				AddNode (const NodePtr& node, const NodeEvaluatorSetter& setter);
	NodePtr				AddUninitializedNode (const NodePtr& node);
	NodePtr				AddInitializedNode (const NodePtr& node, IdHandlingPolicy idHandling);

	NodeGroupPtr		AddNodeGroup (const NodeGroupPtr& group, const NE::NodeGroupId& groupId);
	NodeGroupPtr		AddUninitializedNodeGroup (const NodeGroupPtr& group);
	NodeGroupPtr		AddInitializedNodeGroup (const NodeGroupPtr& group, IdHandlingPolicy idHandling);

	Stream::Status		ReadNodes (InputStream& inputStream);
	Stream::Status		WriteNodes (OutputStream& outputStream) const;

	Stream::Status		ReadGroups (InputStream& inputStream, const ObjectVersion& version);
	Stream::Status		WriteGroups (OutputStream& outputStream) const;

	UniqueIdGenerator						idGenerator;
	NodeList								nodeList;
	ConnectionManager						connectionManager;
	NodeGroupList							nodeGroupList;
	UpdateMode								updateMode;

	mutable NodeValueCache					nodeValueCache;
	mutable NodeEvaluatorConstPtr			nodeEvaluator;
	mutable bool							isForceCalculate;
};

}

#endif
