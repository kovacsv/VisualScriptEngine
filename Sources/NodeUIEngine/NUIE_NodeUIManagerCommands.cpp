#include "NUIE_NodeUIManagerCommands.hpp"
#include "NE_MemoryStream.hpp"

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
	if (DBGERROR (nodes.IsEmpty ())) {
		return;
	}
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		uiManager.DeleteNode (nodeId, evaluationEnv);
		return true;
	});
}

MoveNodesCommand::MoveNodesCommand (const NE::NodeCollection& nodes, const Point& offset) :
	UndoableCommand (),
	nodes (nodes),
	offset (offset)
{
}

void MoveNodesCommand::Do (NodeUIManager& uiManager)
{
	for (size_t i = 0; i < nodes.Count (); i++) {
		const NE::NodeId& nodeId = nodes.Get (i);
		UINodePtr uiNode = uiManager.GetUINode (nodeId);
		uiNode->SetNodePosition (uiNode->GetNodePosition () + offset);
		uiManager.InvalidateNodeGroupDrawing (uiNode);
	}
}

MoveNodesWithOffsetsCommand::MoveNodesWithOffsetsCommand (const NE::NodeCollection& nodes, const std::vector<Point>& offsets) :
	UndoableCommand (),
	nodes (nodes),
	offsets (offsets)
{
}

void MoveNodesWithOffsetsCommand::Do (NodeUIManager& uiManager)
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

CopyMoveNodesCommand::CopyMoveNodesCommand (const NE::NodeCollection& nodes, const Point& offset) :
	UndoableCommand (),
	nodes (nodes),
	offset (offset)
{
}

void CopyMoveNodesCommand::Do (NodeUIManager& uiManager)
{
	NE::NodeCollection duplicatedNodes = uiManager.Duplicate (nodes);
	duplicatedNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		UINodePtr uiNode = uiManager.GetUINode (nodeId);
		uiNode->SetNodePosition (uiNode->GetNodePosition () + offset);
		return true;
	});
	uiManager.SetSelectedNodes (duplicatedNodes);
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

ReconnectInputSlotCommand::ReconnectInputSlotCommand (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& oldInputSlot, const UIInputSlotConstPtr& newInputSlot) :
	UndoableCommand (),
	outputSlot (outputSlot),
	oldInputSlot (oldInputSlot),
	newInputSlot (newInputSlot)
{
}

void ReconnectInputSlotCommand::Do (NodeUIManager& uiManager)
{
	uiManager.DisconnectOutputSlotFromInputSlot (outputSlot, oldInputSlot);
	uiManager.ConnectOutputSlotToInputSlot (outputSlot, newInputSlot);
}

ReconnectOutputSlotCommand::ReconnectOutputSlotCommand (const UIOutputSlotConstPtr& oldOutputSlot, const UIOutputSlotConstPtr& newOutputSlot, const UIInputSlotConstPtr& inputSlot) :
	UndoableCommand (),
	oldOutputSlot (oldOutputSlot),
	newOutputSlot (newOutputSlot),
	inputSlot (inputSlot)
{
}

void ReconnectOutputSlotCommand::Do (NodeUIManager& uiManager)
{
	uiManager.DisconnectOutputSlotFromInputSlot (oldOutputSlot, inputSlot);
	uiManager.ConnectOutputSlotToInputSlot (newOutputSlot, inputSlot);
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

CopyNodesCommand::CopyNodesCommand (const NE::NodeCollection& nodes, ClipboardHandler& clipboard) :
	NotUndoableCommand (),
	nodes (nodes),
	clipboard (clipboard)
{
}

void CopyNodesCommand::Do (NodeUIManager& uiManager)
{
	if (DBGERROR (nodes.IsEmpty ())) {
		return;
	}

	NE::NodeManager clipboardNodeManager;
	if (DBGERROR (!uiManager.CopyToNodeManager (nodes, clipboardNodeManager))) {
		return;
	}

	NE::MemoryOutputStream outputStream;
	Version currentVersion = clipboard.GetCurrentVersion ();
	currentVersion.Write (outputStream);
	clipboardNodeManager.Write (outputStream);
	if (DBGERROR (outputStream.GetStatus () != NE::Stream::Status::NoError)) {
		return;
	}

	clipboard.SetClipboardContent (outputStream.GetBuffer ());
}

PasteNodesCommand::PasteNodesCommand (const Point& position, ClipboardHandler& clipboard) :
	UndoableCommand (),
	position (position),
	clipboard (clipboard)
{
}

void PasteNodesCommand::Do (NodeUIManager& uiManager)
{
	if (DBGERROR (!clipboard.HasClipboardContent ())) {
		return;
	}

	std::vector<char> clipboardBuffer;
	if (DBGERROR (!clipboard.GetClipboardContent (clipboardBuffer))) {
		return;
	}

	NE::MemoryInputStream inputStream (clipboardBuffer);
	Version readVersion;
	readVersion.Read (inputStream);
	if (!clipboard.IsCompatibleVersion (readVersion)) {
		return;
	}
	NE::NodeManager clipboardNodeManager;
	clipboardNodeManager.Read (inputStream);

	if (DBGERROR (inputStream.GetStatus () != NE::Stream::Status::NoError)) {
		return;
	}

	NE::NodeCollection newNodeIds = uiManager.PasteFromNodeManager (clipboardNodeManager);
	std::vector<UINodePtr> newNodes;
	newNodeIds.Enumerate ([&] (const NE::NodeId& nodeId) {
		newNodes.push_back (uiManager.GetUINode (nodeId));
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
	if (DBGERROR (nodes.IsEmpty ())) {
		return;
	}
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

ApplyParametersCommand::ApplyParametersCommand (const ParameterInterfacePtr& paramInterface, NE::EvaluationEnv& evaluationEnv) :
	paramInterface (paramInterface),
	evaluationEnv (evaluationEnv)
{

}

void ApplyParametersCommand::Do (NodeUIManager& uiManager)
{
	paramInterface->ApplyChanges (uiManager, evaluationEnv);
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
