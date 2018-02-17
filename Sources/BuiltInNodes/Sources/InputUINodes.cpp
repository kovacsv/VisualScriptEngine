#include "InputUINodes.hpp"
#include "UINodePanels.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"
#include "NodeUIManager.hpp"
#include "SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

NE::DynamicSerializationInfo	NumericUpDownNode::serializationInfo (NE::ObjectId ("{F888C04D-FF22-4225-AC9A-90464D01ACF9}"), NE::ObjectVersion (1), NumericUpDownNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	NumberRangeNode::serializationInfo (NE::ObjectId ("{B697B7DE-7AB9-479D-8DBE-8D3CCB6E4F50}"), NE::ObjectVersion (1), NumberRangeNode::CreateSerializableInstance);

NumericUpDownNode::NumericUpDownNode () :
	UINode (),
	val (0.0),
	step (0.0)
{

}

NumericUpDownNode::NumericUpDownNode (const std::wstring& name, const Point& position, double val, double step) :
	UINode (name, position),
	val (val),
	step (step)
{

}

NumericUpDownNode::~NumericUpDownNode ()
{

}

void NumericUpDownNode::RegisterSlots ()
{
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"Output")));
}

NE::ValuePtr NumericUpDownNode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValuePtr (new NE::DoubleValue (val));
}

void NumericUpDownNode::RegisterParameters (NodeParameterList& parameterList) const
{
	class ValueParameter : public DoubleParameter<NumericUpDownNode>
	{
	public:
		ValueParameter () :
			DoubleParameter<NumericUpDownNode> ("NumericUpDownNodeValueParameter", L"Value")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const std::shared_ptr<NumericUpDownNode>& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (uiNode->GetValue ()));
		}

		virtual bool SetValueInternal (NodeUIManager& uiManager, NE::EvaluationEnv&, std::shared_ptr<NumericUpDownNode>& uiNode, const NE::ValuePtr& value) override
		{
			uiNode->SetValue (NE::DoubleValue::Get (value));
			uiManager.InvalidateNodeValue (uiNode);
			uiManager.InvalidateNodeDrawing (uiNode);
			return true;
		}
	};

	class StepParameter : public DoubleParameter<NumericUpDownNode>
	{
	public:
		StepParameter () :
			DoubleParameter<NumericUpDownNode> ("NumericUpDownNodeStepParameter", L"Step")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const std::shared_ptr<NumericUpDownNode>& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (uiNode->GetStep ()));
		}

		virtual bool SetValueInternal (NodeUIManager&, NE::EvaluationEnv&, std::shared_ptr<NumericUpDownNode>& uiNode, const NE::ValuePtr& value) override
		{
			uiNode->SetStep (NE::DoubleValue::Get (value));
			return true;
		}
	};

	UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NodeParameterPtr (new ValueParameter ()));
	parameterList.AddParameter (NodeParameterPtr (new StepParameter ()));
}

EventHandlerResult NumericUpDownNode::HandleMouseClick (NodeUIEnvironment& env, const KeySet&, MouseButton mouseButton, const Point& position)
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

NE::Stream::Status NumericUpDownNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	inputStream.Read (val);
	inputStream.Read (step);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumericUpDownNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	outputStream.Write (val);
	outputStream.Write (step);
	return outputStream.GetStatus ();
}

void NumericUpDownNode::Increase ()
{
	val = val + step;
	InvalidateValue ();
}

void NumericUpDownNode::Decrease ()
{
	val = val - step;
	InvalidateValue ();
}

double NumericUpDownNode::GetValue () const
{
	return val;
}

void NumericUpDownNode::SetValue (double newValue)
{
	val = newValue;
	InvalidateValue ();
}

double NumericUpDownNode::GetStep () const
{
	return step;
}

void NumericUpDownNode::SetStep (double newStep)
{
	step = newStep;
}

void NumericUpDownNode::UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const
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

void NumericUpDownNode::CalculationPostProcess (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

NumberRangeNode::NumberRangeNode () :
	NumberRangeNode (L"", Point ())
{

}

NumberRangeNode::NumberRangeNode (const std::wstring& name, const Point& position) :
	HeaderWithSlotsUINode (name, position)
{

}

NumberRangeNode::~NumberRangeNode ()
{

}

void NumberRangeNode::RegisterSlots ()
{
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("step"), L"Step", NE::ValuePtr (new NE::DoubleValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValuePtr NumberRangeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr start = EvaluateSingleInputSlot (NE::SlotId ("start"), env);
	NE::ValuePtr step = EvaluateSingleInputSlot (NE::SlotId ("step"), env);
	NE::ValuePtr count = EvaluateSingleInputSlot (NE::SlotId ("count"), env);
	if (!NE::Value::IsType<NE::NumberValue> (start) || !NE::Value::IsType<NE::NumberValue> (step) || !NE::Value::IsType<NE::NumberValue> (count)) {
		return nullptr;
	}

	double startNum = NE::NumberValue::ToDouble (start);
	double stepNum = NE::NumberValue::ToDouble (step);
	int countNum = NE::NumberValue::ToInteger (count);
	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countNum; ++i) {
		list->Push (NE::ValuePtr (new NE::DoubleValue (startNum + i * stepNum)));
	}

	return list;
}

void NumberRangeNode::RegisterParameters (NodeParameterList& parameterList) const
{
	class StartParameter : public SlotDefaultValueParameter<NumberRangeNode, NE::DoubleValue>
	{
	public:
		StartParameter () :
			SlotDefaultValueParameter<NumberRangeNode, NE::DoubleValue> ("NumberRangeNodeStartParameter", L"Start", ParameterType::Double, NE::SlotId ("start"))
		{

		}
	};

	class StepParameter : public SlotDefaultValueParameter<NumberRangeNode, NE::DoubleValue>
	{
	public:
		StepParameter () :
			SlotDefaultValueParameter<NumberRangeNode, NE::DoubleValue> ("NumberRangeNodeStepParameter", L"Step", ParameterType::Double, NE::SlotId ("step"))
		{

		}
	};

	class CountParameter : public SlotDefaultValueParameter<NumberRangeNode, NE::IntValue>
	{
	public:
		CountParameter () :
			SlotDefaultValueParameter<NumberRangeNode, NE::IntValue> ("NumberRangeNodeCountParameter", L"Count", ParameterType::Integer, NE::SlotId ("count"))
		{

		}

		virtual bool IsValidValue (const UINodePtr&, const std::shared_ptr<NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0.0;
		}
	};

	UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NodeParameterPtr (new StartParameter ()));
	parameterList.AddParameter (NodeParameterPtr (new StepParameter ()));
	parameterList.AddParameter (NodeParameterPtr (new CountParameter ()));
}

NE::Stream::Status NumberRangeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	HeaderWithSlotsUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumberRangeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	HeaderWithSlotsUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

}
