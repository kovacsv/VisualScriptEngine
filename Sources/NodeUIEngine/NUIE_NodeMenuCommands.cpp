#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeMenuCommandRegistrator.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_EventHandlers.hpp"
#include "NUIE_SkinParams.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Localization.hpp"
#include "NE_Debug.hpp"

#include <limits>
#include <algorithm>

namespace NUIE
{

DeleteNodesMenuCommand::DeleteNodesMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
	SingleMenuCommand (NE::Localize (L"Delete Nodes"), false),
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
	SingleMenuCommand (NE::Localize (L"Copy Nodes"), false),
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
	SingleMenuCommand (NE::Localize (L"Paste Nodes"), false),
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

AlignToWindowMenuCommand::AlignToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment) :
	SingleMenuCommand (NE::Localize (L"Align To Window"), false),
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
	SingleMenuCommand (NE::Localize (L"Center To Window"), false),
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
	SingleMenuCommand (NE::Localize (L"Fit To Window"), false),
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
	SingleMenuCommand (NE::Localize (L"Undo"), false),
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
	SingleMenuCommand (NE::Localize (L"Redo"), false),
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
	SingleMenuCommand (NE::Localize (L"Set Parameters"), false),
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
	if (uiEnvironment.GetEventHandlers ().OnParameterSettings (paramInterface)) {
		CustomUndoableCommand command ([&] () {
			paramInterface->ApplyChanges (uiManager, uiEnvironment, relevantNodes);
		});
		uiManager.ExecuteCommand (command);
	}
}

SetGroupParametersCommand::SetGroupParametersCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group) :
	SingleMenuCommand (NE::Localize (L"Set Parameters"), false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	group (group)
{

}

SetGroupParametersCommand::~SetGroupParametersCommand ()
{

}

void SetGroupParametersCommand::Do ()
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
				{ NE::Localize (L"Name"), ParameterType::String },
				{ NE::Localize (L"Color"), ParameterType::Enumeration }
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

		virtual const std::wstring& GetParameterName (size_t index) const override
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
	if (uiEnvironment.GetEventHandlers ().OnParameterSettings (paramInterface)) {
		CustomUndoableCommand command ([&] () {
			paramInterface->ApplyChanges (uiManager);
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

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIInputSlotConstPtr& inputSlot) override
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

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIInputSlotConstPtr& inputSlot) override
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

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIOutputSlotConstPtr& outputSlot) override
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

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UIOutputSlotConstPtr& outputSlot) override
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
		SingleMenuCommand (NE::Localize (L"Create New Group"), false),
		uiManager (uiManager),
		relevantNodes (relevantNodes)
	{
	
	}

	virtual ~CreateGroupMenuCommand ()
	{
	
	}

	virtual void Do () override
	{
		UINodeGroupPtr group (new UINodeGroup (NE::Localize (L"Group")));
		AddGroupCommand command (group, relevantNodes);
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
		SingleMenuCommand (NE::Localize (L"Delete Group"), false),
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
		SingleMenuCommand (NE::Localize (L"Select Nodes"), false),
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
		SingleMenuCommand (NE::Localize (L"Remove From Group"), false),
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
		SingleMenuCommand (NE::FormatString (NE::Localize (L"Add To Group \"%ls\""), uiNodeGroup->GetName ().c_str ()), false),
		uiManager (uiManager),
		uiNodeGroup (uiNodeGroup),
		relevantNodes (relevantNodes)
	{

	}

	virtual ~AddNodesToGroupMenuCommand ()
	{

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

	AlignNodesMenuCommand (const std::wstring& name, Mode mode, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes) :
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
		MoveNodesCommand command (relevantNodes, offsets);
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

	GroupMenuCommandPtr groupingCommandGroup (new GroupMenuCommand (NE::Localize (L"Grouping")));
	groupingCommandGroup->AddChildCommand (MenuCommandPtr (new CreateGroupMenuCommand (uiManager, relevantNodes)));
	UINodeGroupConstPtr nodeGroup = uiManager.GetUINodeGroup (uiNode->GetId ());
	if (nodeGroup != nullptr) {
		groupingCommandGroup->AddChildCommand (MenuCommandPtr (new RemoveNodesFromGroupMenuCommand (uiManager, relevantNodes)));
	}
	uiManager.EnumerateUINodeGroups ([&] (const UINodeGroupPtr& group) {
		groupingCommandGroup->AddChildCommand (MenuCommandPtr (new AddNodesToGroupMenuCommand (uiManager, group, relevantNodes)));
		return true;
	});
	commandStructureBuilder.RegisterCommand (groupingCommandGroup);

	if (relevantNodes.Count () > 1) {
		GroupMenuCommandPtr alignCommandGroup (new GroupMenuCommand (NE::Localize (L"Aligning")));
		alignCommandGroup->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::Localize (L"Align Left"), AlignNodesMenuCommand::Mode::Left, uiManager, uiEnvironment, relevantNodes)));
		alignCommandGroup->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::Localize (L"Align Right"), AlignNodesMenuCommand::Mode::Right, uiManager, uiEnvironment, relevantNodes)));
		alignCommandGroup->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::Localize (L"Align Top"), AlignNodesMenuCommand::Mode::Top, uiManager, uiEnvironment, relevantNodes)));
		alignCommandGroup->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::Localize (L"Align Bottom"), AlignNodesMenuCommand::Mode::Bottom, uiManager, uiEnvironment, relevantNodes)));
		alignCommandGroup->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::Localize (L"Center Horizontically"), AlignNodesMenuCommand::Mode::HCenter, uiManager, uiEnvironment, relevantNodes)));
		alignCommandGroup->AddChildCommand (MenuCommandPtr (new AlignNodesMenuCommand (NE::Localize (L"Center Vertically"), AlignNodesMenuCommand::Mode::VCenter, uiManager, uiEnvironment, relevantNodes)));
		commandStructureBuilder.RegisterCommand (alignCommandGroup);
	}

	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotConstPtr& outputSlot)
{
	SlotCommandStructureBuilder<OutputSlotCommandRegistrator, UIOutputSlotConstPtr, OutputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, outputSlot);

	if (uiManager.HasConnectedInputSlots (outputSlot)) {
		OutputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<OutputSlotCommandPtr> (NE::Localize (L"Disconnect")));
		uiManager.EnumerateConnectedInputSlots (outputSlot, [&] (UIInputSlotConstPtr inputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (inputSlot->GetOwnerNodeId ());
			std::wstring disconnectCommandName = NE::FormatString (L"%ls (%ls)", uiNode->GetNodeName ().c_str (), inputSlot->GetName ().c_str ());
			disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectFromOutputSlotMenuCommand (disconnectCommandName, inputSlot)));
		});
		disconnectGroup->AddChildCommand (OutputSlotCommandPtr (new DisconnectAllFromOutputSlotMenuCommand (NE::Localize (L"All"))));
		commandStructureBuilder.RegisterSlotGroupCommand (disconnectGroup);
	}

	outputSlot->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

MenuCommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotConstPtr& inputSlot)
{
	SlotCommandStructureBuilder<InputSlotCommandRegistrator, UIInputSlotConstPtr, InputSlotCommandPtr> commandStructureBuilder (uiManager, uiEnvironment, inputSlot);

	if (uiManager.HasConnectedOutputSlots (inputSlot)) {
		InputSlotGroupCommandPtr disconnectGroup (new NodeGroupCommand<InputSlotCommandPtr> (NE::Localize (L"Disconnect")));
		uiManager.EnumerateConnectedOutputSlots (inputSlot, [&] (UIOutputSlotConstPtr outputSlot) {
			UINodeConstPtr uiNode = uiManager.GetUINode (outputSlot->GetOwnerNodeId ());
			std::wstring disconnectCommandName = NE::FormatString (L"%ls (%ls)", uiNode->GetNodeName ().c_str (), outputSlot->GetName ().c_str ());
			disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectFromInputSlotMenuCommand (disconnectCommandName, outputSlot)));
		});
		disconnectGroup->AddChildCommand (InputSlotCommandPtr (new DisconnectAllFromInputSlotMenuCommand (NE::Localize (L"All"))));
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
	commandStructure.AddCommand (MenuCommandPtr (new SelectGroupNodesCommand (uiManager, group)));
	return commandStructure;
}

}
