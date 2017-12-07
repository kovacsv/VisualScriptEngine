#include "UIInputSlot.hpp"
#include "UINodeCommands.hpp"
#include "NodeUIManager.hpp"

namespace NUIE
{

NE::DynamicSerializationInfo UIInputSlot::serializationInfo (NE::ObjectId ("{6573888B-EE86-49C8-9E18-02A18FF83274}"), NE::ObjectVersion (1), UIInputSlot::CreateSerializableInstance);

class DisconnectInputSlotCommand : public InputSlotCommand
{
public:
	DisconnectInputSlotCommand (const std::wstring& name, const UIOutputSlotConstPtr& slotToDisconnect) :
		InputSlotCommand (name, false),
		slotToDisconnect (slotToDisconnect)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIInputSlotPtr& inputSlot) override
	{
		uiManager.DisconnectOutputSlotFromInputSlot (slotToDisconnect, inputSlot);
	}

private:
	UIOutputSlotConstPtr slotToDisconnect;
};

UIInputSlot::UIInputSlot () :
	NE::InputSlot (),
	name ()
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

const std::wstring& UIInputSlot::GetName () const
{
	return name;
}

void UIInputSlot::SetName (const std::wstring& newName)
{
	name = newName;
}

void UIInputSlot::RegisterCommands (InputSlotCommandRegistrator& commandRegistrator)
{
	InputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<InputSlotCommandPtr> (L"Disconnect"));
	bool foundConnectedSlot = false;
	commandRegistrator.EnumerateConnectedNodes ([&] (UINodeConstPtr uiNode, UIOutputSlotConstPtr outputSlot) {
		disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectInputSlotCommand (uiNode->GetNodeName (), outputSlot)));
		foundConnectedSlot = true;
	});
	if (foundConnectedSlot) {
		commandRegistrator.RegisterSlotGroupCommand (disconnectGroup);
	}
}

NE::Stream::Status UIInputSlot::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	InputSlot::Read (inputStream);
	inputStream.Read (name);
	return inputStream.GetStatus ();
}

NE::Stream::Status UIInputSlot::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	InputSlot::Write (outputStream);
	outputStream.Write (name);
	return outputStream.GetStatus ();
}

}
