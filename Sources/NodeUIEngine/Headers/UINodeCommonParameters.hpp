#ifndef UINODECOMMONPARAMETERS_HPP
#define UINODECOMMONPARAMETERS_HPP

#include "UINodeParameters.hpp"
#include "NodeUIManager.hpp"
#include "SingleValues.hpp"

namespace NUIE
{

template <typename NodeType, typename ValueType>
class TypedNodeParameter : public NodeParameter
{
public:
	TypedNodeParameter (const std::string& paramId, const std::wstring& name, ParameterType type) :
		NodeParameter (paramId, name, type)
	{

	}

	virtual ~TypedNodeParameter ()
	{

	}

	virtual NE::ValuePtr GetValue (const UINodePtr& uiNode) const override
	{
		std::shared_ptr<NodeType> typedNode = NE::Node::Cast<NodeType> (uiNode);
		return GetValueInternal (typedNode);
	}

	virtual bool IsApplicableTo (const UINodePtr& uiNode) const
	{
		return NE::Node::IsType<NodeType> (uiNode);
	}

	virtual bool CanSetValue (const UINodePtr& uiNode, const NE::ValuePtr& value) const
	{
		if (!NE::Value::IsType<ValueType> (value)) {
			return false;
		}
		std::shared_ptr<ValueType> typedValue = NE::Value::Cast<ValueType> (value);
		if (!IsValidValue (uiNode, typedValue)) {
			return false;
		}
		return true;
	}

	virtual bool IsValidValue (const UINodePtr&, const std::shared_ptr<ValueType>&) const
	{
		return true;
	}

	virtual bool SetValue (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, const NE::ValuePtr& value) override
	{
		if (DBGERROR (!CanSetValue (uiNode, value))) {
			return false;
		}
		std::shared_ptr<NodeType> typedNode = NE::Node::Cast<NodeType> (uiNode);
		return SetValueInternal (uiManager, evaluationEnv, typedNode, value);
	}

	virtual NE::ValuePtr GetValueInternal (const std::shared_ptr<NodeType>& uiNode) const = 0;
	virtual bool SetValueInternal (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, std::shared_ptr<NodeType>& uiNode, const NE::ValuePtr& value) = 0;
};

template <typename NodeType>
class NotEmptyStringParameter : public TypedNodeParameter<NodeType, NE::StringValue>
{
public:
	NotEmptyStringParameter (const std::string& paramId, const std::wstring& name) :
		TypedNodeParameter<NodeType, NE::StringValue> (paramId, name, ParameterType::String)
	{

	}

	virtual bool IsValidValue (const UINodePtr&, const std::shared_ptr<NE::StringValue>& value) const override
	{
		return !value->GetValue ().empty ();
	}
};

template <typename NodeType>
class IntegerParameter : public TypedNodeParameter<NodeType, NE::IntValue>
{
public:
	IntegerParameter (const std::string& paramId, const std::wstring& name) :
		TypedNodeParameter<NodeType, NE::IntValue> (paramId, name, ParameterType::Integer)
	{

	}
};

template <typename NodeType>
class PositiveIntegerParameter : public TypedNodeParameter<NodeType, NE::IntValue>
{
public:
	PositiveIntegerParameter (const std::string& paramId, const std::wstring& name) :
		TypedNodeParameter<NodeType, NE::IntValue> (paramId, name, ParameterType::Integer)
	{

	}

	virtual bool IsValidValue (const UINodePtr&, const std::shared_ptr<NE::IntValue>& value) const
	{
		return value->GetValue () > 0;
	}
};

template <typename NodeType>
class DoubleParameter : public TypedNodeParameter<NodeType, NE::DoubleValue>
{
public:
	DoubleParameter (const std::string& paramId, const std::wstring& name) :
		TypedNodeParameter<NodeType, NE::DoubleValue> (paramId, name, ParameterType::Double)
	{

	}
};

template <typename NodeType, typename ValueType>
class SlotDefaultValueParameter : public TypedNodeParameter<NodeType, ValueType>
{
public:
	SlotDefaultValueParameter (const std::string& paramId, const std::wstring& name, ParameterType type, const NE::SlotId& slotId) :
		TypedNodeParameter<NodeType, ValueType> (paramId, name, type),
		slotId (slotId)
	{

	}

	virtual NE::ValuePtr GetValueInternal (const std::shared_ptr<NodeType>& uiNode) const override
	{
		NE::InputSlotConstPtr inputSlot = uiNode->GetInputSlot (slotId);
		if (DBGERROR (inputSlot == nullptr)) {
			return nullptr;
		}
		return inputSlot->GetDefaultValue ();
	}

	virtual bool SetValueInternal (NodeUIManager& uiManager, NE::EvaluationEnv&, std::shared_ptr<NodeType>& uiNode, const NE::ValuePtr& value) override
	{
		uiNode->GetInputSlot (slotId)->SetDefaultValue (value);
		uiManager.InvalidateNodeValue (uiNode->GetId ());
		uiManager.InvalidateNodeDrawing (uiNode->GetId ());
		return true;
	}

private:
	NE::SlotId slotId;
};

}

#endif
