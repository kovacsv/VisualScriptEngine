#include "UINodeCommands.hpp"
#include "NodeUIManager.hpp"
#include "UIItemFinder.hpp"
#include "Debug.hpp"

namespace NUIE
{

NodeCommandBase::NodeCommandBase (const std::wstring& name, bool isChecked) :
	name (name),
	isChecked (isChecked)
{

}

NodeCommandBase::~NodeCommandBase ()
{

}

const std::wstring& NodeCommandBase::GetName () const
{
	return name;
}

bool NodeCommandBase::IsChecked () const
{
	return isChecked;
}

NodeCommand::NodeCommand (const std::wstring& name, bool isChecked) :
	NodeCommandBase (name, isChecked)
{

}

NodeCommand::~NodeCommand ()
{

}

InputSlotCommand::InputSlotCommand (const std::wstring& name, bool isChecked) :
	NodeCommandBase (name, isChecked)
{

}

InputSlotCommand::~InputSlotCommand ()
{

}

OutputSlotCommand::OutputSlotCommand (const std::wstring& name, bool isChecked) :
	NodeCommandBase (name, isChecked)
{

}

OutputSlotCommand::~OutputSlotCommand ()
{

}

InputSlotCommandRegistrator::InputSlotCommandRegistrator ()
{

}

InputSlotCommandRegistrator::~InputSlotCommandRegistrator ()
{

}

NodeCommandRegistrator::NodeCommandRegistrator ()
{

}

NodeCommandRegistrator::~NodeCommandRegistrator ()
{

}

OutputSlotCommandRegistrator::OutputSlotCommandRegistrator ()
{

}

OutputSlotCommandRegistrator::~OutputSlotCommandRegistrator ()
{

}

template <typename SlotType, typename CommandType>
class SlotCommand : public SingleCommand
{
public:
	SlotCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, SlotType& slot, CommandType& command) :
		SingleCommand (name, command->IsChecked ()),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		slot (slot),
		command (command)
	{

	}

	virtual ~SlotCommand ()
	{

	}

	virtual void Do () override
	{
		if (DBGERROR (command == nullptr)) {
			return;
		}
		command->Do (uiManager, uiEnvironment, slot);
	}

private:
	NodeUIManager&				uiManager;
	NodeUIEnvironment&			uiEnvironment;
	SlotType					slot;
	CommandType					command;
	std::vector<UINodePtr>		uiNodes;
};

template <typename SourceSlotType, typename TargetSlotType>
static void EnumerateConnectedSlots (const NodeUIManager& uiManager, const SourceSlotType& sourceSlot, const std::function<void (TargetSlotType)>& processor);

template <>
void EnumerateConnectedSlots<UIOutputSlotPtr, UIInputSlotConstPtr> (const NodeUIManager& uiManager, const UIOutputSlotPtr& sourceSlot, const std::function<void (UIInputSlotConstPtr)>& processor)
{
	uiManager.EnumerateConnectedInputSlots (sourceSlot, processor);
}

template <>
void EnumerateConnectedSlots<UIInputSlotPtr, UIOutputSlotConstPtr> (const NodeUIManager& uiManager, const UIInputSlotPtr& sourceSlot, const std::function<void (UIOutputSlotConstPtr)>& processor)
{
	uiManager.EnumerateConnectedOutputSlots (sourceSlot, processor);
}

template <typename RegistratorType, typename SourceSlotType, typename TargetSlotType, typename SlotCommandType>
class SlotCommandStructureBuilder : public RegistratorType
{
public:
	SlotCommandStructureBuilder (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const SourceSlotType& slot) :
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		slot (slot)
	{

	}

	virtual void RegisterSlotCommand (SlotCommandType slotCommand) override
	{
		CommandPtr command = CreateSlotCommand (slotCommand);
		commandStructure.AddCommand (command);
	}

	virtual void RegisterSlotGroupCommand (std::shared_ptr<NodeGroupCommand<SlotCommandType>> slotGroupCommand) override
	{
		GroupCommandPtr groupCommand (new GroupCommand (slotGroupCommand->GetName ()));
		slotGroupCommand->EnumerateChildCommands ([&] (const SlotCommandType& slotNodeCommand) {
			CommandPtr slotCommand = CreateSlotCommand (slotNodeCommand);
			groupCommand->AddChildCommand (slotCommand);
		});
		commandStructure.AddCommand (groupCommand);
	}

	virtual void EnumerateConnectedNodes (const std::function<void (UINodeConstPtr, TargetSlotType)>& processor) const override
	{
		EnumerateConnectedSlots<SourceSlotType, TargetSlotType> (uiManager, slot, [&] (TargetSlotType connectedSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (connectedSlot->GetOwnerNodeId ());
			processor (uiNode, connectedSlot);
		});
	}

	CommandPtr CreateSlotCommand (SlotCommandType slotNodeCommand)
	{
		CommandPtr slotCommand (new SlotCommand<SourceSlotType, SlotCommandType> (slotNodeCommand->GetName (), uiManager, uiEnvironment, slot, slotNodeCommand));
		return slotCommand;
	}

	const CommandStructure& GetCommandStructure ()
	{
		return commandStructure;
	}
	
private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	SourceSlotType		slot;
	CommandStructure	commandStructure;
};

CommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot)
{
	SlotCommandStructureBuilder<InputSlotCommandRegistrator, UIInputSlotPtr, UIOutputSlotConstPtr, InputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, inputSlot);
	inputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

CommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot)
{
	SlotCommandStructureBuilder<OutputSlotCommandRegistrator, UIOutputSlotPtr, UIInputSlotConstPtr, OutputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, outputSlot);
	outputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

}
