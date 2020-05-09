#include "NUIE_UIInputSlot.hpp"

namespace NUIE
{

NE::DynamicSerializationInfo UIInputSlot::serializationInfo (NE::ObjectId ("{6573888B-EE86-49C8-9E18-02A18FF83274}"), NE::ObjectVersion (1), UIInputSlot::CreateSerializableInstance);

UIInputSlot::UIInputSlot () :
	UIInputSlot (NE::SlotId (), L"", nullptr, NE::OutputSlotConnectionMode::Single)
{

}

UIInputSlot::UIInputSlot (const NE::SlotId& id, const std::wstring& name, const NE::ValuePtr& defaultValue, NE::OutputSlotConnectionMode slotConnectionMode) :
	NE::InputSlot (id, defaultValue, slotConnectionMode),
	name (name)
{

}

UIInputSlot::~UIInputSlot ()
{

}

std::wstring UIInputSlot::GetName () const
{
	return name.GetLocalized ();
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
