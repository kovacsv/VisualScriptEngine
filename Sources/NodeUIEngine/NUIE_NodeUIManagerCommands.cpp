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

DeleteNodesCommand::DeleteNodesCommand (NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& nodes) :
	UndoableCommand (),
	uiEnvironment (uiEnvironment),
	nodes (nodes)
{
}

void DeleteNodesCommand::Do (NodeUIManager& uiManager)
{
	if (DBGERROR (nodes.IsEmpty ())) {
		return;
	}
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		uiManager.DeleteNode (nodeId, uiEnvironment.GetEvaluationEnv (), uiEnvironment);
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
		UINodePtr uiNode = uiManager.GetNode (nodeId);
		uiNode->SetPosition (uiNode->GetPosition () + offset);
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
		UINodePtr uiNode = uiManager.GetNode (nodeId);
		uiNode->SetPosition (uiNode->GetPosition () + offset);
		uiManager.InvalidateNodeGroupDrawing (uiNode);
	}
}

CopyMoveNodesCommand::CopyMoveNodesCommand (NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& nodes, const Point& offset) :
	UndoableCommand (),
	uiEnvironment (uiEnvironment),
	nodes (nodes),
	offset (offset)
{
}

void CopyMoveNodesCommand::Do (NodeUIManager& uiManager)
{
	NE::NodeCollection duplicatedNodes = uiManager.Duplicate (nodes);
	duplicatedNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		UINodePtr uiNode = uiManager.GetNode (nodeId);
		uiNode->SetPosition (uiNode->GetPosition () + offset);
		return true;
	});
	Selection newSelection;
	newSelection.SetNodes (duplicatedNodes);
	uiManager.SetSelection (newSelection, uiEnvironment);
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

CopyNodesCommand::CopyNodesCommand (NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& nodes) :
	NotUndoableCommand (),
	uiEnvironment (uiEnvironment),
	nodes (nodes)
{
}

void CopyNodesCommand::Do (NodeUIManager& uiManager)
{
	if (DBGERROR (nodes.IsEmpty ())) {
		return;
	}

	NE::NodeManager clipboardNodeManager;
	if (DBGERROR (!uiManager.Copy (nodes, clipboardNodeManager))) {
		return;
	}

	NE::MemoryOutputStream outputStream;
	ClipboardHandler& clipboard = uiEnvironment.GetClipboardHandler ();
	Version currentVersion = clipboard.GetCurrentVersion ();
	currentVersion.Write (outputStream);
	clipboardNodeManager.Write (outputStream);
	if (DBGERROR (outputStream.GetStatus () != NE::Stream::Status::NoError)) {
		return;
	}

	bool hasPrevContent = clipboard.HasClipboardContent ();
	clipboard.SetClipboardContent (outputStream.GetBuffer ());
	if (!hasPrevContent) {
		ClipboardState clipboardState (clipboard.HasClipboardContent ());
		uiEnvironment.OnClipboardStateChanged (clipboardState);
	}
}

PasteNodesCommand::PasteNodesCommand (NodeUIEnvironment& uiEnvironment, const Point& position) :
	UndoableCommand (),
	uiEnvironment (uiEnvironment),
	position (position)
{
}

void PasteNodesCommand::Do (NodeUIManager& uiManager)
{
	ClipboardHandler& clipboard = uiEnvironment.GetClipboardHandler ();
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

	NE::NodeCollection newNodeIds = uiManager.Paste (clipboardNodeManager);
	std::vector<UINodePtr> newNodes;
	newNodeIds.Enumerate ([&] (const NE::NodeId& nodeId) {
		newNodes.push_back (uiManager.GetNode (nodeId));
		return true;
	});

	Point centerPosition;
	for (UINodePtr& uiNode : newNodes) {
		Point nodePosition = uiNode->GetPosition ();
		centerPosition = centerPosition + nodePosition;
	}

	Selection newSelection;
	centerPosition = centerPosition / (double) newNodes.size ();
	Point nodeOffset = position - centerPosition;
	for (UINodePtr& uiNode : newNodes) {
		Point nodePosition = uiNode->GetPosition ();
		uiNode->SetPosition (nodePosition + nodeOffset);
		newSelection.AddNode (uiNode->GetId ());
	}

	uiManager.SetSelection (newSelection, uiEnvironment);
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
	uiManager.AddNodeGroup (uiGroup);
	uiManager.AddNodesToGroup (uiGroup, nodes);
}

DeleteGroupCommand::DeleteGroupCommand (const UINodeGroupPtr& uiGroup) :
	UndoableCommand (),
	uiGroup (uiGroup)
{

}

void DeleteGroupCommand::Do (NodeUIManager& uiManager)
{
	uiManager.DeleteNodeGroup (uiGroup);
}

AddNodesToGroupCommand::AddNodesToGroupCommand (const UINodeGroupPtr& uiGroup, const NE::NodeCollection& nodes) :
	UndoableCommand (),
	uiGroup (uiGroup),
	nodes (nodes)
{

}

void AddNodesToGroupCommand::Do (NodeUIManager& uiManager)
{
	uiManager.AddNodesToGroup (uiGroup, nodes);
}

RemoveNodesFromGroupCommand::RemoveNodesFromGroupCommand (const NE::NodeCollection& nodes) :
	UndoableCommand (),
	nodes (nodes)
{

}

void RemoveNodesFromGroupCommand::Do (NodeUIManager& uiManager)
{
	uiManager.RemoveNodesFromGroup (nodes);
}

UndoCommand::UndoCommand (NodeUIEnvironment& uiEnvironment) :
	NotUndoableCommand (),
	uiEnvironment (uiEnvironment)
{
}

void UndoCommand::Do (NodeUIManager& uiManager)
{
	uiManager.Undo (uiEnvironment.GetEvaluationEnv (), uiEnvironment);
}

RedoCommand::RedoCommand (NodeUIEnvironment& uiEnvironment) :
	NotUndoableCommand (),
	uiEnvironment (uiEnvironment)
{
}

void RedoCommand::Do (NodeUIManager& uiManager)
{
	uiManager.Redo (uiEnvironment.GetEvaluationEnv (), uiEnvironment);
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
