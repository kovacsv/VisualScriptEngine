#include "NUIE_NodeCommonMenuCommands.hpp"
#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_EventHandler.hpp"
#include "NUIE_SkinParams.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Localization.hpp"
#include "NE_Debug.hpp"

#include <limits>
#include <algorithm>

namespace NUIE
{

DeleteNodesMenuCommand::DeleteNodesMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::String (L"Delete Nodes"), false),
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

CopyNodesMenuCommand::CopyNodesMenuCommand (NodeUIManager& uiManager, NodeUIInteractionEnvironment& interactionEnv, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::String (L"Copy Nodes"), false),
	uiManager (uiManager),
	interactionEnv (interactionEnv),
	relevantNodes (relevantNodes)
{

}

CopyNodesMenuCommand::~CopyNodesMenuCommand ()
{

}

void CopyNodesMenuCommand::Do ()
{
	CopyNodesCommand command (relevantNodes, interactionEnv.GetClipboardHandler ());
	uiManager.ExecuteCommand (command);
}

PasteNodesMenuCommand::PasteNodesMenuCommand (NodeUIManager& uiManager, NodeUIInteractionEnvironment& interactionEnv, const Point& position) :
	SingleMenuCommand (NE::String (L"Paste Nodes"), false),
	uiManager (uiManager),
	interactionEnv (interactionEnv),
	position (position)
{

}

PasteNodesMenuCommand::~PasteNodesMenuCommand ()
{

}

void PasteNodesMenuCommand::Do ()
{
	PasteNodesCommand command (position, interactionEnv.GetClipboardHandler ());
	uiManager.ExecuteCommand (command);
}

AlignToWindowMenuCommand::AlignToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::String (L"Align To Window"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{
}

AlignToWindowMenuCommand::~AlignToWindowMenuCommand ()
{
}

void AlignToWindowMenuCommand::Do ()
{
	uiManager.AlignToWindow (uiEnvironment);
}

CenterToWindowMenuCommand::CenterToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::String (L"Center To Window"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{
}

CenterToWindowMenuCommand::~CenterToWindowMenuCommand ()
{
}

void CenterToWindowMenuCommand::Do ()
{
	uiManager.CenterToWindow (uiEnvironment);
}

FitToWindowMenuCommand::FitToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::String (L"Fit To Window"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment)
{
}

FitToWindowMenuCommand::~FitToWindowMenuCommand ()
{
}

void FitToWindowMenuCommand::Do ()
{
	uiManager.FitToWindow (uiEnvironment);
}

UndoMenuCommand::UndoMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::String (L"Undo"), false),
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
	SingleMenuCommand (NE::String (L"Redo"), false),
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

SetParametersMenuCommand::SetParametersMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& currentNode, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::String (L"Node Settings"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	currentNode (currentNode),
	relevantNodes (relevantNodes),
	relevantParameters ()
{
	DBGASSERT (relevantNodes.Contains (currentNode->GetId ()));
}

SetParametersMenuCommand::~SetParametersMenuCommand ()
{

}

void SetParametersMenuCommand::Do ()
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
		}

		virtual size_t GetParameterCount () const override
		{
			return paramList.GetParameterCount ();
		}

		virtual std::wstring GetParameterName (size_t index) const override
		{
			static const std::wstring InvalidParameterName = L"";
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
			for (const NE::String& choice : parameter->GetValueChoices ()) {
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

	private:
		NodeParameterList&								paramList;
		const UINodePtr&								currentNode;
		std::unordered_map<size_t, NE::ValueConstPtr>	changedParameterValues;
	};

	RegisterCommonParameters (uiManager, relevantNodes, relevantParameters);
	std::shared_ptr<NodeSelectionParameterInterface> paramInterface (new NodeSelectionParameterInterface (relevantParameters, currentNode));
	if (uiEnvironment.GetEventHandler ().OnParameterSettings (paramInterface, currentNode)) {
		CustomUndoableCommand command ([&] () {
			paramInterface->ApplyChanges (uiManager, uiEnvironment, relevantNodes);
		});
		uiManager.ExecuteCommand (command);
	}
}

CreateGroupMenuCommand::CreateGroupMenuCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::String (L"Create New Group"), false),
	uiManager (uiManager),
	relevantNodes (relevantNodes)
{

}

CreateGroupMenuCommand::~CreateGroupMenuCommand ()
{

}

void CreateGroupMenuCommand::Do ()
{
	UINodeGroupPtr group (new UINodeGroup (NE::String (L"Group")));
	AddGroupCommand command (group, relevantNodes);
	uiManager.ExecuteCommand (command);
}

SetGroupParametersMenuCommand::SetGroupParametersMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group) :
	SingleMenuCommand (NE::String (L"Group Settings"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	group (group)
{

}

SetGroupParametersMenuCommand::~SetGroupParametersMenuCommand ()
{

}

void SetGroupParametersMenuCommand::Do ()
{
	class GroupParameterInterface : public ParameterInterface
	{
	public:
		struct GroupParameter
		{
			std::wstring	name;
			ParameterType	type;
		};

		GroupParameterInterface (const UINodeGroupPtr& currentGroup, const NamedColorSet& groupBackgroundColors) :
			currentGroup (currentGroup),
			groupBackgroundColors (groupBackgroundColors),
			groupParameters ({
				{ L"Group Name", ParameterType::String },
				{ L"Group Color", ParameterType::Enumeration }
			})
		{

		}

		void ApplyChanges (NodeUIManager& uiManager)
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

		virtual size_t GetParameterCount () const override
		{
			return groupParameters.size ();
		}

		virtual std::wstring GetParameterName (size_t index) const override
		{
			return groupParameters[index].name;
		}

		virtual NE::ValueConstPtr GetParameterValue (size_t index) const override
		{
			switch (index) {
				case 0:
					return NE::ValuePtr (new NE::StringValue (currentGroup->GetName ()));
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

	private:
		const UINodeGroupPtr&							currentGroup;
		const NamedColorSet&							groupBackgroundColors;
		std::vector<GroupParameter>						groupParameters;
		std::unordered_map<size_t, NE::ValueConstPtr>	changedParameterValues;
	};

	const NamedColorSet& groupBackgroundColors = uiEnvironment.GetSkinParams ().GetGroupBackgroundColors ();
	std::shared_ptr<GroupParameterInterface> paramInterface (new GroupParameterInterface (group, groupBackgroundColors));
	if (uiEnvironment.GetEventHandler ().OnParameterSettings (paramInterface, group)) {
		CustomUndoableCommand command ([&] () {
			paramInterface->ApplyChanges (uiManager);
		});
		uiManager.ExecuteCommand (command);
	}
}

class DisconnectFromInputSlotCommand : public InputSlotCommand
{
public:
	DisconnectFromInputSlotCommand (const NE::String& name, const UIOutputSlotConstPtr& slotToDisconnect, const std::wstring& nodeName) :
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

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIInputSlotConstPtr& inputSlot) override
	{
		DisconnectSlotsCommand command (slotToDisconnect, inputSlot);
		uiManager.ExecuteCommand (command);
	}

private:
	UIOutputSlotConstPtr	slotToDisconnect;
	std::wstring			nodeName;
};

class DisconnectAllFromInputSlotCommand : public InputSlotCommand
{
public:
	DisconnectAllFromInputSlotCommand (const NE::String& name) :
		InputSlotCommand (name, false)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIInputSlotConstPtr& inputSlot) override
	{
		DisconnectAllOutputSlotsCommand command (inputSlot);
		uiManager.ExecuteCommand (command);
	}
};

class DisconnectFromOutputSlotCommand : public OutputSlotCommand
{
public:
	DisconnectFromOutputSlotCommand (const NE::String& name, const UIInputSlotConstPtr& slotToDisconnect, const std::wstring& nodeName) :
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

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIOutputSlotConstPtr& outputSlot) override
	{
		DisconnectSlotsCommand command (outputSlot, slotToDisconnect);
		uiManager.ExecuteCommand (command);
	}

private:
	UIInputSlotConstPtr		slotToDisconnect;
	std::wstring			nodeName;
};

class DisconnectAllFromOutputSlotCommand : public OutputSlotCommand
{
public:
	DisconnectAllFromOutputSlotCommand (const NE::String& name) :
		OutputSlotCommand (name, false)
	{

	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIOutputSlotConstPtr& outputSlot) override
	{
		DisconnectAllInputSlotsCommand command (outputSlot);
		uiManager.ExecuteCommand (command);
	}
};

class MultiNodeMenuCommand : public SingleMenuCommand
{
public:
	MultiNodeMenuCommand (const NE::String& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, NodeCommandPtr& nodeCommand) :
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
				NodeUIManagerNodeInvalidator invalidator (uiManager, uiNode);
				nodeCommand->Do (invalidator, uiEnvironment, uiNode);
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
		NE::String menuCommandName (nodeGroupCommand->GetName (), NE::String::Localization::NonLocalizable);
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
		NE::String menuCommandName (nodeCommand->GetName (), NE::String::Localization::NonLocalizable);
		std::shared_ptr<MultiNodeMenuCommand> multiNodeCommand (new MultiNodeMenuCommand (menuCommandName, uiManager, uiEnvironment, nodeCommand));
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
class SlotMenuCommand : public SingleMenuCommand
{
public:
	SlotMenuCommand (const NE::String& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, SlotType& slot, CommandType& command) :
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
		NE::String menuCommandName (slotGroupCommand->GetName (), NE::String::Localization::NonLocalizable);
		MultiMenuCommandPtr multiCommand (new MultiMenuCommand (menuCommandName));
		slotGroupCommand->EnumerateChildCommands ([&] (const SlotCommandType& slotNodeCommand) {
			MenuCommandPtr slotCommand = CreateSlotCommand (slotNodeCommand);
			multiCommand->AddChildCommand (slotCommand);
		});
		commandStructure.AddCommand (multiCommand);
	}

	MenuCommandPtr CreateSlotCommand (SlotCommandType slotNodeCommand)
	{
		NE::String menuCommandName (slotNodeCommand->GetName (), NE::String::Localization::NonLocalizable);
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
	DeleteGroupMenuCommand (NodeUIManager& uiManager, UINodeGroupPtr group) :
		SingleMenuCommand (NE::String (L"Ungroup Nodes"), false),
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

class SelectGroupNodesCommand : public SingleMenuCommand
{
public:
	SelectGroupNodesCommand (NodeUIManager& uiManager, UINodeGroupPtr group) :
		SingleMenuCommand (NE::String (L"Select Nodes"), false),
		uiManager (uiManager),
		group (group)
	{

	}

	virtual ~SelectGroupNodesCommand ()
	{

	}

	virtual void Do () override
	{
		NE::NodeCollection groupNodes = uiManager.GetUIGroupNodes (group);
		uiManager.SetSelectedNodes (groupNodes);
	}

private:
	NodeUIManager&		uiManager;
	UINodeGroupPtr		group;
};

class RemoveNodesFromGroupMenuCommand : public SingleMenuCommand
{
public:
	RemoveNodesFromGroupMenuCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
		SingleMenuCommand (NE::String (L"Remove From Group"), false),
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

class AddNodesToGroupMenuCommand : public SingleMenuCommand
{
public:
	AddNodesToGroupMenuCommand (NodeUIManager& uiManager, const UINodeGroupPtr& uiNodeGroup, const NE::NodeCollection& relevantNodes) :
		SingleMenuCommand (NE::String (L"Add To Group \"%ls\""), false),
		uiManager (uiManager),
		uiNodeGroup (uiNodeGroup),
		relevantNodes (relevantNodes)
	{

	}

	virtual ~AddNodesToGroupMenuCommand ()
	{

	}

	virtual std::wstring GetName () const override
	{
		return NE::FormatString (NE::LocalizeString (SingleMenuCommand::GetName ()), uiNodeGroup->GetName ().c_str ());
	}

	virtual void Do () override
	{
		AddNodesToGroupCommand command (uiNodeGroup, relevantNodes);
		uiManager.ExecuteCommand (command);
	}

private:
	NodeUIManager&		uiManager;
	UINodeGroupPtr		uiNodeGroup;
	NE::NodeCollection	relevantNodes;
};

class AlignNodesMenuCommand : public SingleMenuCommand
{
public:
	enum class Mode
	{
		Left,
		Right,
		Top,
		Bottom,
		HCenter,
		VCenter
	};

	AlignNodesMenuCommand (const NE::String& name, Mode mode, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
		SingleMenuCommand (name, false),
		mode (mode),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		relevantNodes (relevantNodes)
	{

	}

	virtual ~AlignNodesMenuCommand ()
	{

	}

	virtual void Do () override
	{
		if (DBGERROR (relevantNodes.Count () < 2)) {
			return;
		}

		std::vector<Rect> nodeRects;
		relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			UINodePtr uiNode = uiManager.GetUINode (nodeId);
			nodeRects.push_back (uiNode->GetNodeRect (uiEnvironment));
			return true;
		});
		std::vector<Point> offsets;
		if (mode == Mode::Left) {
			auto min = std::min_element (nodeRects.begin (), nodeRects.end (), [&] (const Rect& a, const Rect& b) {
				return a.GetLeft () < b.GetLeft ();
			});
			Rect minRect = *min;
			for (const Rect& nodeRect : nodeRects) {
				double offsetX = minRect.GetLeft () - nodeRect.GetLeft ();
				offsets.push_back (Point (offsetX, 0.0));
			}
		} else if (mode == Mode::Right) {
			auto max = std::max_element (nodeRects.begin (), nodeRects.end (), [&] (const Rect& a, const Rect& b) {
				return a.GetRight () < b.GetRight ();
			});
			Rect maxRect = *max;
			for (const Rect& nodeRect : nodeRects) {
				double offsetX = maxRect.GetRight () - nodeRect.GetRight ();
				offsets.push_back (Point (offsetX, 0.0));
			}
		} else if (mode == Mode::Top) {
			auto min = std::min_element (nodeRects.begin (), nodeRects.end (), [&] (const Rect& a, const Rect& b) {
				return a.GetTop () < b.GetTop ();
			});
			Rect minRect = *min;
			for (const Rect& nodeRect : nodeRects) {
				double offsetY = minRect.GetTop () - nodeRect.GetTop ();
				offsets.push_back (Point (0.0, offsetY));
			}
		} else if (mode == Mode::Bottom) {
			auto max = std::max_element (nodeRects.begin (), nodeRects.end (), [&] (const Rect& a, const Rect& b) {
				return a.GetBottom () < b.GetBottom ();
			});
			Rect maxRect = *max;
			for (const Rect& nodeRect : nodeRects) {
				double offsetY = maxRect.GetBottom () - nodeRect.GetBottom ();
				offsets.push_back (Point (0.0, offsetY));
			}
		} else if (mode == Mode::HCenter) {
			double avgCenterX = 0.0;
			for (const Rect& nodeRect : nodeRects) {
				avgCenterX += nodeRect.GetCenter ().GetX ();
			}
			avgCenterX /= (double) nodeRects.size ();
			for (const Rect& nodeRect : nodeRects) {
				double offsetX = avgCenterX - nodeRect.GetCenter ().GetX ();
				offsets.push_back (Point (offsetX, 0.0));
			}
		} else if (mode == Mode::VCenter) {
			double avgCenterY = 0.0;
			for (const Rect& nodeRect : nodeRects) {
				avgCenterY += nodeRect.GetCenter ().GetY ();
			}
			avgCenterY /= (double) nodeRects.size ();
			for (const Rect& nodeRect : nodeRects) {
				double offsetY = avgCenterY - nodeRect.GetCenter ().GetY ();
				offsets.push_back (Point (0.0, offsetY));
			}
		} else {
			DBGBREAK ();
			return;
		}
		MoveNodesWithOffsetsCommand command (relevantNodes, offsets);
		uiManager.ExecuteCommand (command);
	}

private:
	Mode				mode;
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
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

MenuCommandStructure CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode)
{
	NE::NodeCollection relevantNodes = GetNodesForCommand (uiManager, uiNode);
	NodeCommandStructureBuilder commandStructureBuilder (uiManager, uiEnvironment, relevantNodes);

	commandStructureBuilder.RegisterCommand (MenuCommandPtr (new SetParametersMenuCommand (uiManager, uiEnvironment, uiNode, relevantNodes)));
	uiNode->RegisterCommands (commandStructureBuilder);

	commandStructureBuilder.RegisterCommand (MenuCommandPtr (new CopyNodesMenuCommand (uiManager, uiEnvironment, relevantNodes)));
	commandStructureBuilder.RegisterCommand (MenuCommandPtr (new DeleteNodesMenuCommand (uiManager, uiEnvironment, relevantNodes)));

	MultiMenuCommandPtr groupingMultiCommand (new MultiMenuCommand (NE::String (L"Grouping")));
	groupingMultiCommand->AddChildCommand (MenuCommandPtr (new CreateGroupMenuCommand (uiManager, relevantNodes)));
	UINodeGroupConstPtr nodeGroup = uiManager.GetUINodeGroup (uiNode->GetId ());
	if (nodeGroup != nullptr) {
		groupingMultiCommand->AddChildCommand (MenuCommandPtr (new RemoveNodesFromGroupMenuCommand (uiManager, relevantNodes)));
	}
	uiManager.EnumerateUINodeGroups ([&] (const UINodeGroupPtr& group) {
		groupingMultiCommand->AddChildCommand (MenuCommandPtr (new AddNodesToGroupMenuCommand (uiManager, group, relevantNodes)));
		return true;
	});
	commandStructureBuilder.RegisterCommand (groupingMultiCommand);

	if (relevantNodes.Count () > 1) {
		MultiMenuCommandPtr alignMultiCommand (new MultiMenuCommand (NE::String (L"Aligning")));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::String (L"Left"), AlignNodesMenuCommand::Mode::Left, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::String (L"Right"), AlignNodesMenuCommand::Mode::Right, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::String (L"Top"), AlignNodesMenuCommand::Mode::Top, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::String (L"Bottom"), AlignNodesMenuCommand::Mode::Bottom, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::String (L"Horizontal Center"), AlignNodesMenuCommand::Mode::HCenter, uiManager, uiEnvironment, relevantNodes)));
		alignMultiCommand->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::String (L"Vertical Center"), AlignNodesMenuCommand::Mode::VCenter, uiManager, uiEnvironment, relevantNodes)));
		commandStructureBuilder.RegisterCommand (alignMultiCommand);
	}

	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotConstPtr& outputSlot)
{
	SlotCommandStructureBuilder<OutputSlotCommandRegistrator, UIOutputSlotConstPtr, OutputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, outputSlot);

	if (uiManager.HasConnectedInputSlots (outputSlot)) {
		OutputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<OutputSlotCommandPtr> (NE::String (L"Disconnect")));
		uiManager.EnumerateConnectedUIInputSlots (outputSlot, [&] (UIInputSlotConstPtr inputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (inputSlot->GetOwnerNodeId ());
			std::wstring outputNodeName = uiNode->GetNodeName ().GetLocalized ();
			disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectFromOutputSlotCommand (NE::String (L"%ls (%ls)"), inputSlot, outputNodeName)));
		});
		disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectAllFromOutputSlotCommand (NE::String (L"All"))));
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	outputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotConstPtr& inputSlot)
{
	SlotCommandStructureBuilder<InputSlotCommandRegistrator, UIInputSlotConstPtr, InputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, inputSlot);

	if (uiManager.HasConnectedOutputSlots (inputSlot)) {
		InputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<InputSlotCommandPtr> (NE::String (L"Disconnect")));
		uiManager.EnumerateConnectedUIOutputSlots (inputSlot, [&] (UIOutputSlotConstPtr outputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (outputSlot->GetOwnerNodeId ());
			std::wstring inputNodeName = uiNode->GetNodeName ().GetLocalized ();
			disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectFromInputSlotCommand (NE::String (L"%ls (%ls)"), outputSlot, inputNodeName)));
		});
		disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectAllFromInputSlotCommand (NE::String (L"All"))));
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	inputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateNodeGroupCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group)
{
	MenuCommandStructure commandStructure;
	commandStructure.AddCommand (MenuCommandPtr (new SetGroupParametersMenuCommand (uiManager, uiEnvironment, group)));
	commandStructure.AddCommand (MenuCommandPtr (new DeleteGroupMenuCommand (uiManager, group)));
	commandStructure.AddCommand (MenuCommandPtr (new SelectGroupNodesCommand (uiManager, group)));
	return commandStructure;
}

}
