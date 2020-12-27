#include "NUIE_NodeCommonMenuCommands.hpp"
#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_EventHandler.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_NodeAlignment.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Localization.hpp"
#include "NE_Debug.hpp"

#include <limits>
#include <algorithm>

namespace NUIE
{

DeleteNodesMenuCommand::DeleteNodesMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::LocString (L"Delete Nodes"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	relevantNodes (relevantNodes)
{

}

DeleteNodesMenuCommand::~DeleteNodesMenuCommand ()
{

}

bool DeleteNodesMenuCommand::WillModify () const
{
	return !relevantNodes.IsEmpty ();
}

void DeleteNodesMenuCommand::DoModification ()
{
	DeleteNodesCommand command (uiEnvironment, relevantNodes);
	uiManager.ExecuteCommand (command, uiEnvironment);
}

CopyNodesMenuCommand::CopyNodesMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::LocString (L"Copy Nodes"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	relevantNodes (relevantNodes)
{

}

CopyNodesMenuCommand::~CopyNodesMenuCommand ()
{

}

bool CopyNodesMenuCommand::WillModify () const
{
	return !relevantNodes.IsEmpty ();
}

void CopyNodesMenuCommand::DoModification ()
{
	CopyNodesCommand command (uiEnvironment, relevantNodes);
	uiManager.ExecuteCommand (command, uiEnvironment);
}

PasteNodesMenuCommand::PasteNodesMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position) :
	SingleMenuCommand (NE::LocString (L"Paste Nodes"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	position (position)
{

}

PasteNodesMenuCommand::~PasteNodesMenuCommand ()
{

}

bool PasteNodesMenuCommand::WillModify () const
{
	const ClipboardHandler& clipboard = uiEnvironment.GetClipboardHandler ();
	return clipboard.HasClipboardContent ();
}

void PasteNodesMenuCommand::DoModification ()
{
	PasteNodesCommand command (uiEnvironment, position);
	uiManager.ExecuteCommand (command, uiEnvironment);
}

AlignToWindowMenuCommand::AlignToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::LocString (L"Align To Window"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{
}

AlignToWindowMenuCommand::~AlignToWindowMenuCommand ()
{
}

bool AlignToWindowMenuCommand::WillModify () const
{
	return true;
}

void AlignToWindowMenuCommand::DoModification ()
{
	uiManager.AlignToWindow (uiEnvironment);
}

CenterToWindowMenuCommand::CenterToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::LocString (L"Center To Window"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{
}

CenterToWindowMenuCommand::~CenterToWindowMenuCommand ()
{
}

bool CenterToWindowMenuCommand::WillModify () const
{
	return true;
}

void CenterToWindowMenuCommand::DoModification ()
{
	uiManager.CenterToWindow (uiEnvironment);
}

FitToWindowMenuCommand::FitToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::LocString (L"Fit To Window"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{
}

FitToWindowMenuCommand::~FitToWindowMenuCommand ()
{
}

bool FitToWindowMenuCommand::WillModify () const
{
	return true;
}

void FitToWindowMenuCommand::DoModification ()
{
	uiManager.FitToWindow (uiEnvironment);
}

UndoMenuCommand::UndoMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::LocString (L"Undo"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{

}

UndoMenuCommand::~UndoMenuCommand ()
{

}

bool UndoMenuCommand::WillModify () const
{
	return uiManager.CanUndo ();
}

void UndoMenuCommand::DoModification ()
{
	UndoCommand command (uiEnvironment);
	uiManager.ExecuteCommand (command, uiEnvironment);
}

RedoMenuCommand::RedoMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::LocString (L"Redo"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{

}

RedoMenuCommand::~RedoMenuCommand ()
{

}

bool RedoMenuCommand::WillModify () const
{
	return uiManager.CanRedo ();
}

void RedoMenuCommand::DoModification ()
{
	RedoCommand command (uiEnvironment);
	uiManager.ExecuteCommand (command, uiEnvironment);
}

SetNodeParametersMenuCommand::SetNodeParametersMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeConstPtr& currentNode, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::LocString (L"Node Settings"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	currentNode (currentNode),
	relevantNodes (relevantNodes)
{
	DBGASSERT (relevantNodes.Contains (currentNode->GetId ()));
}

SetNodeParametersMenuCommand::~SetNodeParametersMenuCommand ()
{

}

bool SetNodeParametersMenuCommand::WillModify () const
{
	return !relevantNodes.IsEmpty ();
}

void SetNodeParametersMenuCommand::DoModification ()
{
	class NodeSelectionParameterInterface : public ParameterInterface
	{
	public:
		NodeSelectionParameterInterface (const UINodeConstPtr& currentNode, const NE::NodeCollection& nodeList, const NodeParameterList& paramList) :
			currentNode (currentNode),
			nodeList (nodeList),
			paramList (paramList)
		{

		}

		virtual size_t GetParameterCount () const override
		{
			return paramList.GetParameterCount ();
		}

		virtual std::wstring GetParameterName (size_t index) const override
		{
			static const std::wstring InvalidParameterName;
			NodeParameterPtr parameter = paramList.GetParameter (index);
			if (DBGERROR (parameter == nullptr)) {
				return InvalidParameterName;
			}
			return parameter->GetName ().GetLocalized ();
		}

		virtual NE::ValueConstPtr GetParameterValue (size_t index) const override
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
			std::vector<std::wstring> locValueChoices;
			for (const NE::LocString& choice : parameter->GetValueChoices ()) {
				locValueChoices.push_back (choice.GetLocalized ());
			}
			return locValueChoices;
		}

		virtual const ParameterType& GetParameterType (size_t index) const override
		{
			NodeParameterPtr parameter = paramList.GetParameter (index);
			if (DBGERROR (parameter == nullptr)) {
				return ParameterType::Undefined;
			}
			return parameter->GetType ();
		}

		virtual bool IsValidParameterValue (size_t index, const NE::ValueConstPtr& value) const override
		{
			NodeParameterPtr parameter = paramList.GetParameter (index);
			if (DBGERROR (parameter == nullptr)) {
				return false;
			}

			return parameter->CanSetValue (currentNode, value);
		}

		virtual bool SetParameterValue (size_t index, const NE::ValueConstPtr& value) override
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

		virtual void ApplyChanges (NodeUIManager& uiManager, NE::EvaluationEnv& evalEnv) override
		{
			for (const auto& it : changedParameterValues) {
				NodeParameterPtr& parameter = paramList.GetParameter (it.first);
				ApplyCommonParameter (uiManager, evalEnv, nodeList, parameter, it.second);
			}
		}

	private:
		UINodeConstPtr									currentNode;
		NE::NodeCollection								nodeList;
		NodeParameterList								paramList;
		std::unordered_map<size_t, NE::ValueConstPtr>	changedParameterValues;
	};

	NodeParameterList relevantParameters;
	RegisterCommonParameters (uiManager, relevantNodes, relevantParameters);
	ParameterInterfacePtr paramInterface (new NodeSelectionParameterInterface (currentNode, relevantNodes, relevantParameters));
	if (uiEnvironment.GetEventHandler ().OnParameterSettings (EventHandler::ParameterSettingsType::Node, paramInterface)) {
		ApplyParametersCommand command (paramInterface, uiEnvironment.GetEvaluationEnv ());
		uiManager.ExecuteCommand (command, uiEnvironment);
	}
}

CreateGroupMenuCommand::CreateGroupMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::LocString (L"Create New Group"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	relevantNodes (relevantNodes)
{

}

CreateGroupMenuCommand::~CreateGroupMenuCommand ()
{

}

bool CreateGroupMenuCommand::WillModify () const
{
	return !relevantNodes.IsEmpty ();
}

void CreateGroupMenuCommand::DoModification ()
{
	UINodeGroupPtr group (new UINodeGroup (NE::LocString (L"Group")));
	AddGroupCommand command (group, relevantNodes);
	uiManager.ExecuteCommand (command, uiEnvironment);
}

RemoveNodesFromGroupMenuCommand::RemoveNodesFromGroupMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::LocString (L"Remove From Group"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	relevantNodes (relevantNodes)
{

}

RemoveNodesFromGroupMenuCommand::~RemoveNodesFromGroupMenuCommand ()
{

}

bool RemoveNodesFromGroupMenuCommand::WillModify () const
{
	return !relevantNodes.IsEmpty ();
}

void RemoveNodesFromGroupMenuCommand::DoModification ()
{
	RemoveNodesFromGroupCommand command (relevantNodes);
	uiManager.ExecuteCommand (command, uiEnvironment);
}

SetGroupParametersMenuCommand::SetGroupParametersMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group) :
	SingleMenuCommand (NE::LocString (L"Group Settings"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	group (group)
{

}

SetGroupParametersMenuCommand::~SetGroupParametersMenuCommand ()
{

}

bool SetGroupParametersMenuCommand::WillModify () const
{
	return true;
}

void SetGroupParametersMenuCommand::DoModification ()
{
	class GroupParameterInterface : public ParameterInterface
	{
	public:
		struct GroupParameter
		{
			NE::LocString	name;
			ParameterType	type;
		};

		GroupParameterInterface (const UINodeGroupPtr& currentGroup, const NamedColorSet& groupBackgroundColors) :
			currentGroup (currentGroup),
			groupBackgroundColors (groupBackgroundColors),
			groupParameters ({
				{ NE::LocString (L"Group Name"), ParameterType::String },
				{ NE::LocString (L"Group Color"), ParameterType::Enumeration }
			})
		{

		}

		virtual size_t GetParameterCount () const override
		{
			return groupParameters.size ();
		}

		virtual std::wstring GetParameterName (size_t index) const override
		{
			const NE::LocString& name = groupParameters[index].name;
			return name.GetLocalized ();
		}

		virtual NE::ValueConstPtr GetParameterValue (size_t index) const override
		{
			switch (index) {
				case 0:
					return NE::ValuePtr (new NE::StringValue (currentGroup->GetName ().GetLocalized ()));
				case 1:
					return NE::ValuePtr (new NE::IntValue ((int) currentGroup->GetBackgroundColorIndex ()));
				default:
					DBGBREAK ();
					return nullptr;
			}
		}

		virtual std::vector<std::wstring> GetParameterValueChoices (size_t index) const override
		{
			switch (index) {
				case 0:
					DBGBREAK ();
					return {};
				case 1:
					{
						std::vector<std::wstring> result;
						const std::vector<NamedColorSet::NamedColor>& colors = groupBackgroundColors.GetColors ();
						for (const NamedColorSet::NamedColor& color : colors) {
							result.push_back (color.name);
						}
						return result;
					}
				default:
					DBGBREAK ();
					return {};
			}
		}

		virtual const ParameterType& GetParameterType (size_t index) const override
		{
			return groupParameters[index].type;
		}

		virtual bool IsValidParameterValue (size_t index, const NE::ValueConstPtr& value) const override
		{
			switch (index) {
				case 0:
					return NE::Value::IsType<NE::StringValue> (value) && !NE::StringValue::Get (value).empty ();
				case 1:
					return true;
				default:
					DBGBREAK ();
			}
			return false;
		}

		virtual bool SetParameterValue (size_t index, const NE::ValueConstPtr& value) override
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

		virtual void ApplyChanges (NodeUIManager& uiManager, NE::EvaluationEnv&) override
		{
			for (const auto& it : changedParameterValues) {
				switch (it.first) {
				case 0:
					currentGroup->SetName (NE::StringValue::Get (it.second));
					break;
				case 1:
					currentGroup->SetBackgroundColorIndex (NE::IntValue::Get (it.second));
					break;
				default:
					DBGBREAK ();
					break;
				}
			}
			uiManager.RequestRedraw ();
		}

	private:
		UINodeGroupPtr									currentGroup;
		NamedColorSet									groupBackgroundColors;
		std::vector<GroupParameter>						groupParameters;
		std::unordered_map<size_t, NE::ValueConstPtr>	changedParameterValues;
	};

	const NamedColorSet& groupBackgroundColors = uiEnvironment.GetSkinParams ().GetGroupBackgroundColors ();
	ParameterInterfacePtr paramInterface (new GroupParameterInterface (group, groupBackgroundColors));
	if (uiEnvironment.GetEventHandler ().OnParameterSettings (EventHandler::ParameterSettingsType::Group, paramInterface)) {
		ApplyParametersCommand command (paramInterface, uiEnvironment.GetEvaluationEnv ());
		uiManager.ExecuteCommand (command, uiEnvironment);
	}
}

class DisconnectFromInputSlotCommand : public InputSlotCommand
{
public:
	DisconnectFromInputSlotCommand (const NE::LocString& name, const UIOutputSlotConstPtr& slotToDisconnect, const std::wstring& nodeName) :
		InputSlotCommand (name, false),
		slotToDisconnect (slotToDisconnect),
		nodeName (nodeName)
	{

	}

	virtual std::wstring GetName () const override
	{
		std::wstring slotName = slotToDisconnect->GetName ().GetLocalized ();
		return NE::FormatString (NodeCommandBase::GetName (), nodeName.c_str (), slotName.c_str ());
	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UIInputSlotPtr& inputSlot) override
	{
		DisconnectSlotsCommand command (slotToDisconnect, inputSlot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	}

private:
	UIOutputSlotConstPtr	slotToDisconnect;
	std::wstring			nodeName;
};

class DisconnectAllFromInputSlotCommand : public InputSlotCommand
{
public:
	DisconnectAllFromInputSlotCommand (const NE::LocString& name) :
		InputSlotCommand (name, false)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UIInputSlotPtr& inputSlot) override
	{
		DisconnectAllOutputSlotsCommand command (inputSlot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	}
};

class DisconnectFromOutputSlotCommand : public OutputSlotCommand
{
public:
	DisconnectFromOutputSlotCommand (const NE::LocString& name, const UIInputSlotConstPtr& slotToDisconnect, const std::wstring& nodeName) :
		OutputSlotCommand (name, false),
		slotToDisconnect (slotToDisconnect),
		nodeName (nodeName)
	{

	}

	virtual std::wstring GetName () const override
	{
		std::wstring slotName = slotToDisconnect->GetName ().GetLocalized ();
		return NE::FormatString (NodeCommandBase::GetName (), nodeName.c_str (), slotName.c_str ());
	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UIOutputSlotPtr& outputSlot) override
	{
		DisconnectSlotsCommand command (outputSlot, slotToDisconnect);
		uiManager.ExecuteCommand (command, uiEnvironment);
	}

private:
	UIInputSlotConstPtr		slotToDisconnect;
	std::wstring			nodeName;
};

class DisconnectAllFromOutputSlotCommand : public OutputSlotCommand
{
public:
	DisconnectAllFromOutputSlotCommand (const NE::LocString& name) :
		OutputSlotCommand (name, false)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UIOutputSlotPtr& outputSlot) override
	{
		DisconnectAllInputSlotsCommand command (outputSlot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	}
};

class MultiNodeMenuCommand : public SingleMenuCommand
{
public:
	MultiNodeMenuCommand (const NE::LocString& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, NodeCommandPtr& nodeCommand) :
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

	virtual bool WillModify () const override
	{
		return true;
	}

	virtual void DoModification () override
	{
		if (DBGERROR (nodeCommand == nullptr)) {
			return;
		}
		CustomUndoableCommand command ([&] () {
			for (UINodePtr& uiNode : uiNodes) {
				NodeUIManagerNodeInvalidator invalidator (uiManager, uiNode);
				nodeCommand->Do (invalidator, uiEnvironment, uiNode);
			}
		});
		uiManager.ExecuteCommand (command, uiEnvironment);
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
		NE::LocString menuCommandName (nodeGroupCommand->GetName (), NE::LocString::Localization::DoNotLocalize);
		MultiMenuCommandPtr multiCommand (new MultiMenuCommand (menuCommandName));
		nodeGroupCommand->EnumerateChildCommands ([&] (const NodeCommandPtr& nodeCommand) {
			std::shared_ptr<MultiNodeMenuCommand> multiNodeCommand = CreateMultiNodeCommand (nodeCommand);
			multiCommand->AddChildCommand (multiNodeCommand);
		});
		commandStructure.AddCommand (multiCommand);
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
		NE::LocString menuCommandName (nodeCommand->GetName (), NE::LocString::Localization::DoNotLocalize);
		std::shared_ptr<MultiNodeMenuCommand> multiNodeCommand (new MultiNodeMenuCommand (menuCommandName, uiManager, uiEnvironment, nodeCommand));
		relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			UINodePtr uiNode = uiManager.GetNode (nodeId);
			if (nodeCommand->IsApplicableTo (uiNode)) {
				multiNodeCommand->AddNode (uiNode);
			}
			return true;
		});
		return multiNodeCommand;
	}

	NodeUIManager&				uiManager;
	NodeUIEnvironment&			uiEnvironment;
	NE::NodeCollection			relevantNodes;
	MenuCommandStructure		commandStructure;
};

template <typename SlotType, typename CommandType>
class SlotMenuCommand : public SingleMenuCommand
{
public:
	SlotMenuCommand (const NE::LocString& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, SlotType& slot, CommandType& command) :
		SingleMenuCommand (name, command->IsChecked ()),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		slot (slot),
		command (command)
	{

	}

	virtual ~SlotMenuCommand ()
	{

	}

	virtual bool WillModify () const override
	{
		return true;
	}

	virtual void DoModification () override
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
		NE::LocString menuCommandName (slotGroupCommand->GetName (), NE::LocString::Localization::DoNotLocalize);
		MultiMenuCommandPtr multiCommand (new MultiMenuCommand (menuCommandName));
		slotGroupCommand->EnumerateChildCommands ([&] (const SlotCommandType& slotNodeCommand) {
			MenuCommandPtr slotCommand = CreateSlotCommand (slotNodeCommand);
			multiCommand->AddChildCommand (slotCommand);
		});
		commandStructure.AddCommand (multiCommand);
	}

	MenuCommandPtr CreateSlotCommand (SlotCommandType slotNodeCommand)
	{
		NE::LocString menuCommandName (slotNodeCommand->GetName (), NE::LocString::Localization::DoNotLocalize);
		MenuCommandPtr slotCommand (new SlotMenuCommand<SourceSlotType, SlotCommandType> (menuCommandName, uiManager, uiEnvironment, sourceSlot, slotNodeCommand));
		return slotCommand;
	}

	const MenuCommandStructure& GetCommandStructure ()
	{
		return commandStructure;
	}

private:
	NodeUIManager&			uiManager;
	NodeUIEnvironment&		uiEnvironment;
	SourceSlotType			sourceSlot;
	MenuCommandStructure	commandStructure;
};

class DeleteGroupMenuCommand : public SingleMenuCommand
{
public:
	DeleteGroupMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UINodeGroupPtr group) :
		SingleMenuCommand (NE::LocString (L"Ungroup Nodes"), false),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		group (group)
	{
	
	}

	virtual ~DeleteGroupMenuCommand ()
	{
	
	}

	virtual bool WillModify () const override
	{
		return true;
	}

	virtual void DoModification () override
	{
		DeleteGroupCommand command (group);
		uiManager.ExecuteCommand (command, uiEnvironment);
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodeGroupPtr		group;
};

class SelectGroupNodesCommand : public SingleMenuCommand
{
public:
	SelectGroupNodesCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UINodeGroupPtr group) :
		SingleMenuCommand (NE::LocString (L"Select Nodes"), false),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		group (group)
	{

	}

	virtual ~SelectGroupNodesCommand ()
	{

	}

	virtual bool WillModify () const override
	{
		return true;
	}

	virtual void DoModification () override
	{
		Selection selection;
		NE::NodeCollection groupNodes = uiManager.GetGroupNodes (group);
		selection.SetNodes (groupNodes);
		uiManager.SetSelection (selection, uiEnvironment);
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodeGroupPtr		group;
};

class AddNodesToGroupMenuCommand : public SingleMenuCommand
{
public:
	AddNodesToGroupMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& uiNodeGroup, const NE::NodeCollection& relevantNodes) :
		SingleMenuCommand (NE::LocString (L"Add To Group \"%ls\""), false),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		uiNodeGroup (uiNodeGroup),
		relevantNodes (relevantNodes)
	{

	}

	virtual ~AddNodesToGroupMenuCommand ()
	{

	}

	virtual std::wstring GetName () const override
	{
		std::wstring groupName = uiNodeGroup->GetName ().GetLocalized ();
		return NE::FormatString (NE::LocalizeString (SingleMenuCommand::GetName ()), groupName.c_str ());
	}

	virtual bool WillModify () const override
	{
		return !relevantNodes.IsEmpty ();
	}

	virtual void DoModification () override
	{
		AddNodesToGroupCommand command (uiNodeGroup, relevantNodes);
		uiManager.ExecuteCommand (command, uiEnvironment);
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodeGroupPtr		uiNodeGroup;
	NE::NodeCollection	relevantNodes;
};

class AlignNodesMenuCommand : public SingleMenuCommand
{
public:
	AlignNodesMenuCommand (const NE::LocString& name, Alignment alignment, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
		SingleMenuCommand (name, false),
		alignment (alignment),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		relevantNodes (relevantNodes)
	{

	}

	virtual ~AlignNodesMenuCommand ()
	{

	}

	virtual bool WillModify () const override
	{
		return relevantNodes.Count () >= 2;
	}

	virtual void DoModification () override
	{
		std::unordered_map<NE::NodeId, Rect> rects;
		relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			UINodeConstPtr uiNode = uiManager.GetNode (nodeId);
			rects.insert ({ nodeId, uiNode->GetRect (uiEnvironment) });
			return true;
		});

		std::unordered_map<NE::NodeId, Point> offsets = AlignNodes (alignment, rects);
		MoveNodesWithOffsetsCommand command (offsets);
		uiManager.ExecuteCommand (command, uiEnvironment);
	}

private:
	Alignment			alignment;
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NE::NodeCollection	relevantNodes;
};

class DistributeNodesMenuCommand : public SingleMenuCommand
{
public:
	DistributeNodesMenuCommand (const NE::LocString& name, Distribution distribution, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
		SingleMenuCommand (name, false),
		distribution (distribution),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		relevantNodes (relevantNodes)
	{

	}

	virtual ~DistributeNodesMenuCommand ()
	{

	}

	virtual bool WillModify () const override
	{
		return relevantNodes.Count () >= 3;
	}

	virtual void DoModification () override
	{
		std::unordered_map<NE::NodeId, Rect> rects;
		relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			UINodeConstPtr uiNode = uiManager.GetNode (nodeId);
			rects.insert ({ nodeId, uiNode->GetRect (uiEnvironment) });
			return true;
		});

		std::unordered_map<NE::NodeId, Point> offsets = DistributeNodes (distribution, rects);
		MoveNodesWithOffsetsCommand command (offsets);
		uiManager.ExecuteCommand (command, uiEnvironment);
	}

private:
	Distribution		distribution;
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NE::NodeCollection	relevantNodes;
};

NE::NodeCollection GetNodesForCommand (const NodeUIManager& uiManager, const UINodeConstPtr& uiNode)
{
	const Selection& selection = uiManager.GetSelection ();
	const NE::NodeCollection& selectedNodes = selection.GetNodes ();
	if (selectedNodes.Contains (uiNode->GetId ())) {
		return selectedNodes;
	}
	return NE::NodeCollection ({ uiNode->GetId () });
}

MenuCommandStructure CreateEmptyAreaCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position)
{
	MenuCommandStructure commandStructure;
	commandStructure.AddCommand (MenuCommandPtr (new AlignToWindowMenuCommand (uiManager, uiEnvironment)));
	commandStructure.AddCommand (MenuCommandPtr (new CenterToWindowMenuCommand (uiManager, uiEnvironment)));
	commandStructure.AddCommand (MenuCommandPtr (new FitToWindowMenuCommand (uiManager, uiEnvironment)));
	if (uiEnvironment.GetClipboardHandler ().HasClipboardContent ()) {
		commandStructure.AddCommand (MenuCommandPtr (new PasteNodesMenuCommand (uiManager, uiEnvironment, position)));
	}
	return commandStructure;
}

MenuCommandStructure CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeConstPtr& uiNode)
{
	NE::NodeCollection relevantNodes = GetNodesForCommand (uiManager, uiNode);
	NodeCommandStructureBuilder commandStructureBuilder (uiManager, uiEnvironment, relevantNodes);

	commandStructureBuilder.RegisterCommand (MenuCommandPtr (new SetNodeParametersMenuCommand (uiManager, uiEnvironment, uiNode, relevantNodes)));
	uiNode->RegisterCommands (commandStructureBuilder);

	commandStructureBuilder.RegisterCommand (MenuCommandPtr (new CopyNodesMenuCommand (uiManager, uiEnvironment, relevantNodes)));
	commandStructureBuilder.RegisterCommand (MenuCommandPtr (new DeleteNodesMenuCommand (uiManager, uiEnvironment, relevantNodes)));

	MultiMenuCommandPtr groupingMultiCommand (new MultiMenuCommand (NE::LocString (L"Grouping")));
	groupingMultiCommand->AddChildCommand (MenuCommandPtr (new CreateGroupMenuCommand (uiManager, uiEnvironment, relevantNodes)));
	UINodeGroupConstPtr nodeGroup = uiManager.GetNodeGroup (uiNode->GetId ());
	if (nodeGroup != nullptr) {
		groupingMultiCommand->AddChildCommand (MenuCommandPtr (new RemoveNodesFromGroupMenuCommand (uiManager, uiEnvironment, relevantNodes)));
	}
	uiManager.EnumerateNodeGroups ([&] (UINodeGroupPtr group) {
		groupingMultiCommand->AddChildCommand (MenuCommandPtr (new AddNodesToGroupMenuCommand (uiManager, uiEnvironment, group, relevantNodes)));
		return true;
	});
	commandStructureBuilder.RegisterCommand (groupingMultiCommand);

	if (relevantNodes.Count () > 1) {
		MultiMenuCommandPtr alignMultiCommand (new MultiMenuCommand (NE::LocString (L"Align")));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::LocString (L"Horizontal Left"), Alignment::HorizontalLeft, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::LocString (L"Horizontal Right"), Alignment::HorizontalRight, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::LocString (L"Horizontal Center"), Alignment::HorizontalCenter, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::LocString (L"Vertical Top"), Alignment::VerticalTop, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::LocString (L"Vertical Bottom"), Alignment::VerticalBottom, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::LocString (L"Vertical Center"), Alignment::VerticalCenter, uiManager, uiEnvironment, relevantNodes)));
		commandStructureBuilder.RegisterCommand (alignMultiCommand);
	}
	if (relevantNodes.Count () > 2) {
		MultiMenuCommandPtr distributeMultiCommand (new MultiMenuCommand (NE::LocString (L"Distribute")));
		distributeMultiCommand->AddChildCommand (MenuCommandPtr (new DistributeNodesMenuCommand (NE::LocString (L"Horizontal"), Distribution::Horizontal, uiManager, uiEnvironment, relevantNodes)));
		distributeMultiCommand->AddChildCommand (MenuCommandPtr (new DistributeNodesMenuCommand (NE::LocString (L"Vertical"), Distribution::Vertical, uiManager, uiEnvironment, relevantNodes)));
		commandStructureBuilder.RegisterCommand (distributeMultiCommand);
	}

	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot)
{
	SlotCommandStructureBuilder<OutputSlotCommandRegistrator, UIOutputSlotPtr, OutputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, outputSlot);

	if (uiManager.HasConnectedInputSlots (outputSlot)) {
		OutputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<OutputSlotCommandPtr> (NE::LocString (L"Disconnect")));
		uiManager.EnumerateConnectedUIInputSlots (outputSlot, [&] (UIInputSlotConstPtr inputSlot) {
			UINodeConstPtr uiNode = uiManager.GetNode (inputSlot->GetOwnerNodeId ());
			std::wstring outputNodeName = uiNode->GetName ().GetLocalized ();
			disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectFromOutputSlotCommand (NE::LocString (L"%ls (%ls)"), inputSlot, outputNodeName)));
		});
		disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectAllFromOutputSlotCommand (NE::LocString (L"All"))));
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	outputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot)
{
	SlotCommandStructureBuilder<InputSlotCommandRegistrator, UIInputSlotPtr, InputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, inputSlot);

	if (uiManager.HasConnectedOutputSlots (inputSlot)) {
		InputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<InputSlotCommandPtr> (NE::LocString (L"Disconnect")));
		uiManager.EnumerateConnectedUIOutputSlots (inputSlot, [&] (UIOutputSlotConstPtr outputSlot) {
			UINodeConstPtr uiNode = uiManager.GetNode (outputSlot->GetOwnerNodeId ());
			std::wstring inputNodeName = uiNode->GetName ().GetLocalized ();
			disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectFromInputSlotCommand (NE::LocString (L"%ls (%ls)"), outputSlot, inputNodeName)));
		});
		disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectAllFromInputSlotCommand (NE::LocString (L"All"))));
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	inputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateNodeGroupCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group)
{
	MenuCommandStructure commandStructure;
	commandStructure.AddCommand (MenuCommandPtr (new SetGroupParametersMenuCommand (uiManager, uiEnvironment, group)));
	commandStructure.AddCommand (MenuCommandPtr (new DeleteGroupMenuCommand (uiManager, uiEnvironment, group)));
	commandStructure.AddCommand (MenuCommandPtr (new SelectGroupNodesCommand (uiManager, uiEnvironment, group)));
	return commandStructure;
}

}
