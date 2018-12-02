#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeMenuCommandRegistrator.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_EventHandlers.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"

#include <limits>

namespace NUIE
{

DeleteNodesMenuCommand::DeleteNodesMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (L"Delete Nodes", false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	relevantNodes (relevantNodes)
{

}

DeleteNodesMenuCommand::~DeleteNodesMenuCommand ()
{

}

void DeleteNodesMenuCommand::Do ()
{
	DeleteNodesCommand command (relevantNodes, uiEnvironment.GetEvaluationEnv ());
	uiManager.ExecuteCommand (command);
}

CopyNodesMenuCommand::CopyNodesMenuCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (L"Copy Nodes", false),
	uiManager (uiManager),
	relevantNodes (relevantNodes)
{

}

CopyNodesMenuCommand::~CopyNodesMenuCommand ()
{

}

void CopyNodesMenuCommand::Do ()
{
	CopyNodesCommand command (relevantNodes);
	uiManager.ExecuteCommand (command);
}

PasteNodesMenuCommand::PasteNodesMenuCommand (NodeUIManager& uiManager, const Point& position) :
	SingleMenuCommand (L"Paste Nodes", false),
	uiManager (uiManager),
	position (position)
{

}

PasteNodesMenuCommand::~PasteNodesMenuCommand ()
{

}

void PasteNodesMenuCommand::Do ()
{
	PasteNodesCommand command (position);
	uiManager.ExecuteCommand (command);
}

UndoMenuCommand::UndoMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment) :
	SingleMenuCommand (L"Undo", false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{

}

UndoMenuCommand::~UndoMenuCommand ()
{

}

void UndoMenuCommand::Do ()
{
	UndoCommand command (uiEnvironment.GetEvaluationEnv ());
	uiManager.ExecuteCommand (command);
}

RedoMenuCommand::RedoMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment) :
	SingleMenuCommand (L"Redo", false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{

}

RedoMenuCommand::~RedoMenuCommand ()
{

}

void RedoMenuCommand::Do ()
{
	RedoCommand command (uiEnvironment.GetEvaluationEnv ());
	uiManager.ExecuteCommand (command);
}

SetParametersCommand::SetParametersCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& currentNode, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (L"Set Parameters", false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	currentNode (currentNode),
	relevantNodes (relevantNodes),
	relevantParameters ()
{
	DBGASSERT (relevantNodes.Contains (currentNode->GetId ()));
}

SetParametersCommand::~SetParametersCommand ()
{

}

void SetParametersCommand::Do ()
{
	class NodeSelectionParameterInterface : public ParameterInterface
	{
	public:
		NodeSelectionParameterInterface (NodeParameterList& paramList, const UINodePtr& currentNode) :
			paramList (paramList),
			currentNode (currentNode)
		{

		}

		void ApplyChanges (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes)
		{
			for (const auto& it : changedParameterValues) {
				NodeParameterPtr& parameter = paramList.GetParameter (it.first);
				ApplyCommonParameter (uiManager, uiEnvironment.GetEvaluationEnv (), relevantNodes, parameter, it.second);
			}
			uiManager.Update (uiEnvironment);
		}

		virtual size_t GetParameterCount () const override
		{
			return paramList.GetParameterCount ();
		}

		virtual const std::wstring& GetParameterName (size_t index) const override
		{
			static const std::wstring InvalidParameterName = L"";
			NodeParameterPtr parameter = paramList.GetParameter (index);
			if (DBGERROR (parameter == nullptr)) {
				return InvalidParameterName;
			}
			return parameter->GetName ();
		}

		virtual NE::ValuePtr GetParameterValue (size_t index) const override
		{
			NodeParameterPtr parameter = paramList.GetParameter (index);
			if (DBGERROR (parameter == nullptr)) {
				return nullptr;
			}
			return parameter->GetValue (currentNode);
		}

		virtual std::vector<std::wstring> GetParameterValueChoices (size_t index) const override
		{
			NodeParameterPtr parameter = paramList.GetParameter (index);
			if (DBGERROR (parameter == nullptr)) {
				return {};
			}
			return parameter->GetValueChoices ();
		}

		virtual const ParameterType& GetParameterType (size_t index) const override
		{
			NodeParameterPtr parameter = paramList.GetParameter (index);
			if (DBGERROR (parameter == nullptr)) {
				return ParameterType::Undefined;
			}
			return parameter->GetType ();
		}

		virtual bool IsValidParameterValue (size_t index, const NE::ValuePtr& value) const override
		{
			NodeParameterPtr parameter = paramList.GetParameter (index);
			if (DBGERROR (parameter == nullptr)) {
				return false;
			}

			return parameter->CanSetValue (currentNode, value);
		}

		virtual bool SetParameterValue (size_t index, const NE::ValuePtr& value) override
		{
			if (DBGERROR (!IsValidParameterValue (index, value))) {
				return false;
			}

			auto found = changedParameterValues.find (index);
			if (found != changedParameterValues.end ()) {
				found->second = value;
			} else {
				changedParameterValues.insert ({ index, value });
			}

			return true;
		}

	private:
		NodeParameterList&							paramList;
		const UINodePtr&							currentNode;
		std::unordered_map<size_t, NE::ValuePtr>	changedParameterValues;
	};

	RegisterCommonParameters (uiManager, relevantNodes, relevantParameters);
	std::shared_ptr<NodeSelectionParameterInterface> paramInterface (new NodeSelectionParameterInterface (relevantParameters, currentNode));
	if (uiEnvironment.GetEventHandlers ().OnParameterSettings (paramInterface)) {
		CustomUndoableCommand command ([&] () {
			paramInterface->ApplyChanges (uiManager, uiEnvironment, relevantNodes);
		});
		uiManager.ExecuteCommand (command);
	}
}

class DisconnectFromInputSlotMenuCommand : public InputSlotCommand
{
public:
	DisconnectFromInputSlotMenuCommand (const std::wstring& name, const UIOutputSlotConstPtr& slotToDisconnect) :
		InputSlotCommand (name, false),
		slotToDisconnect (slotToDisconnect)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIInputSlotPtr& inputSlot) override
	{
		DisconnectSlotsCommand command (slotToDisconnect, inputSlot);
		uiManager.ExecuteCommand (command);
	}

private:
	UIOutputSlotConstPtr slotToDisconnect;
};

class DisconnectAllFromInputSlotMenuCommand : public InputSlotCommand
{
public:
	DisconnectAllFromInputSlotMenuCommand (const std::wstring& name) :
		InputSlotCommand (name, false)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIInputSlotPtr& inputSlot) override
	{
		DisconnectAllOutputSlotsCommand command (inputSlot);
		uiManager.ExecuteCommand (command);
	}
};

class DisconnectFromOutputSlotMenuCommand : public OutputSlotCommand
{
public:
	DisconnectFromOutputSlotMenuCommand (const std::wstring& name, const UIInputSlotConstPtr& slotToDisconnect) :
		OutputSlotCommand (name, false),
		slotToDisconnect (slotToDisconnect)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIOutputSlotPtr& outputSlot) override
	{
		DisconnectSlotsCommand command (outputSlot, slotToDisconnect);
		uiManager.ExecuteCommand (command);
	}

private:
	UIInputSlotConstPtr slotToDisconnect;
};

class DisconnectAllFromOutputSlotMenuCommand : public OutputSlotCommand
{
public:
	DisconnectAllFromOutputSlotMenuCommand (const std::wstring& name) :
		OutputSlotCommand (name, false)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIOutputSlotPtr& outputSlot) override
	{
		DisconnectAllInputSlotsCommand command (outputSlot);
		uiManager.ExecuteCommand (command);
	}
};

class MultiNodeMenuCommand : public SingleMenuCommand
{
public:
	MultiNodeMenuCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, NodeCommandPtr& nodeCommand) :
		SingleMenuCommand (name, nodeCommand->IsChecked ()),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		nodeCommand (nodeCommand)
	{

	}

	virtual ~MultiNodeMenuCommand ()
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
		CustomUndoableCommand command ([&] () {
			for (UINodePtr& uiNode : uiNodes) {
				nodeCommand->Do (uiManager, uiEnvironment, uiNode);
			}
		});
		uiManager.ExecuteCommand (command);
	}

private:
	NodeUIManager&				uiManager;
	NodeUIEnvironment&			uiEnvironment;
	NodeCommandPtr				nodeCommand;
	std::vector<UINodePtr>		uiNodes;
};

class SetGroupParametersCommand : public SingleMenuCommand
{
public:
	SetGroupParametersCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group) :
		SingleMenuCommand (L"Set Parameters", false),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		group (group)
	{

	}

	virtual ~SetGroupParametersCommand ()
	{

	}

	virtual void Do () override
	{
		class GroupParameterInterface : public ParameterInterface
		{
		public:
			struct GroupParameter
			{
				std::wstring	name;
				ParameterType	type;
			};

			GroupParameterInterface (const UINodeGroupPtr& currentGroup) :
				currentGroup (currentGroup),
				groupParameters ({
					{ L"Name", ParameterType::String }
				})
			{
			
			}

			void ApplyChanges (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment)
			{
				for (const auto& it : changedParameterValues) {
					switch (it.first) {
						case 0:
							currentGroup->SetName (NE::StringValue::Get (it.second));
							break;
						default:
							DBGBREAK ();
							break;
					}
				}
				uiManager.RequestRedraw ();
				uiManager.Update (uiEnvironment);
			}

			virtual size_t GetParameterCount () const override
			{
				return groupParameters.size ();
			}

			virtual const std::wstring& GetParameterName (size_t index) const override
			{
				return groupParameters[index].name;
			}

			virtual NE::ValuePtr GetParameterValue (size_t index) const override
			{
				switch (index) {
					case 0:
						return NE::ValuePtr (new NE::StringValue (currentGroup->GetName ()));
					default:
						DBGBREAK ();
						return nullptr;
				}
			}

			virtual std::vector<std::wstring> GetParameterValueChoices (size_t) const override
			{
				DBGBREAK ();
				return {};
			}

			virtual const ParameterType& GetParameterType (size_t index) const override
			{
				return groupParameters[index].type;
			}

			virtual bool IsValidParameterValue (size_t index, const NE::ValuePtr& value) const override
			{
				switch (index) {
					case 0:
						return NE::Value::IsType<NE::StringValue> (value) && !NE::StringValue::Get (value).empty ();
					default:
						DBGBREAK ();
				}
				return false;
			}

			virtual bool SetParameterValue (size_t index, const NE::ValuePtr& value) override
			{
				if (DBGERROR (!IsValidParameterValue (index, value))) {
					return false;
				}

				auto found = changedParameterValues.find (index);
				if (found != changedParameterValues.end ()) {
					found->second = value;
				} else {
					changedParameterValues.insert ({ index, value });
				}
				return true;
			}

		private:
			const UINodeGroupPtr&						currentGroup;
			std::vector<GroupParameter>					groupParameters;
			std::unordered_map<size_t, NE::ValuePtr>	changedParameterValues;
		};

		std::shared_ptr<GroupParameterInterface> paramInterface (new GroupParameterInterface (group));
		if (uiEnvironment.GetEventHandlers ().OnParameterSettings (paramInterface)) {
			paramInterface->ApplyChanges (uiManager, uiEnvironment);
		}
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodeGroupPtr		group;
};

class NodeCommandStructureBuilder : public NodeCommandRegistrator
{
public:
	NodeCommandStructureBuilder (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		relevantNodes (relevantNodes)
	{

	}

	virtual void RegisterNodeCommand (NodeCommandPtr nodeCommand) override
	{
		std::shared_ptr<MultiNodeMenuCommand> multiNodeCommand = CreateMultiNodeCommand (nodeCommand);
		commandStructure.AddCommand (multiNodeCommand);
	}

	virtual void RegisterNodeGroupCommand (NodeGroupCommandPtr nodeGroupCommand) override
	{
		GroupMenuCommandPtr groupCommand (new GroupMenuCommand (nodeGroupCommand->GetName ()));
		nodeGroupCommand->EnumerateChildCommands ([&] (const NodeCommandPtr& nodeCommand) {
			std::shared_ptr<MultiNodeMenuCommand> multiNodeCommand = CreateMultiNodeCommand (nodeCommand);
			groupCommand->AddChildCommand (multiNodeCommand);
		});
		commandStructure.AddCommand (groupCommand);
	}

	void RegisterCommand (MenuCommandPtr command)
	{
		commandStructure.AddCommand (command);
	}

	MenuCommandStructure& GetCommandStructure ()
	{
		return commandStructure;
	}

private:
	std::shared_ptr<MultiNodeMenuCommand> CreateMultiNodeCommand (NodeCommandPtr nodeCommand)
	{
		std::shared_ptr<MultiNodeMenuCommand> multiNodeCommand (new MultiNodeMenuCommand (nodeCommand->GetName (), uiManager, uiEnvironment, nodeCommand));
		relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			UINodePtr uiNode = uiManager.GetUINode (nodeId);
			if (nodeCommand->IsApplicableTo (uiNode)) {
				multiNodeCommand->AddNode (uiNode);
			}
			return true;
		});
		return multiNodeCommand;
	}

	NodeUIManager&				uiManager;
	NodeUIEnvironment&			uiEnvironment;
	const NE::NodeCollection&	relevantNodes;
	MenuCommandStructure			commandStructure;
};

template <typename SlotType, typename CommandType>
class SlotCommand : public SingleMenuCommand
{
public:
	SlotCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, SlotType& slot, CommandType& command) :
		SingleMenuCommand (name, command->IsChecked ()),
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
		MenuCommandPtr command = CreateSlotCommand (slotCommand);
		commandStructure.AddCommand (command);
	}

	virtual void RegisterSlotGroupCommand (std::shared_ptr<NodeGroupCommand<SlotCommandType>> slotGroupCommand) override
	{
		GroupMenuCommandPtr groupCommand (new GroupMenuCommand (slotGroupCommand->GetName ()));
		slotGroupCommand->EnumerateChildCommands ([&] (const SlotCommandType& slotNodeCommand) {
			MenuCommandPtr slotCommand = CreateSlotCommand (slotNodeCommand);
			groupCommand->AddChildCommand (slotCommand);
		});
		commandStructure.AddCommand (groupCommand);
	}

	MenuCommandPtr CreateSlotCommand (SlotCommandType slotNodeCommand)
	{
		MenuCommandPtr slotCommand (new SlotCommand<SourceSlotType, SlotCommandType> (slotNodeCommand->GetName (), uiManager, uiEnvironment, sourceSlot, slotNodeCommand));
		return slotCommand;
	}

	const MenuCommandStructure& GetCommandStructure ()
	{
		return commandStructure;
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	SourceSlotType		sourceSlot;
	MenuCommandStructure	commandStructure;
};

class CreateGroupMenuCommand : public SingleMenuCommand
{
public:
	CreateGroupMenuCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
		SingleMenuCommand (L"Create New Group", false),
		uiManager (uiManager),
		relevantNodes (relevantNodes)
	{
	
	}

	virtual ~CreateGroupMenuCommand ()
	{
	
	}

	virtual void Do () override
	{
		UINodeGroupPtr group (new UINodeGroup (L"Group", relevantNodes));
		AddGroupCommand command (group);
		uiManager.ExecuteCommand (command);
	}

private:
	NodeUIManager&		uiManager;
	NE::NodeCollection	relevantNodes;
};

class DeleteGroupMenuCommand : public SingleMenuCommand
{
public:
	DeleteGroupMenuCommand (NodeUIManager& uiManager, UINodeGroupPtr group) :
		SingleMenuCommand (L"Delete Group", false),
		uiManager (uiManager),
		group (group)
	{
	
	}

	virtual ~DeleteGroupMenuCommand ()
	{
	
	}

	virtual void Do () override
	{
		DeleteGroupCommand command (group);
		uiManager.ExecuteCommand (command);
	}

private:
	NodeUIManager&		uiManager;
	UINodeGroupPtr		group;
};

class RemoveNodesFromGroupMenuCommand : public SingleMenuCommand
{
public:
	RemoveNodesFromGroupMenuCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
		SingleMenuCommand (L"Remove From Group", false),
		uiManager (uiManager),
		relevantNodes (relevantNodes)
	{
	
	}

	virtual ~RemoveNodesFromGroupMenuCommand ()
	{
	
	}

	virtual void Do () override
	{
		RemoveNodesFromGroupCommand command (relevantNodes);
		uiManager.ExecuteCommand (command);
	}

private:
	NodeUIManager&		uiManager;
	NE::NodeCollection	relevantNodes;
};

NE::NodeCollection GetNodesForCommand (const NodeUIManager& uiManager, const UINodePtr& uiNode)
{
	const NE::NodeCollection& selectedNodes = uiManager.GetSelectedNodes ();
	if (selectedNodes.Contains (uiNode->GetId ())) {
		return selectedNodes;
	}
	return NE::NodeCollection ({ uiNode->GetId () });
}

MenuCommandStructure CreateEmptyAreaCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment&, const Point& position)
{
	MenuCommandStructure commandStructure;
	if (uiManager.CanPaste ()) {
		commandStructure.AddCommand (MenuCommandPtr (new PasteNodesMenuCommand (uiManager, position)));
	}
	return commandStructure;
}

MenuCommandStructure CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode)
{
	NE::NodeCollection relevantNodes = GetNodesForCommand (uiManager, uiNode);
	NodeCommandStructureBuilder commandStructureBuilder (uiManager, uiEnvironment, relevantNodes);

	commandStructureBuilder.RegisterCommand (MenuCommandPtr (new SetParametersCommand (uiManager, uiEnvironment, uiNode, relevantNodes)));
	uiNode->RegisterCommands (commandStructureBuilder);

	commandStructureBuilder.RegisterCommand (MenuCommandPtr (new CopyNodesMenuCommand (uiManager, relevantNodes)));
	commandStructureBuilder.RegisterCommand (MenuCommandPtr (new DeleteNodesMenuCommand (uiManager, uiEnvironment, relevantNodes)));

	GroupMenuCommandPtr groupingCommandGroup (new GroupMenuCommand (L"Grouping"));
	groupingCommandGroup->AddChildCommand (MenuCommandPtr (new CreateGroupMenuCommand (uiManager, relevantNodes)));
	bool isNodeGrouped = false;
	uiManager.EnumerateUINodeGroups ([&] (const UINodeGroupPtr& group) {
		if (group->ContainsNode (uiNode->GetId ())) {
			isNodeGrouped = true;
		}
		return !isNodeGrouped;
	});
	if (isNodeGrouped) {
		groupingCommandGroup->AddChildCommand (MenuCommandPtr (new RemoveNodesFromGroupMenuCommand (uiManager, relevantNodes)));
	}
	commandStructureBuilder.RegisterCommand (groupingCommandGroup);

	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot)
{
	SlotCommandStructureBuilder<OutputSlotCommandRegistrator, UIOutputSlotPtr, OutputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, outputSlot);

	if (uiManager.HasConnectedInputSlots (outputSlot)) {
		OutputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<OutputSlotCommandPtr> (L"Disconnect"));
		uiManager.EnumerateConnectedInputSlots (outputSlot, [&] (UIInputSlotConstPtr inputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (inputSlot->GetOwnerNodeId ());
			disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectFromOutputSlotMenuCommand (uiNode->GetNodeName () + L" (" + inputSlot->GetName () + L")", inputSlot)));
		});
		disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectAllFromOutputSlotMenuCommand (L"All")));
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	outputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot)
{
	SlotCommandStructureBuilder<InputSlotCommandRegistrator, UIInputSlotPtr, InputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, inputSlot);

	if (uiManager.HasConnectedOutputSlots (inputSlot)) {
		InputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<InputSlotCommandPtr> (L"Disconnect"));
		uiManager.EnumerateConnectedOutputSlots (inputSlot, [&] (UIOutputSlotConstPtr outputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (outputSlot->GetOwnerNodeId ());
			disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectFromInputSlotMenuCommand (uiNode->GetNodeName () + L" (" + outputSlot->GetName () + L")", outputSlot)));
		});
		disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectAllFromInputSlotMenuCommand (L"All")));
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	inputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateNodeGroupCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group)
{
	MenuCommandStructure commandStructure;
	commandStructure.AddCommand (MenuCommandPtr (new SetGroupParametersCommand (uiManager, uiEnvironment, group)));
	commandStructure.AddCommand (MenuCommandPtr (new DeleteGroupMenuCommand (uiManager, group)));
	return commandStructure;
}

}
