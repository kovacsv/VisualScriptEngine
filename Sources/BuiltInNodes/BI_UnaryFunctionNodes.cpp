#include "BI_UnaryFunctionNodes.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeCommonParameters.hpp"

#include <cmath>

namespace BI
{

SERIALIZATION_INFO (UnaryFunctionNode, 1);
DYNAMIC_SERIALIZATION_INFO (AbsoluteNode, 1, "{125E8E5E-F1CB-4AE4-8EA9-53343ACD193B}");
DYNAMIC_SERIALIZATION_INFO (CeilNode, 1, "{60B0DFF2-2718-46A1-B7D5-AA614BF21FDD}");
DYNAMIC_SERIALIZATION_INFO (FloorNode, 1, "{0DB3D5E3-8B32-43A4-82D2-F5B816AB5CC1}");

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

AbsoluteNode::AbsoluteNode () :
	UnaryFunctionNode ()
{

}

AbsoluteNode::AbsoluteNode (const NE::LocString& name, const NUIE::Point& position) :
	UnaryFunctionNode (name, position)
{

}

AbsoluteNode::~AbsoluteNode ()
{

}

double AbsoluteNode::DoOperation (double a) const
{
	return std::abs (a);
}

FloorNode::FloorNode () :
	UnaryFunctionNode ()
{

}

FloorNode::FloorNode (const NE::LocString& name, const NUIE::Point& position) :
	UnaryFunctionNode (name, position)
{

}

FloorNode::~FloorNode ()
{

}

double FloorNode::DoOperation (double a) const
{
	return std::floor (a);
}

CeilNode::CeilNode () :
	UnaryFunctionNode ()
{

}

CeilNode::CeilNode (const NE::LocString& name, const NUIE::Point& position) :
	UnaryFunctionNode (name, position)
{

}

CeilNode::~CeilNode ()
{

}

double CeilNode::DoOperation (double a) const
{
	return std::ceil (a);
}

}
