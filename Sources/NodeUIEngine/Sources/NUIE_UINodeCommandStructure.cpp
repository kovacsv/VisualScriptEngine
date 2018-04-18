#include "NUIE_UINodeCommandStructure.hpp"
#include "NUIE_UINodeCommands.hpp"
#include "NUIE_UINodeParameters.hpp"
#include "NUIE_EventHandlers.hpp"
#include "NE_Debug.hpp"

#include <limits>

namespace NUIE
{

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
			nodeCommand->Do (uiManager, uiEnvironment, uiNode);
		}
	}

private:
	NodeUIManager&				uiManager;
	NodeUIEnvironment&			uiEnvironment;
	NodeCommandPtr				nodeCommand;
	std::vector<UINodePtr>		uiNodes;
};

class SetParametersCommand : public SingleCommand
{
public:
	SetParametersCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& currentNode, const NodeCollection& relevantNodes) :
		SingleCommand (name, false),
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
		class SelectionParameterAccessor : public NodeParameterAccessor
		{
		public:
			SelectionParameterAccessor (NodeParameterList& paramList, const UINodePtr& currentNode) :
				paramList (paramList),
				currentNode (currentNode)
			{
			
			}

			void ApplyChanges (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NodeCollection& relevantNodes)
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

			virtual const ParameterType& GetParameterType (size_t index) const override
			{
				NodeParameterPtr parameter = paramList.GetParameter (index);
				if (DBGERROR (parameter == nullptr)) {
					return ParameterType::Undefined;
				}
				return parameter->GetType ();
			}

			virtual bool SetParameterValue (size_t index, const NE::ValuePtr& value) override
			{
				NodeParameterPtr parameter = paramList.GetParameter (index);
				if (DBGERROR (parameter == nullptr)) {
					return false;
				}

				if (!parameter->CanSetValue (currentNode, value)) {
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
		std::shared_ptr<SelectionParameterAccessor> paramAccessor (new SelectionParameterAccessor (relevantParameters, currentNode));
		if (uiEnvironment.GetEventHandlers ().OnParameterSettings (paramAccessor)) {
			paramAccessor->ApplyChanges (uiManager, uiEnvironment, relevantNodes);
		}
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodePtr			currentNode;
	NodeCollection		relevantNodes;
	NodeParameterList	relevantParameters;
};

class DeleteNodesCommand : public SingleCommand
{
public:
	DeleteNodesCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NodeCollection& relevantNodes) :
		SingleCommand (name, false),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		relevantNodes (relevantNodes)
	{

	}

	virtual ~DeleteNodesCommand ()
	{

	}

	virtual void Do () override
	{
		relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			uiManager.DeleteNode (nodeId, uiEnvironment.GetEvaluationEnv ());
			return true;
		});
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NodeCollection		relevantNodes;
};

class CopyNodesCommand : public SingleCommand
{
public:
	CopyNodesCommand (const std::wstring& name, NodeUIManager& uiManager, const NodeCollection& relevantNodes) :
		SingleCommand (name, false),
		uiManager (uiManager),
		relevantNodes (relevantNodes)
	{

	}

	virtual ~CopyNodesCommand ()
	{

	}

	virtual void Do () override
	{
		uiManager.Copy (relevantNodes);
	}

private:
	NodeUIManager&		uiManager;
	NodeCollection		relevantNodes;
};


class PasteNodesCommand : public SingleCommand
{
public:
	PasteNodesCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position) :
		SingleCommand (name, false),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		position (position)
	{

	}

	virtual ~PasteNodesCommand ()
	{

	}

	virtual void Do () override
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

		NodeCollection newSelection;
		centerPosition = centerPosition / (double) newNodes.size ();
		Point nodeOffset = position - centerPosition;
		for (UINodePtr& uiNode : newNodes) {
			Point nodePosition = uiNode->GetNodePosition ();
			uiNode->SetNodePosition (nodePosition + nodeOffset);
			newSelection.Insert (uiNode->GetId ());
		}

		uiManager.SetSelectedNodes (newSelection);
	}

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	Point				position;
};

class NodeCommandStructureBuilder : public NodeCommandRegistrator
{
public:
	NodeCommandStructureBuilder (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NodeCollection& relevantNodes) :
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
		GroupCommandPtr groupCommand (new GroupCommand (nodeGroupCommand->GetName ()));
		nodeGroupCommand->EnumerateChildCommands ([&] (const NodeCommandPtr& nodeCommand) {
			std::shared_ptr<MultiNodeCommand> multiNodeCommand = CreateMultiNodeCommand (nodeCommand);
			groupCommand->AddChildCommand (multiNodeCommand);
		});
		commandStructure.AddCommand (groupCommand);
	}

	void RegisterCommand (CommandPtr command)
	{
		commandStructure.AddCommand (command);
	}

	CommandStructure& GetCommandStructure ()
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

	NodeUIManager&			uiManager;
	NodeUIEnvironment&		uiEnvironment;
	const NodeCollection&	relevantNodes;
	CommandStructure		commandStructure;
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

NodeCollection GetNodesForCommand (const NodeUIManager& uiManager, const UINodePtr& uiNode)
{
	const NodeCollection& selectedNodes = uiManager.GetSelectedNodes ();
	if (selectedNodes.Contains (uiNode->GetId ())) {
		return selectedNodes;
	}
	return NodeCollection (uiNode->GetId ());
}

CommandStructure CreateEmptyAreaCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position)
{
	CommandStructure commandStructure;
	if (uiManager.CanPaste ()) {
		commandStructure.AddCommand (CommandPtr (new PasteNodesCommand (L"Paste Nodes", uiManager, uiEnvironment, position)));
	}
	return commandStructure;
}

CommandStructure CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode)
{
	NodeCollection relevantNodes = GetNodesForCommand (uiManager, uiNode);
	NodeCommandStructureBuilder commandStructureBuilder (uiManager, uiEnvironment, relevantNodes);
	commandStructureBuilder.RegisterCommand (CommandPtr (new SetParametersCommand (L"Set Parameters", uiManager, uiEnvironment, uiNode, relevantNodes)));
	commandStructureBuilder.RegisterCommand (CommandPtr (new CopyNodesCommand (L"Copy Nodes", uiManager, relevantNodes)));
	commandStructureBuilder.RegisterCommand (CommandPtr (new DeleteNodesCommand (L"Delete Nodes", uiManager, uiEnvironment, relevantNodes)));
	uiNode->RegisterCommands (commandStructureBuilder);
	return commandStructureBuilder.GetCommandStructure ();
}

CommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot)
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

CommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot)
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

}
