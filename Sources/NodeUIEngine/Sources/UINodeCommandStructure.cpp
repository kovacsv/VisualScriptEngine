#include "UINodeCommandStructure.hpp"
#include "UINodeCommands.hpp"
#include "Debug.hpp"

namespace NUIE
{

class DeleteNodeCommand : public NodeCommand
{
public:
	DeleteNodeCommand (const std::wstring& name) :
		NodeCommand (name, false)
	{

	}

	virtual bool IsApplicableTo (const UINodePtr&) override
	{
		return true;
	}

	virtual void Do (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode) override
	{
		uiManager.DeleteNode (uiNode, evaluationEnv);
	}
};

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

class MultiNodeCommand : public SingleCommand
{
public:
	MultiNodeCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, NodeCommandPtr& nodeCommand) :
		SingleCommand (name, nodeCommand->IsChecked ()),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		nodeCommand (nodeCommand)
	{

	}

	virtual ~MultiNodeCommand ()
	{

	}

	void AddNode (const UINodePtr& uiNode)
	{
		if (DBGERROR (nodeCommand == nullptr)) {
			return;
		}
		if (nodeCommand->IsApplicableTo (uiNode)) {
			uiNodes.push_back (uiNode);
		}
	}

	virtual void Do () override
	{
		if (DBGERROR (nodeCommand == nullptr)) {
			return;
		}
		for (UINodePtr& uiNode : uiNodes) {
			nodeCommand->Do (uiManager, uiEnvironment.GetEvaluationEnv (), uiNode);
		}
	}

private:
	NodeUIManager&				uiManager;
	NodeUIEnvironment&			uiEnvironment;
	NodeCommandPtr				nodeCommand;
	std::vector<UINodePtr>		uiNodes;
};

class NodeCommandStructureBuilder : public NodeCommandRegistrator
{
public:
	NodeCommandStructureBuilder (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NodeUIManager::SelectedNodes& selectedNodes) :
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		selectedNodes (selectedNodes)
	{

	}

	virtual void RegisterNodeCommand (NodeCommandPtr nodeCommand) override
	{
		std::shared_ptr<MultiNodeCommand> multiNodeCommand = CreateMultiNodeCommand (nodeCommand);
		commandStructure.AddCommand (multiNodeCommand);
	}

	virtual void RegisterNodeGroupCommand (NodeGroupCommandPtr nodeGroupCommand) override
	{
		GroupCommandPtr groupCommand (new GroupCommand (nodeGroupCommand->GetName ()));
		nodeGroupCommand->EnumerateChildCommands ([&] (const NodeCommandPtr& nodeCommand) {
			std::shared_ptr<MultiNodeCommand> multiNodeCommand = CreateMultiNodeCommand (nodeCommand);
			groupCommand->AddChildCommand (multiNodeCommand);
		});
		commandStructure.AddCommand (groupCommand);
	}

	std::shared_ptr<MultiNodeCommand> CreateMultiNodeCommand (NodeCommandPtr nodeCommand)
	{
		std::shared_ptr<MultiNodeCommand> multiNodeCommand (new MultiNodeCommand (nodeCommand->GetName (), uiManager, uiEnvironment, nodeCommand));
		selectedNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			UINodePtr uiNode = uiManager.GetUINode (nodeId);
			if (nodeCommand->IsApplicableTo (uiNode)) {
				multiNodeCommand->AddNode (uiNode);
			}
		});
		return multiNodeCommand;
	}

	NodeUIManager&							uiManager;
	NodeUIEnvironment&						uiEnvironment;
	const NodeUIManager::SelectedNodes&		selectedNodes;
	CommandStructure						commandStructure;
};

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

template <typename RegistratorType, typename SourceSlotType, typename SlotCommandType>
class SlotCommandStructureBuilder : public RegistratorType
{
public:
	SlotCommandStructureBuilder (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const SourceSlotType& sourceSlot) :
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		sourceSlot (sourceSlot)
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

	CommandPtr CreateSlotCommand (SlotCommandType slotNodeCommand)
	{
		CommandPtr slotCommand (new SlotCommand<SourceSlotType, SlotCommandType> (slotNodeCommand->GetName (), uiManager, uiEnvironment, sourceSlot, slotNodeCommand));
		return slotCommand;
	}

	const CommandStructure& GetCommandStructure ()
	{
		return commandStructure;
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	SourceSlotType		sourceSlot;
	CommandStructure	commandStructure;
};

CommandStructure CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode)
{
	NodeUIManager::SelectedNodes allSelectedNodes = uiManager.GetSelectedNodes ();
	allSelectedNodes.Insert (uiNode->GetId ());

	NodeCommandStructureBuilder commandStructureBuilder (uiManager, uiEnvironment, allSelectedNodes);
	commandStructureBuilder.RegisterNodeCommand (NodeCommandPtr (new DeleteNodeCommand (L"Delete Node")));
	uiNode->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.commandStructure;
}

CommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot)
{
	SlotCommandStructureBuilder<InputSlotCommandRegistrator, UIInputSlotPtr, InputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, inputSlot);

	if (uiManager.HasConnectedOutputSlots (inputSlot)) {
		InputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<InputSlotCommandPtr> (L"Disconnect"));
		uiManager.EnumerateConnectedOutputSlots (inputSlot, [&] (UIOutputSlotConstPtr outputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (outputSlot->GetOwnerNodeId ());
			disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectInputSlotCommand (uiNode->GetNodeName () + L" (" + outputSlot->GetName () + L")", outputSlot)));
		});
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	inputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

CommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot)
{
	SlotCommandStructureBuilder<OutputSlotCommandRegistrator, UIOutputSlotPtr, OutputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, outputSlot);

	if (uiManager.HasConnectedInputSlots (outputSlot)) {
		OutputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<OutputSlotCommandPtr> (L"Disconnect"));
		uiManager.EnumerateConnectedInputSlots (outputSlot, [&] (UIInputSlotConstPtr inputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (inputSlot->GetOwnerNodeId ());
			disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectOutputSlotCommand (uiNode->GetNodeName () + L" (" + inputSlot->GetName () + L")", inputSlot)));
		});
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	outputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

}
