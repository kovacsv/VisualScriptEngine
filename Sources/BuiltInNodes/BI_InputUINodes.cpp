#include "BI_InputUINodes.hpp"
#include "BI_UINodePanels.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_SkinParams.hpp"

#include <cmath>
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

NE::DynamicSerializationInfo	ListBuilderNode::serializationInfo (NE::ObjectId ("{FE9C19DE-4847-458D-8F2D-5D943E7CF8AF}"), NE::ObjectVersion (1), ListBuilderNode::CreateSerializableInstance);

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

BooleanNode::Layout::Layout (	const std::string& switchButtonId,
								const std::wstring& firstSwitchText,
								const std::wstring& secondSwitchText) :
	HeaderWithSlotsAndSwitchLayout (switchButtonId, firstSwitchText, secondSwitchText)
{

}

HeaderWithSlotsAndSwitchLayout::SelectedItem BooleanNode::Layout::GetSelectedItem (const BasicUINode& uiNode) const
{
	const BooleanNode* booleanNode = dynamic_cast<const BooleanNode*> (&uiNode);
	bool nodeValue = booleanNode->GetValue ();
	return nodeValue ? HeaderWithSlotsAndSwitchLayout::SelectedItem::First : HeaderWithSlotsAndSwitchLayout::SelectedItem::Second;
}

std::shared_ptr<HeaderWithSlotsAndSwitchLayout::ClickHandler> BooleanNode::Layout::GetClickHandler (BasicUINode& uiNode) const
{
	class ClickHandler : public HeaderWithSlotsAndSwitchLayout::ClickHandler
	{
	public:
		ClickHandler (BooleanNode* node) :
			node (node)
		{
		}

		virtual void SwitchClicked () override
		{
			node->SetValue (!node->GetValue ());
		}

	private:
		BooleanNode* node;
	};

	return std::shared_ptr<HeaderWithSlotsAndSwitchLayout::ClickHandler> (new ClickHandler (dynamic_cast<BooleanNode*> (&uiNode)));
}

BooleanNode::BooleanNode () :
	BooleanNode (std::wstring (), NUIE::Point (), false)
{

}

BooleanNode::BooleanNode (const std::wstring& name, const NUIE::Point& position, bool val) :
	BasicUINode (name, position, NUIE::InvalidIconId, UINodeLayoutPtr (new Layout ("switch", NE::Localize (L"true"), NE::Localize (L"false")))),
	val (val)
{

}

BooleanNode::~BooleanNode ()
{

}

void BooleanNode::Initialize ()
{
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::Localize (L"Output"))));
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
			NUIE::BooleanNodeParameter<BooleanNode> (NE::Localize (L"Value"))
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
}

NumericUpDownNode::Layout::Layout (	const std::string& leftButtonId,
									const std::wstring& leftButtonText,
									const std::string& rightButtonId,
									const std::wstring& rightButtonText) :
	HeaderWithSlotsAndButtonsLayout (leftButtonId, leftButtonText, rightButtonId, rightButtonText)
{

}

std::wstring NumericUpDownNode::Layout::GetMiddleText (const BasicUINode& uiNode, const NE::StringSettings& stringSettings) const
{
	std::wstring nodeText = NE::Localize (L"<empty>");
	if (uiNode.HasCalculatedValue ()) {
		NE::ValueConstPtr nodeValue = uiNode.GetCalculatedValue ();
		if (nodeValue != nullptr) {
			nodeText = nodeValue->ToString (stringSettings);
		}
	}
	return nodeText;
}

std::shared_ptr<HeaderWithSlotsAndButtonsLayout::ClickHandler> NumericUpDownNode::Layout::GetClickHandler (BasicUINode& uiNode) const
{
	class ClickHandler : public HeaderWithSlotsAndButtonsLayout::ClickHandler
	{
	public:
		ClickHandler (NumericUpDownNode* node) :
			node (node)
		{
		}

		virtual void LeftButtonClicked () override
		{
			node->Decrease ();
		}

		virtual void RightButtonClicked () override
		{
			node->Increase ();
		}

	private:
		NumericUpDownNode* node;
	};

	return std::shared_ptr<HeaderWithSlotsAndButtonsLayout::ClickHandler> (new ClickHandler (dynamic_cast<NumericUpDownNode*> (&uiNode)));
}

NumericUpDownNode::NumericUpDownNode () :
	NumericUpDownNode (std::wstring (), NUIE::Point ())
{

}

NumericUpDownNode::NumericUpDownNode (const std::wstring& name, const NUIE::Point& position) :
	BasicUINode (name, position, NUIE::InvalidIconId, UINodeLayoutPtr (new Layout ("minus", NE::Localize (L"<"), "plus", NE::Localize (L">"))))
{

}

NumericUpDownNode::~NumericUpDownNode ()
{

}

void NumericUpDownNode::Initialize ()
{
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::Localize (L"Output"))));
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

IntegerUpDownNode::IntegerUpDownNode () :
	IntegerUpDownNode (std::wstring (), NUIE::Point (), 0, 0)
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
			NUIE::IntegerNodeParameter<IntegerUpDownNode> (NE::Localize (L"Value"))
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
			NUIE::IntegerNodeParameter<IntegerUpDownNode> (NE::Localize (L"Step"))
		{

		}

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::IntValue (GetTypedNode (uiNode)->GetStep ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			GetTypedNode (uiNode)->SetStep (NE::IntValue::Get (value));
			uiManager.InvalidateNodeValue (uiNode);
			uiManager.InvalidateNodeDrawing (uiNode);
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
}

void IntegerUpDownNode::Decrease ()
{
	val = val - step;
}

int IntegerUpDownNode::GetValue () const
{
	return val;
}

void IntegerUpDownNode::SetValue (int newValue)
{
	val = newValue;
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
	DoubleUpDownNode (std::wstring (), NUIE::Point (), 0.0, 0.0)
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
			NUIE::DoubleNodeParameter<DoubleUpDownNode> (NE::Localize (L"Value"))
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
			NUIE::DoubleNodeParameter<DoubleUpDownNode> (NE::Localize (L"Step"))
		{

		}

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::DoubleValue (GetTypedNode (uiNode)->GetStep ()));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			GetTypedNode (uiNode)->SetStep (NE::DoubleValue::Get (value));
			uiManager.InvalidateNodeValue (uiNode);
			uiManager.InvalidateNodeDrawing (uiNode);
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
}

void DoubleUpDownNode::Decrease ()
{
	val = val - step;
}

double DoubleUpDownNode::GetValue () const
{
	return val;
}

void DoubleUpDownNode::SetValue (double newValue)
{
	val = newValue;
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
	NumericRangeNode (std::wstring (), NUIE::Point ())
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
	IntegerIncrementedNode (std::wstring (), NUIE::Point ())
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
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("start"), NE::Localize (L"Start"), NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("step"), NE::Localize (L"Step"), NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("count"), NE::Localize (L"Count"), NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::Localize (L"List"))));
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
	NUIE::RegisterSlotDefaultValueNodeParameter<IntegerIncrementedNode, NE::IntValue> (parameterList, NE::Localize (L"Start"), NUIE::ParameterType::Integer, NE::SlotId ("start"));
	NUIE::RegisterSlotDefaultValueNodeParameter<IntegerIncrementedNode, NE::IntValue> (parameterList, NE::Localize (L"Step"), NUIE::ParameterType::Integer, NE::SlotId ("step"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new MinValueIntegerParameter<IntegerIncrementedNode> (NE::Localize (L"Count"), NUIE::ParameterType::Integer, NE::SlotId ("count"), 0)));
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
	DoubleIncrementedNode (std::wstring (), NUIE::Point ())
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
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("start"), NE::Localize (L"Start"), NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("step"), NE::Localize (L"Step"), NE::ValuePtr (new NE::DoubleValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("count"), NE::Localize (L"Count"), NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::Localize (L"List"))));
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
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleIncrementedNode, NE::DoubleValue> (parameterList, NE::Localize (L"Start"), NUIE::ParameterType::Double, NE::SlotId ("start"));
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleIncrementedNode, NE::DoubleValue> (parameterList, NE::Localize (L"Step"), NUIE::ParameterType::Double, NE::SlotId ("step"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new MinValueIntegerParameter<DoubleIncrementedNode> (NE::Localize (L"Count"), NUIE::ParameterType::Integer, NE::SlotId ("count"), 0)));
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
	DoubleDistributedNode (std::wstring (), NUIE::Point ())
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
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("start"), NE::Localize (L"Start"), NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("end"), NE::Localize (L"End"), NE::ValuePtr (new NE::DoubleValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("count"), NE::Localize (L"Count"), NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::Localize (L"List"))));
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

	double segmentVal = std::fabs (startNum - endNum) / (double) (countNum - 1);
	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countNum; ++i) {
		list->Push (NE::ValuePtr (new NE::DoubleValue (startNum + i * segmentVal)));
	}

	return list;
}

void DoubleDistributedNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleDistributedNode, NE::DoubleValue> (parameterList, NE::Localize (L"Start"), NUIE::ParameterType::Double, NE::SlotId ("start"));
	NUIE::RegisterSlotDefaultValueNodeParameter<DoubleDistributedNode, NE::DoubleValue> (parameterList, NE::Localize (L"End"), NUIE::ParameterType::Double, NE::SlotId ("end"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new MinValueIntegerParameter<DoubleDistributedNode> (NE::Localize (L"Count"), NUIE::ParameterType::Integer, NE::SlotId ("count"), 2)));
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

ListBuilderNode::ListBuilderNode () :
	ListBuilderNode (std::wstring (), NUIE::Point ())
{

}

ListBuilderNode::ListBuilderNode (const std::wstring& name, const NUIE::Point& position) :
	BasicUINode (name, position)
{

}

ListBuilderNode::~ListBuilderNode ()
{

}

void ListBuilderNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), NE::Localize (L"Input"), nullptr, NE::OutputSlotConnectionMode::Multiple)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::Localize (L"Output"))));
}

NE::ValueConstPtr ListBuilderNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr in = EvaluateInputSlot (NE::SlotId ("in"), env);
	if (in == nullptr) {
		return nullptr;
	}

	NE::ListValuePtr list (new NE::ListValue ());
	NE::FlatEnumerate (in, [&] (const NE::ValueConstPtr& innerVal) {
		list->Push (innerVal);
	});
	return list;
}

NE::Stream::Status ListBuilderNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ListBuilderNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

}
