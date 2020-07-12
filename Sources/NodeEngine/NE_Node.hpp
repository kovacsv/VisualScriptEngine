#ifndef NE_NODE_HPP
#define NE_NODE_HPP

#include "NE_Serializable.hpp"
#include "NE_NodeId.hpp"
#include "NE_SlotId.hpp"
#include "NE_SlotList.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
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

	virtual void			InvalidateNodeValue (const NodeId& nodeId) const = 0;
	virtual bool			HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const = 0;
	virtual void			EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const = 0;

	virtual bool			IsCalculationEnabled () const = 0;
	virtual bool			HasCalculatedNodeValue (const NodeId& nodeId) const = 0;
	virtual ValueConstPtr	GetCalculatedNodeValue (const NodeId& nodeId) const = 0;
	virtual void			SetCalculatedNodeValue (const NodeId& nodeId, const ValueConstPtr& valuePtr) const = 0;
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
	friend class NodeManager;

public:
	enum class CalculationStatus
	{
		Calculated,
		NeedToCalculate,
		NeedToCalculateButDisabled
	};

	Node ();
	Node (const Node& src) = delete;
	virtual ~Node ();

	bool					IsEmpty () const;
	const NodeId&			GetId () const;

	bool					HasInputSlot (const SlotId& slotId) const;
	bool					HasOutputSlot (const SlotId& slotId) const;

	InputSlotConstPtr		GetInputSlot (const SlotId& slotId) const;
	OutputSlotConstPtr		GetOutputSlot (const SlotId& slotId) const;

	size_t					GetInputSlotCount () const;
	size_t					GetOutputSlotCount () const;

	void					EnumerateInputSlots (const std::function<bool (const InputSlotConstPtr&)>& processor) const;
	void					EnumerateOutputSlots (const std::function<bool (const OutputSlotConstPtr&)>& processor) const;

	ValueConstPtr			Evaluate (EvaluationEnv& env) const;
	ValueConstPtr			GetCalculatedValue () const;
	bool					HasCalculatedValue () const;
	CalculationStatus		GetCalculationStatus () const;
	void					InvalidateValue () const;

	ValueConstPtr			GetInputSlotDefaultValue (const SlotId& slotId) const;
	void					SetInputSlotDefaultValue (const SlotId& slotId, const ValueConstPtr& newDefaultValue);

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

	static NodePtr			Clone (const NodeConstPtr& node);
	static bool				IsEqual (const NodeConstPtr& aNode, const NodeConstPtr& bNode);

	template <class SlotConstType>
	void EnumerateSlots (const std::function<bool (const SlotConstType&)>& processor) const;

	template <class Type>
	static bool IsType (Node* node);

	template <class Type>
	static bool IsType (const NodePtr& node);

	template <class Type>
	static bool IsTypeConst (const NodeConstPtr& node);

	template <class Type>
	static Type* Cast (Node* node);

	template <class Type>
	static std::shared_ptr<Type> Cast (const NodePtr& node);

	template <class Type>
	static std::shared_ptr<const Type> CastConst (const NodeConstPtr& node);

protected:
	bool					RegisterInputSlot (const InputSlotPtr& newInputSlot);
	bool					RegisterOutputSlot (const OutputSlotPtr& newOutputSlot);
	ValueConstPtr			EvaluateInputSlot (const SlotId& slotId, EvaluationEnv& env) const;

	InputSlotPtr			GetModifiableInputSlot (const SlotId& slotId);
	OutputSlotPtr			GetModifiableOutputSlot (const SlotId& slotId);

private:
	void					SetNodeEvaluator (const NodeEvaluatorSetter& evaluatorSetter);
	bool					HasNodeEvaluator () const;
	void					ClearNodeEvaluator ();

	virtual void			Initialize () = 0;
	virtual ValueConstPtr	Calculate (EvaluationEnv& env) const = 0;

	virtual bool			IsForceCalculated () const;
	virtual void			ProcessCalculatedValue (const ValueConstPtr& value, EvaluationEnv& env) const;

	ValueConstPtr			EvaluateInputSlot (const InputSlotConstPtr& inputSlot, EvaluationEnv& env) const;

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
bool Node::IsTypeConst (const NodeConstPtr& node)
{
	return dynamic_cast<const Type*> (node.get ()) != nullptr;
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

template <class Type>
std::shared_ptr<const Type> Node::CastConst (const NodeConstPtr& node)
{
	return std::dynamic_pointer_cast<const Type> (node);
}

}

#endif
