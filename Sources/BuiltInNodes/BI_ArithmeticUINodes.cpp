#include "BI_ArithmeticUINodes.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeCommonParameters.hpp"

#include <cmath>

namespace BI
{

SERIALIZATION_INFO (BinaryOperationNode, 1);
DYNAMIC_SERIALIZATION_INFO (AdditionNode, 1, "{1A72C230-3D90-42AD-835A-43306E641EA2}");
DYNAMIC_SERIALIZATION_INFO (SubtractionNode, 1, "{80CACB59-C3E6-441B-B60C-37A6F2611FC2}");
DYNAMIC_SERIALIZATION_INFO (MultiplicationNode, 1, "{75F39B99-8296-4D79-8BB7-418D55F93C25}");
DYNAMIC_SERIALIZATION_INFO (DivisionNode, 1, "{652DDDFC-A441-40B1-87AC-0BED247F35E7}");

BinaryOperationNode::BinaryOperationNode () :
	BinaryOperationNode (NE::LocString (), NUIE::Point ())
{

}

BinaryOperationNode::BinaryOperationNode (const NE::LocString& name, const NUIE::Point& position) :
	BasicUINode (name, position)
{
}

BinaryOperationNode::~BinaryOperationNode ()
{

}

void BinaryOperationNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), NE::LocString (L"A"), NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), NE::LocString (L"B"), NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), NE::LocString (L"Result"))));
	RegisterFeature (NodeFeaturePtr (new ValueCombinationFeature (NE::ValueCombinationMode::Longest)));
}

NE::ValueConstPtr BinaryOperationNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
	NE::ValueConstPtr bValue = EvaluateInputSlot (NE::SlotId ("b"), env);
	if (!NE::IsComplexType<NE::NumberValue> (aValue) || !NE::IsComplexType<NE::NumberValue> (bValue)) {
		return nullptr;
	}

	NE::ListValuePtr resultListValue (new NE::ListValue ());
	std::shared_ptr<ValueCombinationFeature> valueCombination = GetValueCombinationFeature (this);
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
	NUIE::RegisterSlotDefaultValueNodeParameter<BinaryOperationNode, NE::DoubleValue> (parameterList, NE::SlotId ("a"), NE::LocString (L"A"), NUIE::ParameterType::Double);
	NUIE::RegisterSlotDefaultValueNodeParameter<BinaryOperationNode, NE::DoubleValue> (parameterList, NE::SlotId ("b"), NE::LocString (L"B"), NUIE::ParameterType::Double);
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

AdditionNode::AdditionNode (const NE::LocString& name, const NUIE::Point& position) :
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

SubtractionNode::SubtractionNode (const NE::LocString& name, const NUIE::Point& position) :
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

MultiplicationNode::MultiplicationNode (const NE::LocString& name, const NUIE::Point& position) :
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

DivisionNode::DivisionNode (const NE::LocString& name, const NUIE::Point& position) :
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
