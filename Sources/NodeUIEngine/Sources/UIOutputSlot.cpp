#include "UIOutputSlot.hpp"
#include "UINodeCommands.hpp"
#include "NodeUIManager.hpp"

namespace NUIE
{

NE::DynamicSerializationInfo UIOutputSlot::serializationInfo (NE::ObjectId ("{F5EB36BD-8FB2-4887-8E4A-5230022B29C1}"), NE::ObjectVersion (1), UIOutputSlot::CreateSerializableInstance);

class DisconnectOutputSlotCommand : public OutputSlotCommand
{
public:
	DisconnectOutputSlotCommand (const std::wstring& name, const UIInputSlotConstPtr& slotToDisconnect) :
		OutputSlotCommand (name, false),
		slotToDisconnect (slotToDisconnect)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIOutputSlotPtr& outputSlot) override
	{
		uiManager.DisconnectOutputSlotFromInputSlot (outputSlot, slotToDisconnect);
	}

private:
	UIInputSlotConstPtr slotToDisconnect;
};

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

void UIOutputSlot::RegisterCommands (OutputSlotCommandRegistrator& commandRegistrator)
{
	OutputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<OutputSlotCommandPtr> (L"Disconnect"));
	bool foundConnectedSlot = false;
	commandRegistrator.EnumerateConnectedNodes ([&] (UINodeConstPtr uiNode, UIInputSlotConstPtr inputSlot) {
		disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectOutputSlotCommand (uiNode->GetNodeName (), inputSlot)));
		foundConnectedSlot = true;
	});
	if (foundConnectedSlot) {
		commandRegistrator.RegisterSlotGroupCommand (disconnectGroup);
	}
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
