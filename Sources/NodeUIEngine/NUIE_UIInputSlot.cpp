#include "NUIE_UIInputSlot.hpp"

namespace NUIE
{

DYNAMIC_SERIALIZATION_INFO (UIInputSlot, 1, "{6573888B-EE86-49C8-9E18-02A18FF83274}");

UIInputSlot::UIInputSlot () :
	UIInputSlot (NE::SlotId (), NE::LocString (), nullptr, NE::OutputSlotConnectionMode::Single)
{

}

UIInputSlot::UIInputSlot (const NE::SlotId& id, const NE::LocString& name, const NE::ValuePtr& defaultValue, NE::OutputSlotConnectionMode slotConnectionMode) :
	NE::InputSlot (id, defaultValue, slotConnectionMode),
	name (name)
{

}

UIInputSlot::~UIInputSlot ()
{

}

const NE::LocString& UIInputSlot::GetName () const
{
	return name;
}

void UIInputSlot::SetName (const std::wstring& newName)
{
	name.SetCustom (newName);
}

void UIInputSlot::RegisterCommands (InputSlotCommandRegistrator&) const
{

}

NE::Stream::Status UIInputSlot::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	InputSlot::Read (inputStream);
	name.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status UIInputSlot::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	InputSlot::Write (outputStream);
	name.Write (outputStream);
	return outputStream.GetStatus ();
}

}
