#include "NUIE_NodeUIManagerCommands.hpp"

namespace NUIE
{

UndoableCommand::UndoableCommand () :
	NodeUIManagerCommand ()
{
}

UndoableCommand::~UndoableCommand ()
{

}

bool UndoableCommand::IsUndoable () const
{
	return true;
}

NotUndoableCommand::NotUndoableCommand () :
	NodeUIManagerCommand ()
{
}

NotUndoableCommand::~NotUndoableCommand ()
{

}

bool NotUndoableCommand::IsUndoable () const
{
	return false;
}

AddNodeCommand::AddNodeCommand (const UINodePtr& uiNode, NE::EvaluationEnv& evaluationEnv) :
	UndoableCommand (),
	uiNode (uiNode),
	evaluationEnv (evaluationEnv)
{
}

void AddNodeCommand::Do (NodeUIManager& uiManager)
{
	uiManager.AddNode (uiNode, evaluationEnv);
}

DeleteNodesCommand::DeleteNodesCommand (const NE::NodeCollection& nodes, NE::EvaluationEnv& evaluationEnv) :
	UndoableCommand (),
	nodes (nodes),
	evaluationEnv (evaluationEnv)
{
}

void DeleteNodesCommand::Do (NodeUIManager& uiManager)
{
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		uiManager.DeleteNode (nodeId, evaluationEnv);
		return true;
	});
}

MoveNodesCommand::MoveNodesCommand (const NE::NodeCollection& nodes, const std::vector<Point>& offsets) :
	UndoableCommand (),
	nodes (nodes),
	offsets (offsets)
{
}

void MoveNodesCommand::Do (NodeUIManager& uiManager)
{
	if (DBGERROR (nodes.Count () != offsets.size ())) {
		return;
	}
	for (size_t i = 0; i < nodes.Count (); i++) {
		const NE::NodeId& nodeId = nodes.Get (i);
		const Point& offset = offsets[i];
		UINodePtr uiNode = uiManager.GetUINode (nodeId);
		uiNode->SetNodePosition (uiNode->GetNodePosition () + offset);
		uiManager.InvalidateNodeGroupDrawing (uiNode);
	}
}

ConnectSlotsCommand::ConnectSlotsCommand (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) :
	UndoableCommand (),
	outputSlot (outputSlot),
	inputSlot (inputSlot)
{
}

void ConnectSlotsCommand::Do (NodeUIManager& uiManager)
{
	uiManager.ConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}

ReconnectSlotsCommand::ReconnectSlotsCommand (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& oldInputSlot, const UIInputSlotConstPtr& newInputSlot) :
	UndoableCommand (),
	outputSlot (outputSlot),
	oldInputSlot (oldInputSlot),
	newInputSlot (newInputSlot)
{
}

void ReconnectSlotsCommand::Do (NodeUIManager& uiManager)
{
	uiManager.DisconnectOutputSlotFromInputSlot (outputSlot, oldInputSlot);
	uiManager.ConnectOutputSlotToInputSlot (outputSlot, newInputSlot);
}

DisconnectSlotsCommand::DisconnectSlotsCommand (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) :
	UndoableCommand (),
	outputSlot (outputSlot),
	inputSlot (inputSlot)
{
}

void DisconnectSlotsCommand::Do (NodeUIManager& uiManager)
{
	uiManager.DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot);
}

DisconnectAllInputSlotsCommand::DisconnectAllInputSlotsCommand (const UIOutputSlotConstPtr& outputSlot) :
	UndoableCommand (),
	outputSlot (outputSlot)
{
}

void DisconnectAllInputSlotsCommand::Do (NodeUIManager& uiManager)
{
	uiManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
}

DisconnectAllOutputSlotsCommand::DisconnectAllOutputSlotsCommand (const UIInputSlotConstPtr& inputSlot) :
	UndoableCommand (),
	inputSlot (inputSlot)
{
}

void DisconnectAllOutputSlotsCommand::Do (NodeUIManager& uiManager)
{
	uiManager.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
}

CopyNodesCommand::CopyNodesCommand (const NE::NodeCollection& nodes) :
	NotUndoableCommand (),
	nodes (nodes)
{
}

void CopyNodesCommand::Do (NodeUIManager& uiManager)
{
	uiManager.Copy (nodes);
}

PasteNodesCommand::PasteNodesCommand (const Point& position) :
	UndoableCommand (),
	position (position)
{
}

void PasteNodesCommand::Do (NodeUIManager& uiManager)
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

AddGroupCommand::AddGroupCommand (const UINodeGroupPtr& uiGroup, const NE::NodeCollection& nodes) :
	UndoableCommand (),
	uiGroup (uiGroup),
	nodes (nodes)
{

}

void AddGroupCommand::Do (NodeUIManager& uiManager)
{
	uiManager.AddUINodeGroup (uiGroup);
	uiManager.AddNodesToUIGroup (uiGroup, nodes);
}

DeleteGroupCommand::DeleteGroupCommand (const UINodeGroupPtr& uiGroup) :
	UndoableCommand (),
	uiGroup (uiGroup)
{

}

void DeleteGroupCommand::Do (NodeUIManager& uiManager)
{
	uiManager.DeleteUINodeGroup (uiGroup);
}

AddNodesToGroupCommand::AddNodesToGroupCommand (const UINodeGroupPtr& uiGroup, const NE::NodeCollection& nodes) :
	UndoableCommand (),
	uiGroup (uiGroup),
	nodes (nodes)
{

}

void AddNodesToGroupCommand::Do (NodeUIManager& uiManager)
{
	uiManager.AddNodesToUIGroup (uiGroup, nodes);
}

RemoveNodesFromGroupCommand::RemoveNodesFromGroupCommand (const NE::NodeCollection& nodes) :
	UndoableCommand (),
	nodes (nodes)
{

}

void RemoveNodesFromGroupCommand::Do (NodeUIManager& uiManager)
{
	uiManager.RemoveNodesFromUIGroup (nodes);
}

UndoCommand::UndoCommand (NE::EvaluationEnv& evaluationEnv) :
	NotUndoableCommand (),
	evaluationEnv (evaluationEnv)
{
}

void UndoCommand::Do (NodeUIManager& uiManager)
{
	uiManager.Undo (evaluationEnv);
}

RedoCommand::RedoCommand (NE::EvaluationEnv& evaluationEnv) :
	NotUndoableCommand (),
	evaluationEnv (evaluationEnv)
{
}

void RedoCommand::Do (NodeUIManager& uiManager)
{
	uiManager.Redo (evaluationEnv);
}


CustomUndoableCommand::CustomUndoableCommand (const std::function<void ()>& func) :
	UndoableCommand (),
	func (func)
{
}

void CustomUndoableCommand::Do (NodeUIManager&)
{
	func ();
}

}
