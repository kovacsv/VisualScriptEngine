#include "InputSlot.hpp"
#include "Node.hpp"
#include "NodeManager.hpp"
#include "Debug.hpp"

namespace NE
{

DynamicSerializationInfo InputSlot::serializationInfo (ObjectId ("{59CFA81D-588F-40EF-A16B-0E57C55AA7E6}"), ObjectVersion (1), InputSlot::CreateSerializableInstance);

InputSlot::InputSlot () :
	Slot ()
{

}

InputSlot::InputSlot (const SlotId& id, const ValuePtr& defaultValue, OutputSlotConnectionMode outputSlotConnectionMode) :
	Slot (id),
	defaultValue (defaultValue),
	outputSlotConnectionMode (outputSlotConnectionMode)
{

}

InputSlot::~InputSlot ()
{

}

OutputSlotConnectionMode InputSlot::GetOutputSlotConnectionMode () const
{
	return outputSlotConnectionMode;
}

ValuePtr InputSlot::GetDefaultValue () const
{
	return defaultValue;
}

void InputSlot::SetDefaultValue (const ValuePtr& newDefaultValue)
{
	defaultValue = newDefaultValue;
	if (ownerNode != nullptr) {
		ownerNode->InvalidateValue ();
	}
}

Stream::Status InputSlot::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	Slot::Read (inputStream);

	bool hasDefaultValue = false;
	inputStream.Read (hasDefaultValue);
	if (hasDefaultValue) {
		defaultValue.reset (ReadDynamicObject<Value> (inputStream));
	}
	int outputSlotConnectionModeInt;
	inputStream.Read (outputSlotConnectionModeInt);
	outputSlotConnectionMode = (OutputSlotConnectionMode) outputSlotConnectionModeInt;

	return inputStream.GetStatus ();
}

Stream::Status InputSlot::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	Slot::Write (outputStream);

	bool hasDefaultValue = (defaultValue != nullptr);
	outputStream.Write (hasDefaultValue);
	if (hasDefaultValue) {
		WriteDynamicObject (outputStream, defaultValue.get ());
	}
	outputStream.Write ((int) outputSlotConnectionMode);

	return outputStream.GetStatus ();
}

}
