#include "InputUINodes.hpp"
#include "UINodePanels.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"
#include "NodeUIManager.hpp"
#include "SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

NE::SerializationInfo			NumericUpDownUINode::serializationInfo (NE::ObjectId ("{F888C04D-FF22-4225-AC9A-90464D01ACF9}"), NE::ObjectVersion (1));
NE::DynamicSerializationInfo	IntegerUpDownUINode::serializationInfo (NE::ObjectId ("{53207AEE-8865-4DC0-AA38-B9988E39EDFD}"), NE::ObjectVersion (1), IntegerUpDownUINode::CreateSerializableInstance);
NE::DynamicSerializationInfo	IntegerRangeNode::serializationInfo (NE::ObjectId ("{B697B7DE-7AB9-479D-8DBE-8D3CCB6E4F50}"), NE::ObjectVersion (1), IntegerRangeNode::CreateSerializableInstance);

NumericUpDownUINode::NumericUpDownUINode () :
	UINode ()
{

}

NumericUpDownUINode::NumericUpDownUINode (const std::wstring& name, const Point& position) :
	UINode (name, position)
{

}

NumericUpDownUINode::~NumericUpDownUINode ()
{

}

EventHandlerResult NumericUpDownUINode::HandleMouseClick (NodeUIEnvironment& env, const KeySet&, MouseButton mouseButton, const Point& position)
{
	if (mouseButton != MouseButton::Left) {
		return EventHandlerResult::EventNotHandled;
	}

	Rect minusButtonRect = GetSpecialRect (env, "minus");
	Rect plusButtonRect = GetSpecialRect (env, "plus");

	if (minusButtonRect.Contains (position)) {
		Decrease ();
		return EventHandlerResult::EventHandled;
	} else if (plusButtonRect.Contains (position)) {
		Increase ();
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

void NumericUpDownUINode::UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const
{
	std::wstring nodeText = L"<empty>";
	DBGASSERT (ValueIsCalculated ());
	NE::ValuePtr nodeValue = GetCalculatedValue ();
	if (nodeValue != nullptr) {
		nodeText = nodeValue->ToString ();
	}

	NodeUIPanelDrawer drawer;
	drawer.AddPanel (NodeUIPanelPtr (new NodeUIHeaderPanel (GetNodeName ())));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUISlotPanel (*this, env)));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUILeftRightButtonsPanel ("minus", L"<", "plus", L">", nodeText, env)));
	drawer.Draw (env, drawingImage);
}

void NumericUpDownUINode::CalculationPostProcess (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

NE::Stream::Status NumericUpDownUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumericUpDownUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	return outputStream.GetStatus ();
}

IntegerUpDownUINode::IntegerUpDownUINode () :
	NumericUpDownUINode (),
	val (0),
	step (0)
{

}

IntegerUpDownUINode::IntegerUpDownUINode (const std::wstring& name, const Point& position, int val, int step) :
	NumericUpDownUINode (name, position),
	val (val),
	step (step)
{

}

void IntegerUpDownUINode::RegisterSlots ()
{
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"Output")));
}

NE::ValuePtr IntegerUpDownUINode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValuePtr (new NE::IntValue (val));
}

void IntegerUpDownUINode::RegisterParameters (NodeParameterList& parameterList) const
{
	class ValueParameter : public IntegerParameter<IntegerUpDownUINode>
	{
	public:
		ValueParameter () :
			IntegerParameter<IntegerUpDownUINode> ("IntegerUpDownValueParameter", L"Value")
		{

		}

		virtual NE::ValuePtr GetValue (const UINodePtr& uiNode) const override
		{
			std::shared_ptr<IntegerUpDownUINode> upDownNode = NE::Node::Cast<IntegerUpDownUINode> (uiNode);
			return NE::ValuePtr (new NE::IntValue (upDownNode->GetValue ()));
		}

		virtual bool SetValue (NodeUIManager& uiManager, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			if (DBGERROR (!CanSetValue (uiNode, value))) {
				return false;
			}
			std::shared_ptr<IntegerUpDownUINode> upDownNode = NE::Node::Cast<IntegerUpDownUINode> (uiNode);
			upDownNode->SetValue (NE::IntValue::Get (value));
			uiManager.InvalidateNodeValue (upDownNode);
			uiManager.InvalidateNodeDrawing (upDownNode);
			return true;
		}
	};

	class StepParameter : public IntegerParameter<IntegerUpDownUINode>
	{
	public:
		StepParameter () :
			IntegerParameter<IntegerUpDownUINode> ("IntegerUpDownStepParameter", L"Step")
		{

		}

		virtual NE::ValuePtr GetValue (const UINodePtr& uiNode) const override
		{
			std::shared_ptr<IntegerUpDownUINode> upDownNode = NE::Node::Cast<IntegerUpDownUINode> (uiNode);
			return NE::ValuePtr (new NE::IntValue (upDownNode->GetStep ()));
		}

		virtual bool SetValue (NodeUIManager&, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			if (DBGERROR (!CanSetValue (uiNode, value))) {
				return false;
			}
			std::shared_ptr<IntegerUpDownUINode> upDownNode = NE::Node::Cast<IntegerUpDownUINode> (uiNode);
			upDownNode->SetStep (NE::IntValue::Get (value));
			return true;
		}
	};

	NumericUpDownUINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NodeParameterPtr (new ValueParameter ()));
	parameterList.AddParameter (NodeParameterPtr (new StepParameter ()));
}

void IntegerUpDownUINode::Increase ()
{
	val = val + step;
	InvalidateValue ();
}

void IntegerUpDownUINode::Decrease ()
{
	val = val - step;
	InvalidateValue ();
}

NE::Stream::Status IntegerUpDownUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericUpDownUINode::Read (inputStream);
	inputStream.Read (val);
	inputStream.Read (step);
	return inputStream.GetStatus ();
}

NE::Stream::Status IntegerUpDownUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericUpDownUINode::Write (outputStream);
	outputStream.Write (val);
	outputStream.Write (step);
	return outputStream.GetStatus ();
}

int IntegerUpDownUINode::GetValue () const
{
	return val;
}

void IntegerUpDownUINode::SetValue (int newValue)
{
	val = newValue;
	InvalidateValue ();
}

int IntegerUpDownUINode::GetStep () const
{
	return step;
}

void IntegerUpDownUINode::SetStep (int newStep)
{
	step = newStep;
}

IntegerRangeNode::IntegerRangeNode () :
	IntegerRangeNode (L"", Point ())
{

}

IntegerRangeNode::IntegerRangeNode (const std::wstring& name, const Point& position) :
	HeaderWithSlotsUINode (name, position)
{

}

IntegerRangeNode::~IntegerRangeNode ()
{

}

void IntegerRangeNode::RegisterSlots ()
{
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("step"), L"Step", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValuePtr IntegerRangeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr start = EvaluateSingleInputSlot (NE::SlotId ("start"), env);
	NE::ValuePtr step = EvaluateSingleInputSlot (NE::SlotId ("step"), env);
	NE::ValuePtr count = EvaluateSingleInputSlot (NE::SlotId ("count"), env);
	if (!NE::Value::IsType<NE::IntValue> (start) || !NE::Value::IsType<NE::IntValue> (step) || !NE::Value::IsType<NE::IntValue> (count)) {
		return nullptr;
	}

	int startInt = NE::IntValue::Get (start);
	int stepInt = NE::IntValue::Get (step);
	int countInt = NE::IntValue::Get (count);
	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countInt; ++i) {
		list->Push (NE::ValuePtr (new NE::IntValue (startInt + i * stepInt)));
	}

	return list;
}

void IntegerRangeNode::RegisterParameters (NodeParameterList& parameterList) const
{
	class StartParameter : public SlotDefaultValueParameter<IntegerRangeNode, NE::IntValue>
	{
	public:
		StartParameter () :
			SlotDefaultValueParameter<IntegerRangeNode, NE::IntValue> ("IntegerRangeNodeStartParameter", L"Start", ParameterType::Integer, NE::SlotId ("start"))
		{

		}
	};

	class StepParameter : public SlotDefaultValueParameter<IntegerRangeNode, NE::IntValue>
	{
	public:
		StepParameter () :
			SlotDefaultValueParameter<IntegerRangeNode, NE::IntValue> ("IntegerRangeNodeStepParameter", L"Step", ParameterType::Integer, NE::SlotId ("step"))
		{

		}
	};

	class CountParameter : public SlotDefaultValueParameter<IntegerRangeNode, NE::IntValue>
	{
	public:
		CountParameter () :
			SlotDefaultValueParameter<IntegerRangeNode, NE::IntValue> ("IntegerRangeNodeCountParameter", L"Count", ParameterType::Integer, NE::SlotId ("count"))
		{

		}

		virtual bool IsValidValue (const UINodePtr&, const std::shared_ptr<NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0;
		}
	};

	UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NodeParameterPtr (new StartParameter ()));
	parameterList.AddParameter (NodeParameterPtr (new StepParameter ()));
	parameterList.AddParameter (NodeParameterPtr (new CountParameter ()));
}

NE::Stream::Status IntegerRangeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	HeaderWithSlotsUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status IntegerRangeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	HeaderWithSlotsUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

}
