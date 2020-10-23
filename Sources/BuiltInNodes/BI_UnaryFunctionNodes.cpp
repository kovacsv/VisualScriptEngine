#include "BI_UnaryFunctionNodes.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeCommonParameters.hpp"

#include <cmath>

namespace BI
{

SERIALIZATION_INFO (UnaryFunctionNode, 1);
DYNAMIC_SERIALIZATION_INFO (AbsoluteValueNode, 1, "{125E8E5E-F1CB-4AE4-8EA9-53343ACD193B}");

UnaryFunctionNode::UnaryFunctionNode () :
	UnaryFunctionNode (NE::LocString (), NUIE::Point ())
{

}

UnaryFunctionNode::UnaryFunctionNode (const NE::LocString& name, const NUIE::Point& position) :
	BasicUINode (name, position)
{
}

UnaryFunctionNode::~UnaryFunctionNode ()
{

}

void UnaryFunctionNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), NE::LocString (L"A"), NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), NE::LocString (L"Result"))));
}

NE::ValueConstPtr UnaryFunctionNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
	if (!NE::IsComplexType<NE::NumberValue> (aValue)) {
		return nullptr;
	}

	if (NE::IsSingleValue (aValue)) {
		return DoSingleOperation (aValue);
	} else {
		NE::ListValuePtr resultListValue (new NE::ListValue ());
		bool isValid = NE::FlatEnumerate (aValue, [&] (const NE::ValueConstPtr& val) {
			NE::ValuePtr result = DoSingleOperation (val);
			if (result == nullptr) {
				return false;
			}
			resultListValue->Push (result);
			return true;
		});
		if (!isValid) {
			return nullptr;
		}
		return resultListValue;
	}
}

void UnaryFunctionNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<UnaryFunctionNode, NE::DoubleValue> (parameterList, NE::SlotId ("a"), NE::LocString (L"A"), NUIE::ParameterType::Double);
}

bool UnaryFunctionNode::IsForceCalculated () const
{
	return true;
}

NE::Stream::Status UnaryFunctionNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status UnaryFunctionNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr UnaryFunctionNode::DoSingleOperation (const NE::ValueConstPtr& aValue) const
{
	double aDouble = NE::NumberValue::ToDouble (aValue);
	double result = DoOperation (aDouble);
	if (std::isnan (result) || std::isinf (result)) {
		return nullptr;
	}
	return NE::ValuePtr (new NE::DoubleValue (result));
}

AbsoluteValueNode::AbsoluteValueNode () :
	UnaryFunctionNode ()
{

}

AbsoluteValueNode::AbsoluteValueNode (const NE::LocString& name, const NUIE::Point& position) :
	UnaryFunctionNode (name, position)
{

}

AbsoluteValueNode::~AbsoluteValueNode ()
{

}

double AbsoluteValueNode::DoOperation (double a) const
{
	return std::abs (a);
}

}
