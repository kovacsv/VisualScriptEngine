#include "NUIE_UIOutputSlot.hpp"

namespace NUIE
{

NE::DynamicSerializationInfo UIOutputSlot::serializationInfo (NE::ObjectId ("{F5EB36BD-8FB2-4887-8E4A-5230022B29C1}"), NE::ObjectVersion (1), UIOutputSlot::CreateSerializableInstance);

UIOutputSlot::UIOutputSlot () :
	NE::OutputSlot (),
	name ()
{

}

UIOutputSlot::UIOutputSlot (const NE::SlotId& id, const std::wstring& name) :
	NE::OutputSlot (id),
	name (name)
{

}

UIOutputSlot::~UIOutputSlot ()
{

}

const std::wstring& UIOutputSlot::GetName () const
{
	return name;
}

void UIOutputSlot::SetName (const std::wstring& newName)
{
	name = newName;
}

void UIOutputSlot::RegisterCommands (OutputSlotCommandRegistrator&)
{

}

NE::Stream::Status UIOutputSlot::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	OutputSlot::Read (inputStream);
	inputStream.Read (name);
	return inputStream.GetStatus ();
}

NE::Stream::Status UIOutputSlot::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	OutputSlot::Write (outputStream);
	outputStream.Write (name);
	return outputStream.GetStatus ();
}

}
