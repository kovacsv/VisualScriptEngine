#include "BI_ArithmeticUINodes.hpp"
#include "NUIE_UINodeCommonParameters.hpp"

#include <cmath>

namespace BI
{

NE::SerializationInfo			BinaryOperationNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	AdditionNode::serializationInfo (NE::ObjectId ("{1A72C230-3D90-42AD-835A-43306E641EA2}"), NE::ObjectVersion (1), AdditionNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	SubtractionNode::serializationInfo (NE::ObjectId ("{80CACB59-C3E6-441B-B60C-37A6F2611FC2}"), NE::ObjectVersion (1), SubtractionNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	MultiplicationNode::serializationInfo (NE::ObjectId ("{75F39B99-8296-4D79-8BB7-418D55F93C25}"), NE::ObjectVersion (1), MultiplicationNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	DivisionNode::serializationInfo (NE::ObjectId ("{652DDDFC-A441-40B1-87AC-0BED247F35E7}"), NE::ObjectVersion (1), DivisionNode::CreateSerializableInstance);

BinaryOperationNode::BinaryOperationNode () :
	BinaryOperationNode (L"", NUIE::Point ())
{

}

BinaryOperationNode::BinaryOperationNode (const std::wstring& name, const NUIE::Point& position) :
	BasicUINode (name, position)
{
}

BinaryOperationNode::~BinaryOperationNode ()
{

}

void BinaryOperationNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), L"A", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), L"B", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), L"Result")));
	RegisterFeature (NUIE::UINodeFeaturePtr (new BI::ValueCombinationFeature (NE::ValueCombinationMode::Longest)));
}

NE::ValuePtr BinaryOperationNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr aValue = EvaluateSingleInputSlot (NE::SlotId ("a"), env);
	NE::ValuePtr bValue = EvaluateSingleInputSlot (NE::SlotId ("b"), env);
	if (!NE::IsComplexType<NE::NumberValue> (aValue) || !NE::IsComplexType<NE::NumberValue> (bValue)) {
		return nullptr;
	}

	std::shared_ptr<ValueCombinationFeature> valueCombination = GetValueCombinationFeature (this);

	NE::ListValuePtr resultListValue (new NE::ListValue ());
	bool isValid = valueCombination->CombineValues ({aValue, bValue}, [&] (const NE::ValueCombination& combination) {
		double aDouble = NE::NumberValue::ToDouble (combination.GetValue (0));
		double bDouble = NE::NumberValue::ToDouble (combination.GetValue (1));
		double result = DoOperation (aDouble, bDouble);
		if (std::isnan (result) || std::isinf (result)) {
			return false;
		}
		resultListValue->Push (NE::ValuePtr (new NE::DoubleValue (result)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return resultListValue;
}

void BinaryOperationNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<BinaryOperationNode, NE::DoubleValue> (parameterList, L"A", NUIE::ParameterType::Double, NE::SlotId ("a"));
	NUIE::RegisterSlotDefaultValueNodeParameter<BinaryOperationNode, NE::DoubleValue> (parameterList, L"B", NUIE::ParameterType::Double, NE::SlotId ("b"));
}

bool BinaryOperationNode::IsForceCalculated () const
{
	return true;
}

NE::Stream::Status BinaryOperationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status BinaryOperationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

AdditionNode::AdditionNode () :
	BinaryOperationNode ()
{

}

AdditionNode::AdditionNode (const std::wstring& name, const NUIE::Point& position) :
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

SubtractionNode::SubtractionNode (const std::wstring& name, const NUIE::Point& position) :
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

MultiplicationNode::MultiplicationNode (const std::wstring& name, const NUIE::Point& position) :
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

DivisionNode::DivisionNode (const std::wstring& name, const NUIE::Point& position) :
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
