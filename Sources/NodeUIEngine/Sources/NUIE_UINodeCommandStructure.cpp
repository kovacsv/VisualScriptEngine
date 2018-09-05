#include "NUIE_UINodeCommandStructure.hpp"
#include "NUIE_UINodeCommandRegistration.hpp"
#include "NUIE_UINodeParameters.hpp"
#include "NUIE_EventHandlers.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"

#include <limits>

namespace NUIE
{

UndoableCommand::UndoableCommand (const std::wstring & name, bool isChecked) :
	SingleUICommand (name, isChecked)
{

}

UndoableCommand::~UndoableCommand ()
{

}

bool UndoableCommand::IsUndoable () const
{
	return true;
}

NotUndoableCommand::NotUndoableCommand (const std::wstring & name, bool isChecked) :
	SingleUICommand (name, isChecked)
{

}

NotUndoableCommand::~NotUndoableCommand ()
{

}

bool NotUndoableCommand::IsUndoable () const
{
	return false;
}

DeleteNodesCommand::DeleteNodesCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
	UndoableCommand (L"Delete Nodes", false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	relevantNodes (relevantNodes)
{

}

DeleteNodesCommand::~DeleteNodesCommand ()
{

}

void DeleteNodesCommand::Do ()
{
	relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		uiManager.DeleteNode (nodeId, uiEnvironment.GetEvaluationEnv ());
		return true;
	});
}

CopyNodesCommand::CopyNodesCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
	NotUndoableCommand (L"Copy Nodes", false),
	uiManager (uiManager),
	relevantNodes (relevantNodes)
{

}

CopyNodesCommand::~CopyNodesCommand ()
{

}

void CopyNodesCommand::Do ()
{
	uiManager.Copy (relevantNodes);
}

PasteNodesCommand::PasteNodesCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position) :
	UndoableCommand (L"Paste Nodes", false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	position (position)
{

}

PasteNodesCommand::~PasteNodesCommand ()
{

}

void PasteNodesCommand::Do ()
{
	std::unordered_set<NE::NodeId> oldNodes;
	uiManager.EnumerateUINodes ([&] (const UINodeConstPtr& uiNode) {
		oldNodes.insert (uiNode->GetId ());
		return true;
	});

	uiManager.Paste ();

	std::vector<UINodePtr> newNodes;
	uiManager.EnumerateUINodes ([&] (const UINodePtr& uiNode) {
		if (oldNodes.find (uiNode->GetId ()) == oldNodes.end ()) {
			newNodes.push_back (uiNode);
		}
		return true;
	});

	Point centerPosition;
	for (UINodePtr& uiNode : newNodes) {
		Point nodePosition = uiNode->GetNodePosition ();
		centerPosition = centerPosition + nodePosition;
	}

	NE::NodeCollection newSelection;
	centerPosition = centerPosition / (double) newNodes.size ();
	Point nodeOffset = position - centerPosition;
	for (UINodePtr& uiNode : newNodes) {
		Point nodePosition = uiNode->GetNodePosition ();
		uiNode->SetNodePosition (nodePosition + nodeOffset);
		newSelection.Insert (uiNode->GetId ());
	}

	uiManager.SetSelectedNodes (newSelection);
}

UndoCommand::UndoCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment) :
	NotUndoableCommand (L"Undo", false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{

}

UndoCommand::~UndoCommand ()
{

}

void UndoCommand::Do ()
{
	uiManager.Undo (uiEnvironment.GetEvaluationEnv ());
}

RedoCommand::RedoCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment) :
	NotUndoableCommand (L"Redo", false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{

}

RedoCommand::~RedoCommand ()
{

}

void RedoCommand::Do ()
{
	uiManager.Redo (uiEnvironment.GetEvaluationEnv ());
}

class DisconnectFromInputSlotCommand : public InputSlotCommand
{
public:
	DisconnectFromInputSlotCommand (const std::wstring& name, const UIOutputSlotConstPtr& slotToDisconnect) :
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

class DisconnectAllFromInputSlotCommand : public InputSlotCommand
{
public:
	DisconnectAllFromInputSlotCommand (const std::wstring& name) :
		InputSlotCommand (name, false)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIInputSlotPtr& inputSlot) override
	{
		uiManager.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
	}
};

class DisconnectFromOutputSlotCommand : public OutputSlotCommand
{
public:
	DisconnectFromOutputSlotCommand (const std::wstring& name, const UIInputSlotConstPtr& slotToDisconnect) :
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

class DisconnectAllFromOutputSlotCommand : public OutputSlotCommand
{
public:
	DisconnectAllFromOutputSlotCommand (const std::wstring& name) :
		OutputSlotCommand (name, false)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIOutputSlotPtr& outputSlot) override
	{
		uiManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
	}
};

class MultiNodeCommand : public UndoableCommand
{
public:
	MultiNodeCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, NodeCommandPtr& nodeCommand) :
		UndoableCommand (name, nodeCommand->IsChecked ()),
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
			nodeCommand->Do (uiManager, uiEnvironment, uiNode);
		}
	}

private:
	NodeUIManager&				uiManager;
	NodeUIEnvironment&			uiEnvironment;
	NodeCommandPtr				nodeCommand;
	std::vector<UINodePtr>		uiNodes;
};

class SetParametersCommand : public NotUndoableCommand
{
public:
	SetParametersCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& currentNode, const NE::NodeCollection& relevantNodes) :
		NotUndoableCommand (L"Set Parameters", false),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		currentNode (currentNode),
		relevantNodes (relevantNodes),
		relevantParameters ()
	{

	}

	virtual ~SetParametersCommand ()
	{

	}

	virtual void Do () override
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

			virtual std::vector<NE::ValuePtr> GetParameterValueChoices (size_t index) const override
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
			uiManager.SaveUndoState ();
			paramInterface->ApplyChanges (uiManager, uiEnvironment, relevantNodes);
		}
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodePtr			currentNode;
	NE::NodeCollection	relevantNodes;
	NodeParameterList	relevantParameters;
};

class SetGroupParametersCommand : public UndoableCommand
{
public:
	SetGroupParametersCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group) :
		UndoableCommand (L"Set Parameters", false),
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

			virtual std::vector<NE::ValuePtr> GetParameterValueChoices (size_t) const
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
		std::shared_ptr<MultiNodeCommand> multiNodeCommand = CreateMultiNodeCommand (nodeCommand);
		commandStructure.AddCommand (multiNodeCommand);
	}

	virtual void RegisterNodeGroupCommand (NodeGroupCommandPtr nodeGroupCommand) override
	{
		UIGroupCommandPtr groupCommand (new UIGroupCommand (nodeGroupCommand->GetName ()));
		nodeGroupCommand->EnumerateChildCommands ([&] (const NodeCommandPtr& nodeCommand) {
			std::shared_ptr<MultiNodeCommand> multiNodeCommand = CreateMultiNodeCommand (nodeCommand);
			groupCommand->AddChildCommand (multiNodeCommand);
		});
		commandStructure.AddCommand (groupCommand);
	}

	void RegisterCommand (UICommandPtr command)
	{
		commandStructure.AddCommand (command);
	}

	UICommandStructure& GetCommandStructure ()
	{
		return commandStructure;
	}

private:
	std::shared_ptr<MultiNodeCommand> CreateMultiNodeCommand (NodeCommandPtr nodeCommand)
	{
		std::shared_ptr<MultiNodeCommand> multiNodeCommand (new MultiNodeCommand (nodeCommand->GetName (), uiManager, uiEnvironment, nodeCommand));
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
	UICommandStructure			commandStructure;
};

template <typename SlotType, typename CommandType>
class SlotCommand : public UndoableCommand
{
public:
	SlotCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, SlotType& slot, CommandType& command) :
		UndoableCommand (name, command->IsChecked ()),
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
		UICommandPtr command = CreateSlotCommand (slotCommand);
		commandStructure.AddCommand (command);
	}

	virtual void RegisterSlotGroupCommand (std::shared_ptr<NodeGroupCommand<SlotCommandType>> slotGroupCommand) override
	{
		UIGroupCommandPtr groupCommand (new UIGroupCommand (slotGroupCommand->GetName ()));
		slotGroupCommand->EnumerateChildCommands ([&] (const SlotCommandType& slotNodeCommand) {
			UICommandPtr slotCommand = CreateSlotCommand (slotNodeCommand);
			groupCommand->AddChildCommand (slotCommand);
		});
		commandStructure.AddCommand (groupCommand);
	}

	UICommandPtr CreateSlotCommand (SlotCommandType slotNodeCommand)
	{
		UICommandPtr slotCommand (new SlotCommand<SourceSlotType, SlotCommandType> (slotNodeCommand->GetName (), uiManager, uiEnvironment, sourceSlot, slotNodeCommand));
		return slotCommand;
	}

	const UICommandStructure& GetCommandStructure ()
	{
		return commandStructure;
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	SourceSlotType		sourceSlot;
	UICommandStructure	commandStructure;
};

class CreateGroupCommand : public UndoableCommand
{
public:
	CreateGroupCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
		UndoableCommand (L"Create New Group", false),
		uiManager (uiManager),
		relevantNodes (relevantNodes)
	{
	
	}

	virtual ~CreateGroupCommand ()
	{
	
	}

	virtual void Do () override
	{
		UINodeGroupPtr group (new UINodeGroup (L"Group", relevantNodes));
		uiManager.AddUINodeGroup (group);
		uiManager.RequestRedraw ();
	}

private:
	NodeUIManager&		uiManager;
	NE::NodeCollection	relevantNodes;
};

class DeleteGroupCommand : public UndoableCommand
{
public:
	DeleteGroupCommand (NodeUIManager& uiManager, UINodeGroupPtr group) :
		UndoableCommand (L"Delete Group", false),
		uiManager (uiManager),
		group (group)
	{
	
	}

	virtual ~DeleteGroupCommand ()
	{
	
	}

	virtual void Do () override
	{
		uiManager.DeleteUINodeGroup (group);
		uiManager.RequestRedraw ();
	}

private:
	NodeUIManager&		uiManager;
	UINodeGroupPtr		group;
};

class RemoveNodesFromGroupCommand : public UndoableCommand
{
public:
	RemoveNodesFromGroupCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
		UndoableCommand (L"Remove From Group", false),
		uiManager (uiManager),
		relevantNodes (relevantNodes)
	{
	
	}

	virtual ~RemoveNodesFromGroupCommand ()
	{
	
	}

	virtual void Do () override
	{
		uiManager.RemoveNodesFromGroup (relevantNodes);
		uiManager.RequestRedraw ();
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

UICommandStructure CreateEmptyAreaCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position)
{
	UICommandStructure commandStructure;
	if (uiManager.CanPaste ()) {
		commandStructure.AddCommand (UICommandPtr (new PasteNodesCommand (uiManager, uiEnvironment, position)));
	}
	return commandStructure;
}

UICommandStructure CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode)
{
	NE::NodeCollection relevantNodes = GetNodesForCommand (uiManager, uiNode);
	NodeCommandStructureBuilder commandStructureBuilder (uiManager, uiEnvironment, relevantNodes);

	commandStructureBuilder.RegisterCommand (UICommandPtr (new SetParametersCommand (uiManager, uiEnvironment, uiNode, relevantNodes)));
	commandStructureBuilder.RegisterCommand (UICommandPtr (new CopyNodesCommand (uiManager, relevantNodes)));
	commandStructureBuilder.RegisterCommand (UICommandPtr (new DeleteNodesCommand (uiManager, uiEnvironment, relevantNodes)));

	UIGroupCommandPtr groupingCommandGroup (new UIGroupCommand (L"Grouping"));
	groupingCommandGroup->AddChildCommand (UICommandPtr (new CreateGroupCommand (uiManager, relevantNodes)));
	bool isNodeGrouped = false;
	uiManager.EnumerateUINodeGroups ([&] (const UINodeGroupPtr& group) {
		if (group->ContainsNode (uiNode->GetId ())) {
			isNodeGrouped = true;
		}
		return !isNodeGrouped;
	});
	if (isNodeGrouped) {
		groupingCommandGroup->AddChildCommand (UICommandPtr (new RemoveNodesFromGroupCommand (uiManager, relevantNodes)));
	}
	commandStructureBuilder.RegisterCommand (groupingCommandGroup);

	uiNode->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

UICommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot)
{
	SlotCommandStructureBuilder<OutputSlotCommandRegistrator, UIOutputSlotPtr, OutputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, outputSlot);

	if (uiManager.HasConnectedInputSlots (outputSlot)) {
		OutputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<OutputSlotCommandPtr> (L"Disconnect"));
		uiManager.EnumerateConnectedInputSlots (outputSlot, [&] (UIInputSlotConstPtr inputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (inputSlot->GetOwnerNodeId ());
			disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectFromOutputSlotCommand (uiNode->GetNodeName () + L" (" + inputSlot->GetName () + L")", inputSlot)));
		});
		disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectAllFromOutputSlotCommand (L"All")));
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	outputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

UICommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot)
{
	SlotCommandStructureBuilder<InputSlotCommandRegistrator, UIInputSlotPtr, InputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, inputSlot);

	if (uiManager.HasConnectedOutputSlots (inputSlot)) {
		InputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<InputSlotCommandPtr> (L"Disconnect"));
		uiManager.EnumerateConnectedOutputSlots (inputSlot, [&] (UIOutputSlotConstPtr outputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (outputSlot->GetOwnerNodeId ());
			disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectFromInputSlotCommand (uiNode->GetNodeName () + L" (" + outputSlot->GetName () + L")", outputSlot)));
		});
		disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectAllFromInputSlotCommand (L"All")));
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	inputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

UICommandStructure CreateNodeGroupCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group)
{
	UICommandStructure commandStructure;
	commandStructure.AddCommand (UICommandPtr (new SetGroupParametersCommand (uiManager, uiEnvironment, group)));
	commandStructure.AddCommand (UICommandPtr (new DeleteGroupCommand (uiManager, group)));
	return commandStructure;
}

}
