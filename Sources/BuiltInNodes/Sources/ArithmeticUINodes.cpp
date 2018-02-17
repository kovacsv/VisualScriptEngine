#include "ArithmeticUINodes.hpp"
#include "UINodeCommonParameters.hpp"

namespace NUIE
{

NE::DynamicSerializationInfo	AdditionNode::serializationInfo (NE::ObjectId ("{B54F8625-C6B0-4570-9166-220207C84312}"), NE::ObjectVersion (1), AdditionNode::CreateSerializableInstance);

AdditionNode::AdditionNode () :
	HeaderWithSlotsUINode ()
{

}

AdditionNode::AdditionNode (const std::wstring& name, const Point& position) :
	HeaderWithSlotsUINode (name, position)
{

}

AdditionNode::~AdditionNode ()
{

}

void AdditionNode::RegisterSlots ()
{
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("a"), L"A", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("b"), L"B", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("result"), L"Result")));
}

NE::ValuePtr AdditionNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr aValue = EvaluateSingleInputSlot (NE::SlotId ("a"), env);
	NE::ValuePtr bValue = EvaluateSingleInputSlot (NE::SlotId ("b"), env);
	if (!NE::Value::IsType<NE::NumberValue> (aValue) || !NE::Value::IsType<NE::NumberValue> (bValue)) {
		return nullptr;
	}

	double aDouble = NE::NumberValue::ToDouble (aValue);
	double bDouble = NE::NumberValue::ToDouble (bValue);
	return NE::ValuePtr (new NE::DoubleValue (aDouble + bDouble));
}

void AdditionNode::RegisterParameters (NodeParameterList& parameterList) const
{
	HeaderWithSlotsUINode::RegisterParameters (parameterList);
	RegisterSlotDefaultValueParameter<AdditionNode, NE::DoubleValue> (parameterList, "AdditionNodeAParameter", L"A", ParameterType::Double, NE::SlotId ("a"));
	RegisterSlotDefaultValueParameter<AdditionNode, NE::DoubleValue> (parameterList, "AdditionNodeBParameter", L"B", ParameterType::Double, NE::SlotId ("b"));
}

NE::Stream::Status AdditionNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	HeaderWithSlotsUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status AdditionNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	HeaderWithSlotsUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

}
