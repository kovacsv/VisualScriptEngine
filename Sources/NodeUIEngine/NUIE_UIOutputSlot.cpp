#include "NUIE_UIOutputSlot.hpp"

namespace NUIE
{

DYNAMIC_SERIALIZATION_INFO (UIOutputSlot, 1, "{F5EB36BD-8FB2-4887-8E4A-5230022B29C1}");

UIOutputSlot::UIOutputSlot () :
	UIOutputSlot (NE::SlotId (), NE::LocString ())
{

}

UIOutputSlot::UIOutputSlot (const NE::SlotId& id, const NE::LocString& name) :
	NE::OutputSlot (id),
	name (name)
{

}

UIOutputSlot::~UIOutputSlot ()
{

}

const NE::LocString& UIOutputSlot::GetName () const
{
	return name;
}

void UIOutputSlot::SetName (const std::wstring& newName)
{
	name.SetCustom (newName);
}

void UIOutputSlot::RegisterCommands (OutputSlotCommandRegistrator&) const
{

}

NE::Stream::Status UIOutputSlot::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	OutputSlot::Read (inputStream);
	name.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status UIOutputSlot::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	OutputSlot::Write (outputStream);
	name.Write (outputStream);
	return outputStream.GetStatus ();
}

}
