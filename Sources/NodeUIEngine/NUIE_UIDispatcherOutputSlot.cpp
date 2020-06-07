#include "NUIE_UIDispatcherOutputSlot.hpp"

namespace NUIE
{

DYNAMIC_SERIALIZATION_INFO (UIDispatcherOutputSlot, 1, "{F6F046E1-7809-4C26-A1A7-1E68C3CCCFED}");

UIDispatcherOutputSlot::UIDispatcherOutputSlot () :
	UIOutputSlot (),
	listIndex (0)
{

}

UIDispatcherOutputSlot::UIDispatcherOutputSlot (const NE::SlotId& id, const NE::LocString& name, size_t listIndex) :
	UIOutputSlot (id, name),
	listIndex (listIndex)
{

}

UIDispatcherOutputSlot::~UIDispatcherOutputSlot ()
{

}

NE::ValueConstPtr UIDispatcherOutputSlot::Evaluate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr value = EvaluateOwnerNode (env);
	if (value == nullptr) {
		return nullptr;
	}
	if (DBGERROR (!NE::Value::IsType<NE::ListValue> (value))) {
		return nullptr;
	}
	NE::ListValueConstPtr listValue = NE::Value::Cast<NE::ListValue> (value);
	if (DBGERROR (listIndex > listValue->GetSize ())) {
		return nullptr;
	}
	return listValue->GetValue (listIndex);
}

NE::Stream::Status UIDispatcherOutputSlot::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UIOutputSlot::Read (inputStream);
	inputStream.Read (listIndex);
	return inputStream.GetStatus ();
}

NE::Stream::Status UIDispatcherOutputSlot::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UIOutputSlot::Write (outputStream);
	outputStream.Write (listIndex);
	return outputStream.GetStatus ();
}

}
