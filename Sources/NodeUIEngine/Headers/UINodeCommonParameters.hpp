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

template <typename NodeType, typename ValueType>
class SlotDefaultValueParameter : public TypedNodeParameter<NodeType, ValueType>
{
public:
	SlotDefaultValueParameter (const std::string& paramId, const std::wstring& name, ParameterType type, const NE::SlotId& slotId) :
		TypedNodeParameter<NodeType, ValueType> (paramId, name, type),
		slotId (slotId)
	{

	}

	virtual NE::ValuePtr GetValue (const UINodePtr& uiNode) const override
	{
		NE::InputSlotConstPtr inputSlot = uiNode->GetInputSlot (slotId);
		if (DBGERROR (inputSlot == nullptr)) {
			return nullptr;
		}
		return inputSlot->GetDefaultValue ();
	}

	virtual bool SetValue (NodeUIManager& uiManager, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValuePtr& value) override
	{
		bool canSetValue = TypedNodeParameter<NodeType, ValueType>::CanSetValue (uiNode, value);
		if (DBGERROR (!canSetValue)) {
			return false;
		}
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
