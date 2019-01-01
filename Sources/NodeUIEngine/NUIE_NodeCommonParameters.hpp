#ifndef NUIE_NODECOMMONPARAMETERS_HPP
#define NUIE_NODECOMMONPARAMETERS_HPP

#include "NE_SingleValues.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeUIManager.hpp"

namespace NUIE
{

template <typename ValueType>
class TypedParameter : public NodeParameter
{
public:
	TypedParameter (const std::wstring& name, const ParameterType& type) :
		NodeParameter (name, type)
	{

	}

	virtual ~TypedParameter ()
	{

	}

	virtual NE::ValueConstPtr GetValue (const UINodeConstPtr& uiNode) const override
	{
		return GetValueInternal (uiNode);
	}

	virtual bool CanSetValue (const UINodeConstPtr& uiNode, const NE::ValueConstPtr& value) const override
	{
		if (!NE::Value::IsType<ValueType> (value)) {
			return false;
		}
		std::shared_ptr<const ValueType> typedValue = NE::Value::Cast<ValueType> (value);
		if (!IsValidValue (uiNode, typedValue)) {
			return false;
		}
		return true;
	}

	virtual bool SetValue (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, const NE::ValueConstPtr& value) override
	{
		if (DBGERROR (!CanSetValue (uiNode, value))) {
			return false;
		}
		return SetValueInternal (uiManager, evaluationEnv, uiNode, value);
	}

	virtual bool IsValidValue (const UINodeConstPtr&, const std::shared_ptr<const ValueType>&) const
	{
		return true;
	}

	virtual NE::ValueConstPtr	GetValueInternal (const UINodeConstPtr& uiNode) const = 0;
	virtual bool				SetValueInternal (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, const NE::ValueConstPtr& value) = 0;
};

template <typename NodeType, typename ValueType>
class TypedNodeParameter : public TypedParameter<ValueType>
{
public:
	TypedNodeParameter (const std::wstring& name, const ParameterType& type) :
		TypedParameter<ValueType> (name, type)
	{

	}

	virtual ~TypedNodeParameter ()
	{

	}

	virtual bool IsApplicableTo (const UINodeConstPtr& uiNode) const override
	{
		return NE::Node::IsTypeConst<NodeType> (uiNode);
	}

	std::shared_ptr<NodeType> GetTypedNode (const UINodePtr& uiNode)
	{
		return NE::Node::Cast<NodeType> (uiNode);
	}

	std::shared_ptr<const NodeType> GetTypedNode (const UINodeConstPtr& uiNode) const
	{
		return NE::Node::CastConst<NodeType> (uiNode);
	}
};

template <typename NodeType>
class NotEmptyStringNodeParameter : public TypedNodeParameter<NodeType, NE::StringValue>
{
public:
	NotEmptyStringNodeParameter (const std::wstring& name) :
		TypedNodeParameter<NodeType, NE::StringValue> (name, ParameterType::String)
	{

	}

	virtual bool IsValidValue (const UINodeConstPtr&, const std::shared_ptr<const NE::StringValue>& value) const override
	{
		return !value->GetValue ().empty ();
	}
};

template <typename NodeType>
class IntegerNodeParameter : public TypedNodeParameter<NodeType, NE::IntValue>
{
public:
	IntegerNodeParameter (const std::wstring& name) :
		TypedNodeParameter<NodeType, NE::IntValue> (name, ParameterType::Integer)
	{

	}
};

template <typename NodeType>
class PositiveIntegerNodeParameter : public TypedNodeParameter<NodeType, NE::IntValue>
{
public:
	PositiveIntegerNodeParameter (const std::wstring& name) :
		TypedNodeParameter<NodeType, NE::IntValue> (name, ParameterType::Integer)
	{

	}

	virtual bool IsValidValue (const UINodeConstPtr&, const std::shared_ptr<const NE::IntValue>& value) const override
	{
		return value->GetValue () > 0;
	}
};

template <typename NodeType>
class FloatNodeParameter : public TypedNodeParameter<NodeType, NE::FloatValue>
{
public:
	FloatNodeParameter (const std::wstring& name) :
		TypedNodeParameter<NodeType, NE::FloatValue> (name, ParameterType::Float)
	{

	}
};

template <typename NodeType>
class DoubleNodeParameter : public TypedNodeParameter<NodeType, NE::DoubleValue>
{
public:
	DoubleNodeParameter (const std::wstring& name) :
		TypedNodeParameter<NodeType, NE::DoubleValue> (name, ParameterType::Double)
	{

	}
};

template <typename NodeType>
class EnumerationNodeParameter : public TypedNodeParameter<NodeType, NE::IntValue>
{
public:
	EnumerationNodeParameter (const std::wstring& name, const std::vector<std::wstring>& valueChoices) :
		TypedNodeParameter<NodeType, NE::IntValue> (name, ParameterType::Enumeration),
		valueChoices (valueChoices)
	{

	}

	virtual bool IsValidValue (const UINodeConstPtr&, const std::shared_ptr<const NE::IntValue>& value) const override
	{
		int valueInt = value->GetValue ();
		return valueInt >= 0 && valueInt < (int) valueChoices.size ();
	}

	virtual std::vector<std::wstring> GetValueChoices () const override
	{
		return valueChoices;
	}

private:
	std::vector<std::wstring> valueChoices;
};

template <typename NodeType, typename ValueType>
class SlotDefaultValueNodeParameter : public TypedNodeParameter<NodeType, ValueType>
{
public:
	SlotDefaultValueNodeParameter (const std::wstring& name, const ParameterType& type, const NE::SlotId& slotId) :
		TypedNodeParameter<NodeType, ValueType> (name, type),
		slotId (slotId)
	{

	}

	virtual NE::ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
	{
		NE::InputSlotConstPtr inputSlot = uiNode->GetInputSlot (slotId);
		if (DBGERROR (inputSlot == nullptr)) {
			return nullptr;
		}
		return inputSlot->GetDefaultValue ();
	}

	virtual bool SetValueInternal (NodeUIManager& uiManager, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValueConstPtr& value) override
	{
		uiNode->GetInputSlot (slotId)->SetDefaultValue (value);
		uiManager.InvalidateNodeValue (uiNode->GetId ());
		uiManager.InvalidateNodeDrawing (uiNode->GetId ());
		return true;
	}

private:
	NE::SlotId slotId;
};

template <class NodeType, class ValueType>
void RegisterSlotDefaultValueNodeParameter (NodeParameterList& parameterList, const std::wstring& name, const ParameterType& type, const NE::SlotId& slotId)
{
	class Parameter : public SlotDefaultValueNodeParameter<NodeType, ValueType>
	{
	public:
		Parameter (const std::wstring& name, const ParameterType& type, const NE::SlotId& slotId) :
			SlotDefaultValueNodeParameter<NodeType, ValueType> (name, type, slotId)
		{

		}
	};

	parameterList.AddParameter (NodeParameterPtr (new Parameter (name, type, slotId)));
}

template <typename FeatureType, typename ValueType>
class TypedFeatureParameter : public TypedParameter<ValueType>
{
public:
	TypedFeatureParameter (const std::wstring& name, const ParameterType& type, const FeatureId& featureId) :
		TypedParameter<ValueType> (name, type),
		featureId (featureId)
	{

	}

	virtual ~TypedFeatureParameter ()
	{

	}

	virtual bool IsApplicableTo (const UINodeConstPtr& uiNode) const
	{
		return uiNode->HasFeature (featureId);
	}

private:
	FeatureId featureId;
};

template <typename FeatureType>
class EnumerationFeatureParameter : public TypedFeatureParameter<FeatureType, NE::IntValue>
{
public:
	EnumerationFeatureParameter (const std::wstring& name, const std::vector<std::wstring>& valueChoices, const FeatureId& featureId) :
		TypedFeatureParameter<FeatureType, NE::IntValue> (name, ParameterType::Enumeration, featureId),
		valueChoices (valueChoices)
	{

	}

	virtual bool IsValidValue (const UINodeConstPtr&, const std::shared_ptr<const NE::IntValue>& value) const override
	{
		int valueInt = value->GetValue ();
		return valueInt >= 0 && valueInt < (int) valueChoices.size ();
	}

	virtual std::vector<std::wstring> GetValueChoices () const override
	{
		return valueChoices;
	}

private:
	std::vector<std::wstring> valueChoices;
};

}

#endif
