#include "NUIE_UIInputSlot.hpp"
#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_FeatureSet.hpp"

namespace NUIE
{

DYNAMIC_SERIALIZATION_INFO (UIInputSlot, 2, "{6573888B-EE86-49C8-9E18-02A18FF83274}");

UIInputSlot::UIInputSlot () :
	UIInputSlot (NE::SlotId (), NE::LocString (), nullptr, NE::OutputSlotConnectionMode::Single)
{

}

UIInputSlot::UIInputSlot (const NE::SlotId& id, const NE::LocString& name, const NE::ValuePtr& defaultValue, NE::OutputSlotConnectionMode slotConnectionMode) :
	NE::InputSlot (id, defaultValue, slotConnectionMode),
	name (name),
	connDisplayMode (ConnectionDisplayMode::Normal)
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

NUIE::ConnectionDisplayMode UIInputSlot::GetConnectionDisplayMode () const
{
	return connDisplayMode;
}

void UIInputSlot::SetConnectionDisplayMode (ConnectionDisplayMode newConnectionDisplayMode)
{
	connDisplayMode = newConnectionDisplayMode;
}

void UIInputSlot::RegisterCommands (InputSlotCommandRegistrator& commandRegistrator) const
{
	class SetConnectionDisplayModeCommand : public InputSlotCommand
	{
	public:
		SetConnectionDisplayModeCommand (const NE::LocString& name, bool isChecked, ConnectionDisplayMode connDisplayMode) :
			InputSlotCommand (name, isChecked),
			connDisplayMode (connDisplayMode)
		{

		}

		virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UIInputSlotPtr& inputSlot) override
		{
			SetInputSlotConnectionDisplayModeCommand command (inputSlot, connDisplayMode);
			uiManager.ExecuteCommand (command, uiEnvironment);
		}

	private:
		ConnectionDisplayMode connDisplayMode;
	};

	if (IsFeatureEnabled (Feature::ConnectionDisplay)) {
		InputSlotGroupCommandPtr connectionDisplayGroupCommand (new NodeGroupCommand<InputSlotCommandPtr> (NE::LocString (L"Connection Display")));
		connectionDisplayGroupCommand->AddChildCommand (InputSlotCommandPtr (new SetConnectionDisplayModeCommand (NE::LocString (L"Normal"), connDisplayMode == ConnectionDisplayMode::Normal, ConnectionDisplayMode::Normal)));
		connectionDisplayGroupCommand->AddChildCommand (InputSlotCommandPtr (new SetConnectionDisplayModeCommand (NE::LocString (L"Hidden"), connDisplayMode == ConnectionDisplayMode::Hidden, ConnectionDisplayMode::Hidden)));
		commandRegistrator.RegisterSlotGroupCommand (connectionDisplayGroupCommand);
	}
}

NE::Stream::Status UIInputSlot::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	InputSlot::Read (inputStream);
	name.Read (inputStream);
	if (header.GetVersion () > 1) {
		NE::ReadEnum<ConnectionDisplayMode> (inputStream, connDisplayMode);
	}
	return inputStream.GetStatus ();
}

NE::Stream::Status UIInputSlot::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	InputSlot::Write (outputStream);
	name.Write (outputStream);
	NE::WriteEnum<ConnectionDisplayMode> (outputStream, connDisplayMode);
	return outputStream.GetStatus ();
}

}
