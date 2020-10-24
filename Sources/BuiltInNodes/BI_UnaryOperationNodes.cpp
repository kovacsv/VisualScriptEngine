#include "BI_UnaryOperationNodes.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeCommonParameters.hpp"

#include <cmath>

namespace BI
{

SERIALIZATION_INFO (UnaryOperationNode, 1);
DYNAMIC_SERIALIZATION_INFO (AbsoluteNode, 1, "{125E8E5E-F1CB-4AE4-8EA9-53343ACD193B}");
DYNAMIC_SERIALIZATION_INFO (CeilNode, 1, "{60B0DFF2-2718-46A1-B7D5-AA614BF21FDD}");
DYNAMIC_SERIALIZATION_INFO (FloorNode, 1, "{0DB3D5E3-8B32-43A4-82D2-F5B816AB5CC1}");

UnaryOperationNode::UnaryOperationNode () :
	UnaryOperationNode (NE::LocString (), NUIE::Point ())
{

}

UnaryOperationNode::UnaryOperationNode (const NE::LocString& name, const NUIE::Point& position) :
	BasicUINode (name, position)
{
}

UnaryOperationNode::~UnaryOperationNode ()
{

}

void UnaryOperationNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), NE::LocString (L"A"), NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), NE::LocString (L"Result"))));
}

NE::ValueConstPtr UnaryOperationNode::Calculate (NE::EvaluationEnv& env) const
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

void UnaryOperationNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<UnaryOperationNode, NE::DoubleValue> (parameterList, NE::SlotId ("a"), NE::LocString (L"A"), NUIE::ParameterType::Double);
}

bool UnaryOperationNode::IsForceCalculated () const
{
	return true;
}

NE::Stream::Status UnaryOperationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status UnaryOperationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr UnaryOperationNode::DoSingleOperation (const NE::ValueConstPtr& aValue) const
{
	double aDouble = NE::NumberValue::ToDouble (aValue);
	double result = DoOperation (aDouble);
	if (std::isnan (result) || std::isinf (result)) {
		return nullptr;
	}
	return NE::ValuePtr (new NE::DoubleValue (result));
}

AbsoluteNode::AbsoluteNode () :
	UnaryOperationNode ()
{

}

AbsoluteNode::AbsoluteNode (const NE::LocString& name, const NUIE::Point& position) :
	UnaryOperationNode (name, position)
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
	UnaryOperationNode ()
{

}

FloorNode::FloorNode (const NE::LocString& name, const NUIE::Point& position) :
	UnaryOperationNode (name, position)
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
	UnaryOperationNode ()
{

}

CeilNode::CeilNode (const NE::LocString& name, const NUIE::Point& position) :
	UnaryOperationNode (name, position)
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
