#include "BI_InputUINodes.hpp"
#include "BI_UINodePanels.hpp"
#include "BI_UINodeLayouts.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_SkinParams.hpp"

#include <algorithm>

namespace BI
{

NE::SerializationInfo			NumericUpDownNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	IntegerUpDownNode::serializationInfo (NE::ObjectId ("{98ADBB4A-8E55-4FE5-B0F9-EC5DD776C000}"), NE::ObjectVersion (1), IntegerUpDownNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	DoubleUpDownNode::serializationInfo (NE::ObjectId ("{F888C04D-FF22-4225-AC9A-90464D01ACF9}"), NE::ObjectVersion (1), DoubleUpDownNode::CreateSerializableInstance);

NE::SerializationInfo			NumericRangeNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	IntegerRangeNode::serializationInfo (NE::ObjectId ("{74527771-58A4-42D4-850F-1C63FA9A4732}"), NE::ObjectVersion (1), IntegerRangeNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	DoubleRangeNode::serializationInfo (NE::ObjectId ("{B697B7DE-7AB9-479D-8DBE-8D3CCB6E4F50}"), NE::ObjectVersion (1), DoubleRangeNode::CreateSerializableInstance);

NumericUpDownNode::NumericUpDownNode () :
	NumericUpDownNode (L"", NUIE::Point ())
{

}

NumericUpDownNode::NumericUpDownNode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::UINode (name, position)
{

}

NumericUpDownNode::~NumericUpDownNode ()
{

}

void NumericUpDownNode::Initialize ()
{
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"Output")));
}

NUIE::EventHandlerResult NumericUpDownNode::HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys&, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface)
{
	if (mouseButton != NUIE::MouseButton::Left) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	NUIE::Rect minusButtonRect = GetSpecialRect (env, "minus");
	NUIE::Rect plusButtonRect = GetSpecialRect (env, "plus");

	if (minusButtonRect.Contains (position)) {
		commandInterface.RunUndoableCommand ([&] () {
			Decrease ();
		});
		return NUIE::EventHandlerResult::EventHandled;
	} else if (plusButtonRect.Contains (position)) {
		commandInterface.RunUndoableCommand ([&] () {
			Increase ();
		});
		return NUIE::EventHandlerResult::EventHandled;
	}
	return NUIE::EventHandlerResult::EventNotHandled;
}

NUIE::EventHandlerResult NumericUpDownNode::HandleMouseDoubleClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& keys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface)
{
	return HandleMouseClick (env, keys, mouseButton, position, commandInterface);
}

bool NumericUpDownNode::IsForceCalculated () const
{
	return true;
}

NE::Stream::Status NumericUpDownNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::UINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumericUpDownNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::UINode::Write (outputStream);
	return outputStream.GetStatus ();
}

void NumericUpDownNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	DrawHeaderWithSlotsAndButtonsLayout (*this, "minus", L"<", "plus", L">", env, drawingImage);
}

IntegerUpDownNode::IntegerUpDownNode () :
	IntegerUpDownNode (L"", NUIE::Point (), 0, 0)
{

}

IntegerUpDownNode::IntegerUpDownNode (const std::wstring& name, const NUIE::Point& position, int val, int step) :
	NumericUpDownNode (name, position),
	val (val),
	step (step)
{

}

IntegerUpDownNode::~IntegerUpDownNode ()
{

}

NE::ValuePtr IntegerUpDownNode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValuePtr (new NE::IntValue (val));
}

void IntegerUpDownNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class ValueParameter : public NUIE::IntegerNodeParameter<IntegerUpDownNode>
	{
	public:
		ValueParameter () :
			NUIE::IntegerNodeParameter<IntegerUpDownNode> (L"Value")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::IntValue (GetTypedNode (uiNode)->GetValue ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			GetTypedNode (uiNode)->SetValue (NE::IntValue::Get (value));
			uiManager.InvalidateNodeValue (uiNode);
			uiManager.InvalidateNodeDrawing (uiNode);
			return true;
		}
	};

	class StepParameter : public NUIE::IntegerNodeParameter<IntegerUpDownNode>
	{
	public:
		StepParameter () :
			NUIE::IntegerNodeParameter<IntegerUpDownNode> (L"Step")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::IntValue (GetTypedNode (uiNode)->GetStep ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager&, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			GetTypedNode (uiNode)->SetStep (NE::IntValue::Get (value));
			return true;
		}
	};

	NUIE::UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new ValueParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new StepParameter ()));
}

NE::Stream::Status IntegerUpDownNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericUpDownNode::Read (inputStream);
	inputStream.Read (val);
	inputStream.Read (step);
	return inputStream.GetStatus ();
}

NE::Stream::Status IntegerUpDownNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericUpDownNode::Write (outputStream);
	outputStream.Write (val);
	outputStream.Write (step);
	return outputStream.GetStatus ();
}

void IntegerUpDownNode::Increase ()
{
	val = val + step;
	InvalidateValue ();
}

void IntegerUpDownNode::Decrease ()
{
	val = val - step;
	InvalidateValue ();
}

int IntegerUpDownNode::GetValue () const
{
	return val;
}

void IntegerUpDownNode::SetValue (int newValue)
{
	val = newValue;
	InvalidateValue ();
}

int IntegerUpDownNode::GetStep () const
{
	return step;
}

void IntegerUpDownNode::SetStep (int newStep)
{
	step = newStep;
}

DoubleUpDownNode::DoubleUpDownNode () :
	DoubleUpDownNode (L"", NUIE::Point (), 0.0, 0.0)
{

}

DoubleUpDownNode::DoubleUpDownNode (const std::wstring& name, const NUIE::Point& position, double val, double step) :
	NumericUpDownNode (name, position),
	val (val),
	step (step)
{

}

DoubleUpDownNode::~DoubleUpDownNode ()
{

}

NE::ValuePtr DoubleUpDownNode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValuePtr (new NE::DoubleValue (val));
}

void DoubleUpDownNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class ValueParameter : public NUIE::DoubleNodeParameter<DoubleUpDownNode>
	{
	public:
		ValueParameter () :
			NUIE::DoubleNodeParameter<DoubleUpDownNode> (L"Value")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (GetTypedNode (uiNode)->GetValue ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			GetTypedNode (uiNode)->SetValue (NE::DoubleValue::Get (value));
			uiManager.InvalidateNodeValue (uiNode);
			uiManager.InvalidateNodeDrawing (uiNode);
			return true;
		}
	};

	class StepParameter : public NUIE::DoubleNodeParameter<DoubleUpDownNode>
	{
	public:
		StepParameter () :
			NUIE::DoubleNodeParameter<DoubleUpDownNode> (L"Step")
		{

		}

		virtual NE::ValuePtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (GetTypedNode (uiNode)->GetStep ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager&, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			GetTypedNode (uiNode)->SetStep (NE::DoubleValue::Get (value));
			return true;
		}
	};

	NUIE::UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new ValueParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new StepParameter ()));
}

NE::Stream::Status DoubleUpDownNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericUpDownNode::Read (inputStream);
	inputStream.Read (val);
	inputStream.Read (step);
	return inputStream.GetStatus ();
}

NE::Stream::Status DoubleUpDownNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericUpDownNode::Write (outputStream);
	outputStream.Write (val);
	outputStream.Write (step);
	return outputStream.GetStatus ();
}

void DoubleUpDownNode::Increase ()
{
	val = val + step;
	InvalidateValue ();
}

void DoubleUpDownNode::Decrease ()
{
	val = val - step;
	InvalidateValue ();
}

double DoubleUpDownNode::GetValue () const
{
	return val;
}

void DoubleUpDownNode::SetValue (double newValue)
{
	val = newValue;
	InvalidateValue ();
}

double DoubleUpDownNode::GetStep () const
{
	return step;
}

void DoubleUpDownNode::SetStep (double newStep)
{
	step = newStep;
}

NumericRangeNode::NumericRangeNode () :
	NumericRangeNode (L"", NUIE::Point ())
{

}

NumericRangeNode::NumericRangeNode (const std::wstring& name, const NUIE::Point& position) :
	BasicUINode (name, position)
{

}

NumericRangeNode::~NumericRangeNode ()
{

}

bool NumericRangeNode::IsForceCalculated () const
{
	return true;
}

NE::Stream::Status NumericRangeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumericRangeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

IntegerRangeNode::IntegerRangeNode () :
	IntegerRangeNode (L"", NUIE::Point ())
{

}

IntegerRangeNode::IntegerRangeNode (const std::wstring& name, const NUIE::Point& position) :
	NumericRangeNode (name, position)
{

}

IntegerRangeNode::~IntegerRangeNode ()
{

}

void IntegerRangeNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("step"), L"Step", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValuePtr IntegerRangeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr start = EvaluateSingleInputSlot (NE::SlotId ("start"), env);
	NE::ValuePtr step = EvaluateSingleInputSlot (NE::SlotId ("step"), env);
	NE::ValuePtr count = EvaluateSingleInputSlot (NE::SlotId ("count"), env);
	if (!NE::IsSingleType<NE::NumberValue> (start) || !NE::IsSingleType<NE::NumberValue> (step) || !NE::IsSingleType<NE::NumberValue> (count)) {
		return nullptr;
	}

	int startNum = NE::NumberValue::ToInteger (NE::CreateSingleValue (start));
	int stepNum = NE::NumberValue::ToInteger (NE::CreateSingleValue (step));
	int countNum = NE::NumberValue::ToInteger (NE::CreateSingleValue (count));
	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countNum; ++i) {
		list->Push (NE::ValuePtr (new NE::IntValue (startNum + i * stepNum)));
	}

	return list;
}

void IntegerRangeNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class CountParameter : public NUIE::SlotDefaultValueNodeParameter<IntegerRangeNode, NE::IntValue>
	{
	public:
		CountParameter () :
			NUIE::SlotDefaultValueNodeParameter<IntegerRangeNode, NE::IntValue> (L"Count", NUIE::ParameterType::Integer, NE::SlotId ("count"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodeConstPtr&, const std::shared_ptr<NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0.0;
		}
	};

	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<IntegerRangeNode, NE::IntValue> (parameterList, L"Start", NUIE::ParameterType::Integer, NE::SlotId ("start"));
	NUIE::RegisterSlotDefaultValueNodeParameter<IntegerRangeNode, NE::IntValue> (parameterList, L"Step", NUIE::ParameterType::Integer, NE::SlotId ("step"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new CountParameter ()));
}

NE::Stream::Status IntegerRangeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericRangeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status IntegerRangeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericRangeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

DoubleRangeNode::DoubleRangeNode () :
	DoubleRangeNode (L"", NUIE::Point ())
{

}

DoubleRangeNode::DoubleRangeNode (const std::wstring& name, const NUIE::Point& position) :
	NumericRangeNode (name, position)
{

}

DoubleRangeNode::~DoubleRangeNode ()
{

}

void DoubleRangeNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("step"), L"Step", NE::ValuePtr (new NE::DoubleValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValuePtr DoubleRangeNode::Calculate (NE::EvaluationEnv& env) const
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

void DoubleRangeNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class CountParameter : public NUIE::SlotDefaultValueNodeParameter<DoubleRangeNode, NE::IntValue>
	{
	public:
		CountParameter () :
			NUIE::SlotDefaultValueNodeParameter<DoubleRangeNode, NE::IntValue> (L"Count", NUIE::ParameterType::Integer, NE::SlotId ("count"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodeConstPtr&, const std::shared_ptr<NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0.0;
		}
	};

	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleRangeNode, NE::DoubleValue> (parameterList, L"Start", NUIE::ParameterType::Double, NE::SlotId ("start"));
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleRangeNode, NE::DoubleValue> (parameterList, L"Step", NUIE::ParameterType::Double, NE::SlotId ("step"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new CountParameter ()));
}

NE::Stream::Status DoubleRangeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericRangeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status DoubleRangeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericRangeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

}
