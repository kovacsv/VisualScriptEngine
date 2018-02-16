#include "InputUINodes.hpp"
#include "UINodePanels.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"
#include "NodeUIManager.hpp"
#include "SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

NE::DynamicSerializationInfo	NumberUpDownUINode::serializationInfo (NE::ObjectId ("{F888C04D-FF22-4225-AC9A-90464D01ACF9}"), NE::ObjectVersion (1), NumberUpDownUINode::CreateSerializableInstance);
NE::DynamicSerializationInfo	NumberRangeUINode::serializationInfo (NE::ObjectId ("{B697B7DE-7AB9-479D-8DBE-8D3CCB6E4F50}"), NE::ObjectVersion (1), NumberRangeUINode::CreateSerializableInstance);

NumberUpDownUINode::NumberUpDownUINode () :
	UINode (),
	val (0.0),
	step (0.0)
{

}

NumberUpDownUINode::NumberUpDownUINode (const std::wstring& name, const Point& position, double val, double step) :
	UINode (name, position),
	val (val),
	step (step)
{

}

NumberUpDownUINode::~NumberUpDownUINode ()
{

}

void NumberUpDownUINode::RegisterSlots ()
{
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"Output")));
}

NE::ValuePtr NumberUpDownUINode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValuePtr (new NE::DoubleValue (val));
}

void NumberUpDownUINode::RegisterParameters (NodeParameterList& parameterList) const
{
	class ValueParameter : public DoubleParameter<NumberUpDownUINode>
	{
	public:
		ValueParameter () :
			DoubleParameter<NumberUpDownUINode> ("NumberUpDownUINodeValueParameter", L"Value")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const std::shared_ptr<NumberUpDownUINode>& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (uiNode->GetValue ()));
		}

		virtual bool SetValueInternal (NodeUIManager& uiManager, NE::EvaluationEnv&, std::shared_ptr<NumberUpDownUINode>& uiNode, const NE::ValuePtr& value) override
		{
			uiNode->SetValue (NE::DoubleValue::Get (value));
			uiManager.InvalidateNodeValue (uiNode);
			uiManager.InvalidateNodeDrawing (uiNode);
			return true;
		}
	};

	class StepParameter : public DoubleParameter<NumberUpDownUINode>
	{
	public:
		StepParameter () :
			DoubleParameter<NumberUpDownUINode> ("NumberUpDownUINodeStepParameter", L"Step")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const std::shared_ptr<NumberUpDownUINode>& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (uiNode->GetStep ()));
		}

		virtual bool SetValueInternal (NodeUIManager&, NE::EvaluationEnv&, std::shared_ptr<NumberUpDownUINode>& uiNode, const NE::ValuePtr& value) override
		{
			uiNode->SetStep (NE::DoubleValue::Get (value));
			return true;
		}
	};

	UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NodeParameterPtr (new ValueParameter ()));
	parameterList.AddParameter (NodeParameterPtr (new StepParameter ()));
}

EventHandlerResult NumberUpDownUINode::HandleMouseClick (NodeUIEnvironment& env, const KeySet&, MouseButton mouseButton, const Point& position)
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

NE::Stream::Status NumberUpDownUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	inputStream.Read (val);
	inputStream.Read (step);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumberUpDownUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	outputStream.Write (val);
	outputStream.Write (step);
	return outputStream.GetStatus ();
}

void NumberUpDownUINode::Increase ()
{
	val = val + step;
	InvalidateValue ();
}

void NumberUpDownUINode::Decrease ()
{
	val = val - step;
	InvalidateValue ();
}

double NumberUpDownUINode::GetValue () const
{
	return val;
}

void NumberUpDownUINode::SetValue (double newValue)
{
	val = newValue;
	InvalidateValue ();
}

double NumberUpDownUINode::GetStep () const
{
	return step;
}

void NumberUpDownUINode::SetStep (double newStep)
{
	step = newStep;
}

void NumberUpDownUINode::UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const
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

void NumberUpDownUINode::CalculationPostProcess (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

NumberRangeUINode::NumberRangeUINode () :
	NumberRangeUINode (L"", Point ())
{

}

NumberRangeUINode::NumberRangeUINode (const std::wstring& name, const Point& position) :
	HeaderWithSlotsUINode (name, position)
{

}

NumberRangeUINode::~NumberRangeUINode ()
{

}

void NumberRangeUINode::RegisterSlots ()
{
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("step"), L"Step", NE::ValuePtr (new NE::DoubleValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValuePtr NumberRangeUINode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr start = EvaluateSingleInputSlot (NE::SlotId ("start"), env);
	NE::ValuePtr step = EvaluateSingleInputSlot (NE::SlotId ("step"), env);
	NE::ValuePtr count = EvaluateSingleInputSlot (NE::SlotId ("count"), env);
	if (!NE::Value::IsType<NE::NumberValue> (start) || !NE::Value::IsType<NE::NumberValue> (step) || !NE::Value::IsType<NE::NumberValue> (count)) {
		return nullptr;
	}

	double startNum = NE::NumberValue::ToDouble (start);
	double stepNum = NE::NumberValue::ToDouble (step);
	int countNum = (int) NE::NumberValue::ToDouble (count);
	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countNum; ++i) {
		list->Push (NE::ValuePtr (new NE::DoubleValue (startNum + i * stepNum)));
	}

	return list;
}

void NumberRangeUINode::RegisterParameters (NodeParameterList& parameterList) const
{
	class StartParameter : public SlotDefaultValueParameter<NumberRangeUINode, NE::DoubleValue>
	{
	public:
		StartParameter () :
			SlotDefaultValueParameter<NumberRangeUINode, NE::DoubleValue> ("NumberRangeUINodeStartParameter", L"Start", ParameterType::Double, NE::SlotId ("start"))
		{

		}
	};

	class StepParameter : public SlotDefaultValueParameter<NumberRangeUINode, NE::DoubleValue>
	{
	public:
		StepParameter () :
			SlotDefaultValueParameter<NumberRangeUINode, NE::DoubleValue> ("NumberRangeUINodeStepParameter", L"Step", ParameterType::Double, NE::SlotId ("step"))
		{

		}
	};

	class CountParameter : public SlotDefaultValueParameter<NumberRangeUINode, NE::IntValue>
	{
	public:
		CountParameter () :
			SlotDefaultValueParameter<NumberRangeUINode, NE::IntValue> ("NumberRangeUINodeCountParameter", L"Count", ParameterType::Integer, NE::SlotId ("count"))
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

NE::Stream::Status NumberRangeUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	HeaderWithSlotsUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumberRangeUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	HeaderWithSlotsUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

}
