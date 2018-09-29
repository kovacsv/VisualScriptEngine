#ifndef NODE_HPP
#define NODE_HPP

#include "NE_Serializable.hpp"
#include "NE_NodeId.hpp"
#include "NE_SlotId.hpp"
#include "NE_SlotList.hpp"
#include "NE_Value.hpp"
#include "NE_EvaluationEnv.hpp"
#include "NE_NodeValueCache.hpp"

#include <memory>
#include <functional>
#include <unordered_set>

namespace NE
{

class NodeEvaluator
{
public:
	NodeEvaluator ();
	virtual ~NodeEvaluator ();

	virtual void		InvalidateNodeValue (const NodeId& nodeId) const = 0;
	virtual bool		HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const = 0;
	virtual void		EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const = 0;

	virtual bool		IsNodeValueCalculated (const NodeId& nodeId) const = 0;
	virtual ValuePtr	GetCalculatedNodeValue (const NodeId& nodeId) const = 0;
	virtual void		SetCalculatedNodeValue (const NodeId& nodeId, const ValuePtr& valuePtr) const = 0;
};

enum class InitializationMode
{
	Initialize,
	DoNotInitialize
};

class NodeEvaluatorSetter
{
public:
	NodeEvaluatorSetter ();
	virtual ~NodeEvaluatorSetter ();

	virtual const NodeId&					GetNodeId () const = 0;
	virtual const NodeEvaluatorConstPtr&	GetNodeEvaluator () const = 0;
	virtual InitializationMode				GetInitializationMode () const = 0;
};

class Node : public DynamicSerializable
{
	SERIALIZABLE;

public:
	Node ();
	Node (const Node& src) = delete;
	virtual ~Node ();

	bool					IsEmpty () const;
	const NodeId&			GetId () const;

	bool					HasInputSlot (const SlotId& slotId) const;
	bool					HasOutputSlot (const SlotId& slotId) const;

	InputSlotPtr			GetInputSlot (const SlotId& slotId);
	OutputSlotPtr			GetOutputSlot (const SlotId& slotId);

	InputSlotConstPtr		GetInputSlot (const SlotId& slotId) const;
	OutputSlotConstPtr		GetOutputSlot (const SlotId& slotId) const;

	size_t					GetInputSlotCount () const;
	size_t					GetOutputSlotCount () const;

	void					EnumerateInputSlots (const std::function<bool (const InputSlotPtr&)>& processor);
	void					EnumerateOutputSlots (const std::function<bool (const OutputSlotPtr&)>& processor);

	void					EnumerateInputSlots (const std::function<bool (const InputSlotConstPtr&)>& processor) const;
	void					EnumerateOutputSlots (const std::function<bool (const OutputSlotConstPtr&)>& processor) const;

	ValuePtr				Evaluate (EvaluationEnv& env) const;
	ValuePtr				GetCalculatedValue () const;
	void					InvalidateValue () const;
	bool					ValueIsCalculated () const;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

	void					SetNodeEvaluator (const NodeEvaluatorSetter& evaluatorSetter);
	bool					HasNodeEvaluator () const;
	void					ClearNodeEvaluator ();

	static NodePtr			Clone (const NodeConstPtr& node);
	static bool				IsEqual (const NodeConstPtr& aNode, const NodeConstPtr& bNode);

	template <class SlotType>
	void EnumerateSlots (const std::function<bool (const SlotType&)>& processor);

	template <class SlotConstType>
	void EnumerateSlots (const std::function<bool (const SlotConstType&)>& processor) const;

	template <class Type>
	static bool IsType (Node* node);

	template <class Type>
	static bool IsType (const NodePtr& node);

	template <class Type>
	static Type* Cast (Node* node);

	template <class Type>
	static std::shared_ptr<Type> Cast (const NodePtr& node);

protected:
	virtual bool			RegisterInputSlot (const InputSlotPtr& newInputSlot);
	virtual bool			RegisterOutputSlot (const OutputSlotPtr& newOutputSlot);

	ValuePtr				EvaluateSingleInputSlot (const SlotId& slotId, EvaluationEnv& env) const;
	ListValuePtr			EvaluateInputSlot (const SlotId& slotId, EvaluationEnv& env) const;

private:
	virtual void			RegisterSlots () = 0;
	virtual ValuePtr		Calculate (EvaluationEnv& env) const = 0;
	virtual void			ProcessValue (const ValuePtr& value, EvaluationEnv& env) const;

	ListValuePtr			EvaluateInputSlot (const InputSlotConstPtr& inputSlot, EvaluationEnv& env) const;

	NodeId					nodeId;
	NodeEvaluatorConstPtr	nodeEvaluator;

	SlotList<InputSlot>		inputSlots;
	SlotList<OutputSlot>	outputSlots;
};

template <class Type>
bool Node::IsType (Node* node)
{
	return dynamic_cast<Type*> (node) != nullptr;
}

template <class Type>
bool Node::IsType (const NodePtr& node)
{
	return dynamic_cast<Type*> (node.get ()) != nullptr;
}

template <class Type>
Type* Node::Cast (Node* node)
{
	return dynamic_cast<Type*> (node);
}

template <class Type>
std::shared_ptr<Type> Node::Cast (const NodePtr& node)
{
	return std::dynamic_pointer_cast<Type> (node);
}

}

#endif
