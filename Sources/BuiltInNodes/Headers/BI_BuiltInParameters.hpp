#ifndef BUILTINPARAMETERS_HPP
#define BUILTINPARAMETERS_HPP

#include "UINodeParameters.hpp"
#include "NodeUIManager.hpp"

namespace NUIE
{

template <typename NodeType, typename ValueType>
class TypedNodeParameter : public NodeParameter
{
public:
	TypedNodeParameter (const std::string& paramId, const std::wstring& name, NodeParameter::Type type) :
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

	virtual bool CanSetValue (const UINodePtr&, const NE::ValuePtr& value) const
	{
		return NE::Value::IsType<ValueType> (value);
	}
};

template <typename NodeType, typename ValueType>
class SlotDefaultValueParameter : public NUIE::TypedNodeParameter<NodeType, ValueType>
{
public:
	SlotDefaultValueParameter (const std::string& paramId, const std::wstring& name, NUIE::NodeParameter::Type type, const NE::SlotId& slotId) :
		TypedNodeParameter<NodeType, ValueType> (paramId, name, type),
		slotId (slotId)
	{

	}

	virtual NE::ValuePtr GetValue (const NUIE::UINodePtr& uiNode) const override
	{
		NE::InputSlotConstPtr inputSlot = uiNode->GetInputSlot (slotId);
		if (DBGERROR (inputSlot == nullptr)) {
			return nullptr;
		}
		return inputSlot->GetDefaultValue ();
	}

	virtual bool SetValue (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
	{
		if (DBGERROR (!CanSetValue (uiNode, value))) {
			return false;
		}
		uiNode->GetInputSlot (slotId)->SetDefaultValue (value);
		uiNode->InvalidateValue ();
		uiManager.RequestRecalculate ();
		return true;
	}

private:
	NE::SlotId slotId;
};

class NodeNameParameter : public TypedNodeParameter<UINode, NE::StringValue>
{
public:
	NodeNameParameter () :
		TypedNodeParameter<UINode, NE::StringValue> ("NodeNameParameter", L"Name", NodeParameter::Type::String)
	{

	}

	virtual NE::ValuePtr GetValue (const UINodePtr& uiNode) const override
	{
		return NE::ValuePtr (new NE::StringValue (uiNode->GetNodeName ()));
	}

	virtual bool SetValue (NodeUIManager& uiManager, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValuePtr& value) override
	{
		if (DBGERROR (!CanSetValue (uiNode, value))) {
			return false;
		}
		uiNode->SetNodeName (NE::StringValue::Get (value));
		uiManager.InvalidateNodeDrawing (uiNode);
		uiManager.RequestRedraw ();
		return true;
	}
};

}

#endif
