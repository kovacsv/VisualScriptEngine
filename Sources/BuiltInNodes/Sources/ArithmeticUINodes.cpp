#include "ArithmeticUINodes.hpp"
#include "UINodeCommonParameters.hpp"

namespace NUIE
{

NE::SerializationInfo			BinaryOperationNode::serializationInfo (NE::ObjectId ("{B54F8625-C6B0-4570-9166-220207C84312}"), NE::ObjectVersion (1));
NE::DynamicSerializationInfo	AdditionNode::serializationInfo (NE::ObjectId ("{1A72C230-3D90-42AD-835A-43306E641EA2}"), NE::ObjectVersion (1), AdditionNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	SubtractionNode::serializationInfo (NE::ObjectId ("{80CACB59-C3E6-441B-B60C-37A6F2611FC2}"), NE::ObjectVersion (1), SubtractionNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	MultiplicationNode::serializationInfo (NE::ObjectId ("{75F39B99-8296-4D79-8BB7-418D55F93C25}"), NE::ObjectVersion (1), MultiplicationNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	DivisionNode::serializationInfo (NE::ObjectId ("{652DDDFC-A441-40B1-87AC-0BED247F35E7}"), NE::ObjectVersion (1), DivisionNode::CreateSerializableInstance);

BinaryOperationNode::BinaryOperationNode () :
	HeaderWithSlotsUINode ()
{

}

BinaryOperationNode::BinaryOperationNode (const std::wstring& name, const Point& position) :
	HeaderWithSlotsUINode (name, position)
{

}

BinaryOperationNode::~BinaryOperationNode ()
{

}

void BinaryOperationNode::RegisterSlots ()
{
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("a"), L"A", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("b"), L"B", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("result"), L"Result")));
}

NE::ValuePtr BinaryOperationNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr aValue = EvaluateSingleInputSlot (NE::SlotId ("a"), env);
	NE::ValuePtr bValue = EvaluateSingleInputSlot (NE::SlotId ("b"), env);
	if (!NE::Value::IsType<NE::NumberValue> (aValue) || !NE::Value::IsType<NE::NumberValue> (bValue)) {
		return nullptr;
	}

	double aDouble = NE::NumberValue::ToDouble (aValue);
	double bDouble = NE::NumberValue::ToDouble (bValue);
	double result = DoOperation (aDouble, bDouble);
	if (std::isnan (result)) {
		return nullptr;
	}

	return NE::ValuePtr (new NE::DoubleValue (result));
}

void BinaryOperationNode::RegisterParameters (NodeParameterList& parameterList) const
{
	HeaderWithSlotsUINode::RegisterParameters (parameterList);
	RegisterSlotDefaultValueParameter<BinaryOperationNode, NE::DoubleValue> (parameterList, "BinaryOperationNodeAParameter", L"A", ParameterType::Double, NE::SlotId ("a"));
	RegisterSlotDefaultValueParameter<BinaryOperationNode, NE::DoubleValue> (parameterList, "BinaryOperationNodeBParameter", L"B", ParameterType::Double, NE::SlotId ("b"));
}

NE::Stream::Status BinaryOperationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	HeaderWithSlotsUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status BinaryOperationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	HeaderWithSlotsUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

AdditionNode::AdditionNode () :
	BinaryOperationNode ()
{

}

AdditionNode::AdditionNode (const std::wstring& name, const Point& position) :
	BinaryOperationNode (name, position)
{

}

AdditionNode::~AdditionNode ()
{

}

double AdditionNode::DoOperation (double a, double b) const
{
	return a + b;
}

SubtractionNode::SubtractionNode () :
	BinaryOperationNode ()
{

}

SubtractionNode::SubtractionNode (const std::wstring& name, const Point& position) :
	BinaryOperationNode (name, position)
{

}

SubtractionNode::~SubtractionNode ()
{

}

double SubtractionNode::DoOperation (double a, double b) const
{
	return a - b;
}

MultiplicationNode::MultiplicationNode () :
	BinaryOperationNode ()
{

}

MultiplicationNode::MultiplicationNode (const std::wstring& name, const Point& position) :
	BinaryOperationNode (name, position)
{

}

MultiplicationNode::~MultiplicationNode ()
{

}

double MultiplicationNode::DoOperation (double a, double b) const
{
	return a * b;
}

DivisionNode::DivisionNode () :
	BinaryOperationNode ()
{

}

DivisionNode::DivisionNode (const std::wstring& name, const Point& position) :
	BinaryOperationNode (name, position)
{

}

DivisionNode::~DivisionNode ()
{

}

double DivisionNode::DoOperation (double a, double b) const
{
	return a / b;
}

}
