#include "NE_InputSlot.hpp"
#include "NE_Node.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Debug.hpp"

namespace NE
{

DYNAMIC_SERIALIZATION_INFO (InputSlot, 1, "{59CFA81D-588F-40EF-A16B-0E57C55AA7E6}");

InputSlot::InputSlot () :
	Slot (),
	defaultValue (nullptr),
	outputSlotConnectionMode (OutputSlotConnectionMode::Disabled)
{

}

InputSlot::InputSlot (const SlotId& id, const ValueConstPtr& defaultValue, OutputSlotConnectionMode outputSlotConnectionMode) :
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

ValueConstPtr InputSlot::GetDefaultValue () const
{
	return defaultValue;
}

void InputSlot::SetDefaultValue (const ValueConstPtr& newDefaultValue)
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
	ReadEnum (inputStream, outputSlotConnectionMode);

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
	WriteEnum (outputStream, outputSlotConnectionMode);

	return outputStream.GetStatus ();
}

}
