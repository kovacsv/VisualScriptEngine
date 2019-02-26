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

NE::DynamicSerializationInfo	BooleanNode::serializationInfo (NE::ObjectId ("{72E14D86-E5DC-4AD6-A7E4-F60D47BFB114}"), NE::ObjectVersion (1), BooleanNode::CreateSerializableInstance);

NE::SerializationInfo			NumericUpDownNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	IntegerUpDownNode::serializationInfo (NE::ObjectId ("{98ADBB4A-8E55-4FE5-B0F9-EC5DD776C000}"), NE::ObjectVersion (1), IntegerUpDownNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	DoubleUpDownNode::serializationInfo (NE::ObjectId ("{F888C04D-FF22-4225-AC9A-90464D01ACF9}"), NE::ObjectVersion (1), DoubleUpDownNode::CreateSerializableInstance);

NE::SerializationInfo			NumericRangeNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	IntegerIncrementedNode::serializationInfo (NE::ObjectId ("{74527771-58A4-42D4-850F-1C63FA9A4732}"), NE::ObjectVersion (1), IntegerIncrementedNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	DoubleIncrementedNode::serializationInfo (NE::ObjectId ("{B697B7DE-7AB9-479D-8DBE-8D3CCB6E4F50}"), NE::ObjectVersion (1), DoubleIncrementedNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	DoubleDistributedNode::serializationInfo (NE::ObjectId ("{DDD3DF7C-AEA5-4E1E-B48A-2A10EA3FC7EF}"), NE::ObjectVersion (1), DoubleDistributedNode::CreateSerializableInstance);

template <typename NodeType>
class MinValueIntegerParameter : public NUIE::SlotDefaultValueNodeParameter<NodeType, NE::IntValue>
{
public:
	MinValueIntegerParameter (const std::wstring& name, const NUIE::ParameterType& type, const NE::SlotId& slotId, int minValue) :
		NUIE::SlotDefaultValueNodeParameter<NodeType, NE::IntValue> (name, type, slotId),
		minValue (minValue)
	{

	}

	virtual bool IsValidValue (const NUIE::UINodeConstPtr&, const std::shared_ptr<const NE::IntValue>& value) const override
	{
		return value->GetValue () >= minValue;
	}

private:
	int minValue;
};

BooleanNode::BooleanNode () :
	BooleanNode (L"", NUIE::Point (), false)
{

}

BooleanNode::BooleanNode (const std::wstring& name, const NUIE::Point& position, bool val) :
	BasicUINode (name, position),
	val (val)
{

}

BooleanNode::~BooleanNode ()
{

}

void BooleanNode::Initialize ()
{
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"Output")));
}

NUIE::EventHandlerResult BooleanNode::HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys&, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface)
{
	if (mouseButton != NUIE::MouseButton::Left) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}
	
	NUIE::Rect switchRect = GetSpecialRect (env, "switch");
	
	if (switchRect.Contains (position)) {
		commandInterface.RunUndoableCommand ([&] () {
			SetValue (!val);
		});
		return NUIE::EventHandlerResult::EventHandled;
	}
	return NUIE::EventHandlerResult::EventNotHandled;
}

NUIE::EventHandlerResult BooleanNode::HandleMouseDoubleClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& keys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface)
{
	return HandleMouseClick (env, keys, mouseButton, position, commandInterface);
}

bool BooleanNode::IsForceCalculated () const
{
	return true;
}

NE::ValueConstPtr BooleanNode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValueConstPtr (new NE::BooleanValue (val));
}

void BooleanNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class ValueParameter : public NUIE::BooleanNodeParameter<BooleanNode>
	{
	public:
		ValueParameter () :
			NUIE::BooleanNodeParameter<BooleanNode> (L"Value")
		{
	
		}
	
		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::BooleanValue (GetTypedNode (uiNode)->GetValue ()));
		}
	
		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			GetTypedNode (uiNode)->SetValue (NE::BooleanValue::Get (value));
			uiManager.InvalidateNodeValue (uiNode);
			uiManager.InvalidateNodeDrawing (uiNode);
			return true;
		}
	};

	BasicUINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new ValueParameter ()));
}

NE::Stream::Status BooleanNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	inputStream.Read (val);
	return inputStream.GetStatus ();
}

NE::Stream::Status BooleanNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
	outputStream.Write (val);
	return outputStream.GetStatus ();
}

bool BooleanNode::GetValue () const
{
	return val;
}

void BooleanNode::SetValue (bool newVal)
{
	val = newVal;
	InvalidateValue ();
}

void BooleanNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	short selectedIndex = val ? 0 : 1;
	DrawHeaderWithSlotsAndSwitchLayout (*this, "switch", L"true", L"false", selectedIndex, env, drawingImage);
}

NumericUpDownNode::NumericUpDownNode () :
	NumericUpDownNode (L"", NUIE::Point ())
{

}

NumericUpDownNode::NumericUpDownNode (const std::wstring& name, const NUIE::Point& position) :
	BasicUINode (name, position)
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
	BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumericUpDownNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
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

NE::ValueConstPtr IntegerUpDownNode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValueConstPtr (new NE::IntValue (val));
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

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::IntValue (GetTypedNode (uiNode)->GetValue ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
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

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::IntValue (GetTypedNode (uiNode)->GetStep ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager&, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			GetTypedNode (uiNode)->SetStep (NE::IntValue::Get (value));
			return true;
		}
	};

	NumericUpDownNode::RegisterParameters (parameterList);
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

NE::ValueConstPtr DoubleUpDownNode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValueConstPtr (new NE::DoubleValue (val));
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

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (GetTypedNode (uiNode)->GetValue ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
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

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (GetTypedNode (uiNode)->GetStep ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager&, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			GetTypedNode (uiNode)->SetStep (NE::DoubleValue::Get (value));
			return true;
		}
	};

	NumericUpDownNode::RegisterParameters (parameterList);
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

IntegerIncrementedNode::IntegerIncrementedNode () :
	IntegerIncrementedNode (L"", NUIE::Point ())
{

}

IntegerIncrementedNode::IntegerIncrementedNode (const std::wstring& name, const NUIE::Point& position) :
	NumericRangeNode (name, position)
{

}

IntegerIncrementedNode::~IntegerIncrementedNode ()
{

}

void IntegerIncrementedNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("step"), L"Step", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValueConstPtr IntegerIncrementedNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr start = EvaluateInputSlot (NE::SlotId ("start"), env);
	NE::ValueConstPtr step = EvaluateInputSlot (NE::SlotId ("step"), env);
	NE::ValueConstPtr count = EvaluateInputSlot (NE::SlotId ("count"), env);
	if (!NE::IsSingleType<NE::NumberValue> (start) || !NE::IsSingleType<NE::NumberValue> (step) || !NE::IsSingleType<NE::NumberValue> (count)) {
		return nullptr;
	}

	int startNum = NE::NumberValue::ToInteger (NE::CreateSingleValue (start));
	int stepNum = NE::NumberValue::ToInteger (NE::CreateSingleValue (step));
	int countNum = NE::NumberValue::ToInteger (NE::CreateSingleValue (count));
	if (countNum < 0) {
		return nullptr;
	}

	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countNum; ++i) {
		list->Push (NE::ValuePtr (new NE::IntValue (startNum + i * stepNum)));
	}

	return list;
}

void IntegerIncrementedNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<IntegerIncrementedNode, NE::IntValue> (parameterList, L"Start", NUIE::ParameterType::Integer, NE::SlotId ("start"));
	NUIE::RegisterSlotDefaultValueNodeParameter<IntegerIncrementedNode, NE::IntValue> (parameterList, L"Step", NUIE::ParameterType::Integer, NE::SlotId ("step"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new MinValueIntegerParameter<IntegerIncrementedNode> (L"Count", NUIE::ParameterType::Integer, NE::SlotId ("count"), 0)));
}

NE::Stream::Status IntegerIncrementedNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericRangeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status IntegerIncrementedNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericRangeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

DoubleIncrementedNode::DoubleIncrementedNode () :
	DoubleIncrementedNode (L"", NUIE::Point ())
{

}

DoubleIncrementedNode::DoubleIncrementedNode (const std::wstring& name, const NUIE::Point& position) :
	NumericRangeNode (name, position)
{

}

DoubleIncrementedNode::~DoubleIncrementedNode ()
{

}

void DoubleIncrementedNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("step"), L"Step", NE::ValuePtr (new NE::DoubleValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValueConstPtr DoubleIncrementedNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr start = EvaluateInputSlot (NE::SlotId ("start"), env);
	NE::ValueConstPtr step = EvaluateInputSlot (NE::SlotId ("step"), env);
	NE::ValueConstPtr count = EvaluateInputSlot (NE::SlotId ("count"), env);
	if (!NE::IsSingleType<NE::NumberValue> (start) || !NE::IsSingleType<NE::NumberValue> (step) || !NE::IsSingleType<NE::NumberValue> (count)) {
		return nullptr;
	}

	double startNum = NE::NumberValue::ToDouble (NE::CreateSingleValue (start));
	double stepNum = NE::NumberValue::ToDouble (NE::CreateSingleValue (step));
	int countNum = NE::NumberValue::ToInteger (NE::CreateSingleValue (count));
	if (countNum < 0) {
		return nullptr;
	}

	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countNum; ++i) {
		list->Push (NE::ValuePtr (new NE::DoubleValue (startNum + i * stepNum)));
	}

	return list;
}

void DoubleIncrementedNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleIncrementedNode, NE::DoubleValue> (parameterList, L"Start", NUIE::ParameterType::Double, NE::SlotId ("start"));
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleIncrementedNode, NE::DoubleValue> (parameterList, L"Step", NUIE::ParameterType::Double, NE::SlotId ("step"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new MinValueIntegerParameter<DoubleIncrementedNode> (L"Count", NUIE::ParameterType::Integer, NE::SlotId ("count"), 0)));
}

NE::Stream::Status DoubleIncrementedNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericRangeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status DoubleIncrementedNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericRangeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

DoubleDistributedNode::DoubleDistributedNode () :
	DoubleDistributedNode (L"", NUIE::Point ())
{

}

DoubleDistributedNode::DoubleDistributedNode (const std::wstring& name, const NUIE::Point& position) :
	NumericRangeNode (name, position)
{

}

DoubleDistributedNode::~DoubleDistributedNode ()
{

}

void DoubleDistributedNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("end"), L"End", NE::ValuePtr (new NE::DoubleValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValueConstPtr DoubleDistributedNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr start = EvaluateInputSlot (NE::SlotId ("start"), env);
	NE::ValueConstPtr end = EvaluateInputSlot (NE::SlotId ("end"), env);
	NE::ValueConstPtr count = EvaluateInputSlot (NE::SlotId ("count"), env);
	if (!NE::IsSingleType<NE::NumberValue> (start) || !NE::IsSingleType<NE::NumberValue> (end) || !NE::IsSingleType<NE::NumberValue> (count)) {
		return nullptr;
	}

	double startNum = NE::NumberValue::ToDouble (NE::CreateSingleValue (start));
	double endNum = NE::NumberValue::ToDouble (NE::CreateSingleValue (end));
	int countNum = NE::NumberValue::ToInteger (NE::CreateSingleValue (count));
	if (countNum < 2) {
		return nullptr;
	}

	double segmentVal = fabs (startNum - endNum) / (double) (countNum - 1);
	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countNum; ++i) {
		list->Push (NE::ValuePtr (new NE::DoubleValue (startNum + i * segmentVal)));
	}

	return list;
}

void DoubleDistributedNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleDistributedNode, NE::DoubleValue> (parameterList, L"Start", NUIE::ParameterType::Double, NE::SlotId ("start"));
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleDistributedNode, NE::DoubleValue> (parameterList, L"End", NUIE::ParameterType::Double, NE::SlotId ("end"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new MinValueIntegerParameter<DoubleDistributedNode> (L"Count", NUIE::ParameterType::Integer, NE::SlotId ("count"), 2)));
}

NE::Stream::Status DoubleDistributedNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericRangeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status DoubleDistributedNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericRangeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

}
