#include "InputUINodes.hpp"
#include "UINodePanels.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"
#include "UINodeLayouts.hpp"
#include "NodeUIManager.hpp"
#include "SkinParams.hpp"

#include <algorithm>

namespace BI
{

NE::DynamicSerializationInfo	NumericUpDownNode::serializationInfo (NE::ObjectId ("{F888C04D-FF22-4225-AC9A-90464D01ACF9}"), NE::ObjectVersion (1), NumericUpDownNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	NumberRangeNode::serializationInfo (NE::ObjectId ("{B697B7DE-7AB9-479D-8DBE-8D3CCB6E4F50}"), NE::ObjectVersion (1), NumberRangeNode::CreateSerializableInstance);

NumericUpDownNode::NumericUpDownNode () :
	UINode (),
	val (0.0),
	step (0.0)
{

}

NumericUpDownNode::NumericUpDownNode (const std::wstring& name, const NUIE::Point& position, double val, double step) :
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
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"Output")));
}

NE::ValuePtr NumericUpDownNode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValuePtr (new NE::DoubleValue (val));
}

void NumericUpDownNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class ValueParameter : public NUIE::DoubleParameter<NumericUpDownNode>
	{
	public:
		ValueParameter () :
			NUIE::DoubleParameter<NumericUpDownNode> (L"Value")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const std::shared_ptr<NumericUpDownNode>& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (uiNode->GetValue ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, std::shared_ptr<NumericUpDownNode>& uiNode, const NE::ValuePtr& value) override
		{
			uiNode->SetValue (NE::DoubleValue::Get (value));
			uiManager.InvalidateNodeValue (uiNode);
			uiManager.InvalidateNodeDrawing (uiNode);
			return true;
		}
	};

	class StepParameter : public NUIE::DoubleParameter<NumericUpDownNode>
	{
	public:
		StepParameter () :
			NUIE::DoubleParameter<NumericUpDownNode> (L"Step")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const std::shared_ptr<NumericUpDownNode>& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (uiNode->GetStep ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager&, NE::EvaluationEnv&, std::shared_ptr<NumericUpDownNode>& uiNode, const NE::ValuePtr& value) override
		{
			uiNode->SetStep (NE::DoubleValue::Get (value));
			return true;
		}
	};

	UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new ValueParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new StepParameter ()));
}

NUIE::EventHandlerResult NumericUpDownNode::HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::KeySet&, NUIE::MouseButton mouseButton, const NUIE::Point& position)
{
	if (mouseButton != NUIE::MouseButton::Left) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	NUIE::Rect minusButtonRect = GetSpecialRect (env, "minus");
	NUIE::Rect plusButtonRect = GetSpecialRect (env, "plus");

	if (minusButtonRect.Contains (position)) {
		Decrease ();
		return NUIE::EventHandlerResult::EventHandled;
	} else if (plusButtonRect.Contains (position)) {
		Increase ();
		return NUIE::EventHandlerResult::EventHandled;
	}
	return NUIE::EventHandlerResult::EventNotHandled;
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

void NumericUpDownNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	DrawHeaderWithSlotsAndButtonsLayout (*this, "minus", L"<", "plus", L">", env, drawingImage);
}

NumberRangeNode::NumberRangeNode () :
	NumberRangeNode (L"", NUIE::Point ())
{

}

NumberRangeNode::NumberRangeNode (const std::wstring& name, const NUIE::Point& position) :
	UINode (name, position)
{

}

NumberRangeNode::~NumberRangeNode ()
{

}

void NumberRangeNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("step"), L"Step", NE::ValuePtr (new NE::DoubleValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValuePtr NumberRangeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr start = EvaluateSingleInputSlot (NE::SlotId ("start"), env);
	NE::ValuePtr step = EvaluateSingleInputSlot (NE::SlotId ("step"), env);
	NE::ValuePtr count = EvaluateSingleInputSlot (NE::SlotId ("count"), env);
	if (!NE::IsSingleType<NE::NumberValue> (start) || !NE::IsSingleType<NE::NumberValue> (step) || !NE::IsSingleType<NE::NumberValue> (count)) {
		return nullptr;
	}

	double startNum = NE::NumberValue::ToDouble (NE::CreateSingleValue (start));
	double stepNum = NE::NumberValue::ToDouble (NE::CreateSingleValue (step));
	int countNum = NE::NumberValue::ToInteger (NE::CreateSingleValue (count));
	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countNum; ++i) {
		list->Push (NE::ValuePtr (new NE::DoubleValue (startNum + i * stepNum)));
	}

	return list;
}

void NumberRangeNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class CountParameter : public NUIE::SlotDefaultValueParameter<NumberRangeNode, NE::IntValue>
	{
	public:
		CountParameter () :
			NUIE::SlotDefaultValueParameter<NumberRangeNode, NE::IntValue> (L"Count", NUIE::ParameterType::Integer, NE::SlotId ("count"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodePtr&, const std::shared_ptr<NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0.0;
		}
	};

	UINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueParameter<NumberRangeNode, NE::DoubleValue> (parameterList, L"Start", NUIE::ParameterType::Double, NE::SlotId ("start"));
	NUIE::RegisterSlotDefaultValueParameter<NumberRangeNode, NE::DoubleValue> (parameterList, L"Step", NUIE::ParameterType::Double, NE::SlotId ("step"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new CountParameter ()));
}

NE::Stream::Status NumberRangeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumberRangeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	return outputStream.GetStatus ();
}

void NumberRangeNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	DrawStatusHeaderWithSlotsLayout (*this, env, drawingImage);
}

}
