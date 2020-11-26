#include "NUIE_NodeUIManager.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_Debug.hpp"
#include "NUIE_NodeDrawingModifier.hpp"
#include "NUIE_NodeUIManagerDrawer.hpp"
#include "NUIE_SkinParams.hpp"

namespace NUIE
{

SERIALIZATION_INFO (NodeUIManager, 1);

class NodeUIManagerUpdateEventHandler : public NE::UpdateEventHandler
{
public:
	NodeUIManagerUpdateEventHandler (NodeUIManager& uiManager, NodeUIInteractionEnvironment& uiEnvironment, NE::EvaluationEnv& evalEnv) :
		uiManager (uiManager),
		uiEnvironment (uiEnvironment),
		evalEnv (evalEnv)
	{
	
	}

	virtual ~NodeUIManagerUpdateEventHandler ()
	{
	
	}

	virtual void BeforeNodeDelete (const NE::NodeId& nodeId) override
	{
		UINodePtr uiNode = uiManager.GetNode (nodeId);
		if (DBGERROR (uiNode == nullptr)) {
			return;
		}
		uiNode->OnDelete (evalEnv);

		Selection selection = uiManager.GetSelection ();
		selection.DeleteNode (nodeId);
		uiManager.SetSelection (selection, uiEnvironment);
	}

private:
	NodeUIManager&					uiManager;
	NodeUIInteractionEnvironment&	uiEnvironment;
	NE::EvaluationEnv&				evalEnv;
};

NodeUIManager::Status::Status () :
	needToRecalculate (false),
	needToRedraw (false),
	needToSave (false)
{
	Reset ();
}

void NodeUIManager::Status::Reset ()
{
	needToRecalculate = false;
	needToRedraw = false;
	needToSave = false;
}

void NodeUIManager::Status::RequestRecalculate ()
{
	needToRecalculate = true;
}

void NodeUIManager::Status::ResetRecalculate ()
{
	needToRecalculate = false;
}

bool NodeUIManager::Status::NeedToRecalculate () const
{
	return needToRecalculate;
}

void NodeUIManager::Status::RequestRedraw ()
{
	needToRedraw = true;
}

void NodeUIManager::Status::ResetRedraw ()
{
	needToRedraw = false;
}

bool NodeUIManager::Status::NeedToRedraw () const
{
	return needToRedraw;
}

void NodeUIManager::Status::RequestSave ()
{
	needToSave = true;
}

void NodeUIManager::Status::ResetSave ()
{
	needToSave = false;
}

bool NodeUIManager::Status::NeedToSave () const
{
	return needToSave;
}

NodeUIManagerCommand::NodeUIManagerCommand ()
{

}

NodeUIManagerCommand::~NodeUIManagerCommand ()
{

}

NodeUIManagerNodeInvalidator::NodeUIManagerNodeInvalidator (NodeUIManager& uiManager, UINodePtr& uiNode) :
	UINodeInvalidator (),
	uiManager (uiManager),
	uiNode (uiNode)
{
}

NodeUIManagerNodeInvalidator::~NodeUIManagerNodeInvalidator ()
{

}

void NodeUIManagerNodeInvalidator::InvalidateValue ()
{
	uiManager.InvalidateNodeValue (uiNode);
}

void NodeUIManagerNodeInvalidator::InvalidateDrawing ()
{
	uiManager.InvalidateNodeDrawing (uiNode);
}

void NodeUIManagerNodeInvalidator::RequestRecalculate ()
{
	uiManager.RequestRecalculate ();
}

void NodeUIManagerNodeInvalidator::RequestRedraw ()
{
	uiManager.RequestRedraw ();
}

NodeUIManagerNodeRectGetter::NodeUIManagerNodeRectGetter (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& drawingEnv) :
	uiManager (uiManager),
	drawingEnv (drawingEnv)
{
}

Rect NodeUIManagerNodeRectGetter::GetNodeRect (const NE::NodeId& nodeId) const
{
	UINodeConstPtr uiNode = uiManager.GetNode (nodeId);
	return uiNode->GetRect (drawingEnv);
}

UIOutputSlotList::UIOutputSlotList () :
	NE::OutputSlotList ()
{

}

UIOutputSlotList::~UIOutputSlotList ()
{

}

UIInputSlotList::UIInputSlotList () :
	NE::InputSlotList ()
{

}

UIInputSlotList::~UIInputSlotList ()
{

}

NodeUIManager::NodeUIManager (NodeUIEnvironment& uiEnvironment) :
	nodeManager (),
	undoHandler (),
	selection (),
	viewBox (),
	status ()
{
	New (uiEnvironment);
}

NodeUIManager::~NodeUIManager ()
{

}

UINodePtr NodeUIManager::AddNode (const UINodePtr& uiNode)
{
	if (DBGERROR (uiNode == nullptr)) {
		return nullptr;
	}

	NE::NodePtr resultNode = nodeManager.AddNode (uiNode);
	if (resultNode == nullptr) {
		return nullptr;
	}

	RequestRecalculateAndRedraw ();
	return uiNode;
}

bool NodeUIManager::DeleteNode (const UINodePtr& uiNode, NE::EvaluationEnv& evalEnv, NodeUIInteractionEnvironment& interactionEnv)
{
	if (DBGERROR (uiNode == nullptr)) {
		return false;
	}

	uiNode->OnDelete (evalEnv);
	
	Selection::ChangeResult selResult = selection.DeleteNode (uiNode->GetId ());
	HandleSelectionChanged (selResult, interactionEnv);
	
	InvalidateNodeDrawing (uiNode);
	if (!nodeManager.DeleteNode (uiNode)) {
		return false;
	}

	RequestRecalculateAndRedraw ();
	return true;
}

bool NodeUIManager::DeleteNode (const NE::NodeId& nodeId, NE::EvaluationEnv& evalEnv, NodeUIInteractionEnvironment& interactionEnv)
{
	if (DBGERROR (!nodeManager.ContainsNode (nodeId))) {
		return false;
	}
	UINodePtr node = GetNode (nodeId);
	return DeleteNode (node, evalEnv, interactionEnv);
}

const Selection& NodeUIManager::GetSelection () const
{
	return selection;
}

void NodeUIManager::SetSelection (const Selection& newSelection, NodeUIInteractionEnvironment& interactionEnv)
{
	Selection::ChangeResult selResult = selection.Update (newSelection);
	HandleSelectionChanged (selResult, interactionEnv);
	status.RequestRedraw ();
}

bool NodeUIManager::IsOutputSlotConnectedToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot);
}

bool NodeUIManager::CanConnectOutputSlotToInputSlot (const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.CanConnectOutputSlotToInputSlot (inputSlot);
}

bool NodeUIManager::CanConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}

bool NodeUIManager::CanConnectOutputSlotsToInputSlot (const UIOutputSlotList& outputSlots, const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.CanConnectOutputSlotsToInputSlot (outputSlots, inputSlot);
}

bool NodeUIManager::CanConnectOutputSlotToInputSlots (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotList& inputSlots) const
{
	return nodeManager.CanConnectOutputSlotToInputSlots (outputSlot, inputSlots);
}

bool NodeUIManager::ConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot)
{
	DBGASSERT (CanConnectOutputSlotToInputSlot (outputSlot, inputSlot));
	bool success = nodeManager.ConnectOutputSlotToInputSlot (outputSlot, inputSlot);
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::ConnectOutputSlotsToInputSlot (const UIOutputSlotList& outputSlots, const UIInputSlotConstPtr& inputSlot)
{
	DBGASSERT (CanConnectOutputSlotsToInputSlot (outputSlots, inputSlot));
	bool success = nodeManager.ConnectOutputSlotsToInputSlot (outputSlots, inputSlot);
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::ConnectOutputSlotToInputSlots (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotList& inputSlots)
{
	DBGASSERT (CanConnectOutputSlotToInputSlots (outputSlot, inputSlots));
	bool success = nodeManager.ConnectOutputSlotToInputSlots (outputSlot, inputSlots);
	inputSlots.Enumerate ([&] (const NE::InputSlotConstPtr& inputSlot) {
		InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
		return true;
	});
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::DisconnectOutputSlotFromInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot)
{
	bool success = nodeManager.DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot);
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::DisconnectOutputSlotsFromInputSlot (const UIOutputSlotList& outputSlots, const UIInputSlotConstPtr& inputSlot)
{
	bool success = nodeManager.DisconnectOutputSlotsFromInputSlot (outputSlots, inputSlot);
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::DisconnectOutputSlotFromInputSlots (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotList& inputSlots)
{
	bool success = nodeManager.DisconnectOutputSlotFromInputSlots (outputSlot, inputSlots);
	inputSlots.Enumerate ([&] (const NE::InputSlotConstPtr& inputSlot) {
		InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
		return true;
	});
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::DisconnectAllInputSlotsFromOutputSlot (const UIOutputSlotConstPtr& outputSlot)
{
	bool success = nodeManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
	InvalidateNodeDrawing (outputSlot->GetOwnerNodeId ());
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::DisconnectAllOutputSlotsFromInputSlot (const UIInputSlotConstPtr& inputSlot)
{
	bool success = nodeManager.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::HasConnectedInputSlots (const UIOutputSlotConstPtr& outputSlot) const
{
	return nodeManager.HasConnectedInputSlots (outputSlot);
}

bool NodeUIManager::HasConnectedOutputSlots (const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.HasConnectedOutputSlots (inputSlot);
}

size_t NodeUIManager::GetConnectedInputSlotCount (const UIOutputSlotConstPtr& outputSlot) const
{
	return nodeManager.GetConnectedInputSlotCount (outputSlot);
}

size_t NodeUIManager::GetConnectedOutputSlotCount (const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.GetConnectedOutputSlotCount (inputSlot);
}

void NodeUIManager::EnumerateConnectedInputSlots (const NE::OutputSlotConstPtr& outputSlot, const std::function<void (const NE::InputSlotConstPtr&)>& processor) const
{
	nodeManager.EnumerateConnectedInputSlots (outputSlot, processor);
}

void NodeUIManager::EnumerateConnectedOutputSlots (const NE::InputSlotConstPtr& inputSlot, const std::function<void (const NE::OutputSlotConstPtr&)>& processor) const
{
	nodeManager.EnumerateConnectedOutputSlots (inputSlot, processor);
}

void NodeUIManager::EnumerateConnectedUIInputSlots (const UIOutputSlotConstPtr& outputSlot, const std::function<void (UIInputSlotConstPtr)>& processor) const
{
	nodeManager.EnumerateConnectedInputSlots (outputSlot, [&] (const NE::InputSlotConstPtr& inputSlot) {
		processor (std::dynamic_pointer_cast<const UIInputSlot> (inputSlot));
	});
}

void NodeUIManager::EnumerateConnectedUIOutputSlots (const UIInputSlotConstPtr& inputSlot, const std::function<void (UIOutputSlotConstPtr)>& processor) const
{
	nodeManager.EnumerateConnectedOutputSlots (inputSlot, [&] (const NE::OutputSlotConstPtr& outputSlot) {
		processor (std::dynamic_pointer_cast<const UIOutputSlot> (outputSlot));
	});
}

bool NodeUIManager::ContainsNode (const NE::NodeId& nodeId) const
{
	return nodeManager.ContainsNode (nodeId);
}

UINodePtr NodeUIManager::GetNode (const NE::NodeId& nodeId)
{
	NE::NodePtr node = nodeManager.GetNode (nodeId);
	return NE::Node::Cast<UINode> (node);
}

UINodeConstPtr NodeUIManager::GetNode (const NE::NodeId& nodeId) const
{
	NE::NodeConstPtr node = nodeManager.GetNode (nodeId);
	return std::dynamic_pointer_cast<const UINode> (node);
}

void NodeUIManager::EnumerateNodes (const std::function<bool (const UINodePtr&)>& processor)
{
	nodeManager.EnumerateNodes ([&] (const NE::NodePtr& node) {
		return processor (std::static_pointer_cast<UINode> (node));
	});
}

void NodeUIManager::EnumerateNodes (const std::function<bool (const UINodeConstPtr&)>& processor) const
{
	nodeManager.EnumerateNodes ([&] (const NE::NodeConstPtr& node) {
		return processor (std::static_pointer_cast<const UINode> (node));
	});
}

void NodeUIManager::RequestRecalculateAndRedraw ()
{
	status.RequestRecalculate ();
	status.RequestRedraw ();
}

void NodeUIManager::RequestRecalculate ()
{
	status.RequestRecalculate ();
}

void NodeUIManager::RequestRedraw ()
{
	status.RequestRedraw ();
}

void NodeUIManager::InvalidateAllDrawings ()
{
	InvalidateAllNodesDrawing ();
	InvalidateAllNodeGroupsDrawing ();
}

void NodeUIManager::InvalidateAllNodesDrawing ()
{
	EnumerateNodes ([&] (const UINodePtr& uiNode) {
		uiNode->InvalidateDrawing ();
		return true;
	});
	RequestRedraw ();
}

void NodeUIManager::InvalidateAllNodeGroupsDrawing ()
{
	EnumerateNodeGroups ([&] (const UINodeGroupConstPtr& group) {
		group->InvalidateGroupDrawing ();
		return true;
	});
	RequestRedraw ();
}

void NodeUIManager::InvalidateNodeValue (const NE::NodeId& nodeId)
{
	UINodePtr uiNode = GetNode (nodeId);
	InvalidateNodeValue (uiNode);
}

void NodeUIManager::InvalidateNodeValue (const UINodePtr& uiNode)
{
	uiNode->InvalidateValue ();
	RequestRecalculateAndRedraw ();
}

void NodeUIManager::InvalidateNodeDrawing (const NE::NodeId& nodeId)
{
	UINodePtr uiNode = GetNode (nodeId);
	InvalidateNodeDrawing (uiNode);
}

void NodeUIManager::InvalidateNodeDrawing (const UINodePtr& uiNode)
{
	uiNode->InvalidateDrawing ();
	InvalidateNodeGroupDrawing (uiNode);
	nodeManager.EnumerateDependentNodes (uiNode, [&] (const NE::NodeId& dependentNodeId) {
		UINodePtr dependentNode = GetNode (dependentNodeId);
		InvalidateNodeDrawing (dependentNode);
	});
	status.RequestRedraw ();
}

void NodeUIManager::InvalidateNodeGroupDrawing (const NE::NodeId& nodeid)
{
	NE::NodeGroupConstPtr group = nodeManager.GetNodeGroup (nodeid);
	if (group == nullptr) {
		return;
	}

	UINodeGroupConstPtr uiGroup = std::static_pointer_cast<const UINodeGroup> (group);
	uiGroup->InvalidateGroupDrawing ();
	RequestRedraw ();
}

void NodeUIManager::InvalidateNodeGroupDrawing (const UINodePtr& uiNode)
{
	InvalidateNodeGroupDrawing (uiNode->GetId ());
}

void NodeUIManager::Update (NodeUICalculationEnvironment& calcEnv)
{
	UpdateInternal (calcEnv, InternalUpdateMode::Normal);
}

void NodeUIManager::ManualUpdate (NodeUICalculationEnvironment& calcEnv)
{
	InvalidateDrawingsForInvalidatedNodes ();
	UpdateInternal (calcEnv, InternalUpdateMode::Manual);
}

void NodeUIManager::Draw (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawingModifier)
{
	NodeUIManagerDrawer drawer (*this);
	drawer.Draw (drawingEnv, drawingModifier);
}

void NodeUIManager::ResizeContext (NodeUIDrawingEnvironment& drawingEnv, int newWidth, int newHeight)
{
	drawingEnv.GetDrawingContext ().Resize (newWidth, newHeight);
	status.RequestRedraw ();
}

bool NodeUIManager::GetBoundingRect (NodeUIDrawingEnvironment& drawingEnv, Rect& rect) const
{
	BoundingRect boundingRect;
	EnumerateNodes ([&] (const UINodeConstPtr& uiNode) {
		Rect nodeRect = GetNodeExtendedRect (drawingEnv, uiNode.get ());
		boundingRect.AddRect (nodeRect);
		return true;
	});

	NodeUIManagerNodeRectGetter nodeRectGetter (*this, drawingEnv);
	EnumerateNodeGroups ([&] (const UINodeGroupConstPtr& uiGroup) {
		Rect groupRect = uiGroup->GetRect (drawingEnv, nodeRectGetter, GetGroupNodes (uiGroup));
		boundingRect.AddRect (groupRect);
		return true;
	});

	if (!boundingRect.IsValid ()) {
		return false;
	}

	rect = boundingRect.GetRect ();
	return true;
}

void NodeUIManager::AlignToWindow (NodeUIDrawingEnvironment& drawingEnv)
{
	Rect boundingRect;
	if (!GetBoundingRect (drawingEnv, boundingRect)) {
		return;
	}

	double scale = drawingEnv.GetWindowScale ();
	double viewPadding = drawingEnv.GetSkinParams ().GetNodePadding ();
	ViewBox newViewBox (-boundingRect.GetTopLeft () * scale + Point (viewPadding, viewPadding), scale);
	SetViewBox (newViewBox);
	status.RequestRedraw ();
}

void NodeUIManager::CenterToWindow (NodeUIDrawingEnvironment& drawingEnv)
{
	Rect boundingRect;
	if (!GetBoundingRect (drawingEnv, boundingRect)) {
		return;
	}

	double scale = drawingEnv.GetWindowScale ();
	const DrawingContext& drawingContext = drawingEnv.GetDrawingContext ();
	
	Size contextSize (drawingContext.GetWidth (), drawingContext.GetHeight ());
	ViewBox centeredViewBox = CenterRectToSize (contextSize, scale, boundingRect);
	SetViewBox (centeredViewBox);

	status.RequestRedraw ();
}

void NodeUIManager::FitToWindow (NodeUIDrawingEnvironment& drawingEnv)
{
	Rect boundingRect;
	if (!GetBoundingRect (drawingEnv, boundingRect)) {
		return;
	}

	double viewPadding = drawingEnv.GetSkinParams ().GetNodePadding ();
	const DrawingContext& drawingContext = drawingEnv.GetDrawingContext ();
	Size contextSize (drawingContext.GetWidth (), drawingContext.GetHeight ());
	ViewBox newViewBox = FitRectToSize (contextSize, viewPadding, boundingRect);
	SetViewBox (newViewBox);
	status.RequestRedraw ();
}

const ViewBox& NodeUIManager::GetViewBox () const
{
	return viewBox;
}

void NodeUIManager::SetViewBox (const ViewBox& newViewBox)
{
	viewBox = newViewBox;
	status.RequestRedraw ();
}

bool NodeUIManager::IsPreviewMode () const
{
	return viewBox.GetScale () < 0.3;
}

NodeUIManager::UpdateMode NodeUIManager::GetUpdateMode () const
{
	switch (nodeManager.GetUpdateMode ()) {
		case NE::NodeManager::UpdateMode::Automatic:
			return UpdateMode::Automatic;
		case NE::NodeManager::UpdateMode::Manual:
			return UpdateMode::Manual;
	}
	DBGBREAK ();
	return UpdateMode::Automatic;
}

void NodeUIManager::SetUpdateMode (UpdateMode newUpdateMode)
{
	switch (newUpdateMode) {
		case UpdateMode::Automatic:
			nodeManager.SetUpdateMode (NE::NodeManager::UpdateMode::Automatic);
			break;
		case UpdateMode::Manual:
			nodeManager.SetUpdateMode (NE::NodeManager::UpdateMode::Manual);
			break;
		default:
			DBGBREAK ();
			break;
	}
}

void NodeUIManager::New (NodeUIEnvironment& uiEnvironment)
{
	Clear (uiEnvironment);
	RequestRecalculateAndRedraw ();
}

bool NodeUIManager::Open (NodeUIEnvironment& uiEnvironment, NE::InputStream& inputStream)
{
	Clear (uiEnvironment);
	Read (inputStream);
	RequestRecalculateAndRedraw ();
	return inputStream.GetStatus() == NE::Stream::Status::NoError;
}

bool NodeUIManager::Save (NE::OutputStream& outputStream)
{
	Write (outputStream);
	status.ResetSave ();
	return outputStream.GetStatus () == NE::Stream::Status::NoError;
}

bool NodeUIManager::NeedToSave () const
{
	return status.NeedToSave ();
}

bool NodeUIManager::Copy (const NE::NodeCollection& nodeCollection, NE::NodeManager& result) const
{
	NE::NodeCollectionFilter nodeFilter (nodeCollection);
	NE::EmptyAppendEventHandler eventHandler;
	return NE::NodeManagerMerge::AppendNodeManager (nodeManager, result, nodeFilter, eventHandler);
}

NE::NodeCollection NodeUIManager::Paste (const NE::NodeManager& source)
{
	NE::AllNodesFilter allNodesFilter;
	NE::NodeCollectorAppendEventHandler eventHandler;
	if (DBGERROR (!NE::NodeManagerMerge::AppendNodeManager (source, nodeManager, allNodesFilter, eventHandler))) {
		return NE::EmptyNodeCollection;
	}
	RequestRecalculateAndRedraw ();
	return eventHandler.GetAddedTargetNodes ();
}

NE::NodeCollection NodeUIManager::Duplicate (const NE::NodeCollection& nodeCollection)
{
	NE::NodeManager tempNodeManager;
	Copy (nodeCollection, tempNodeManager);
	return Paste (tempNodeManager);
}

bool NodeUIManager::CanUndo () const
{
	return undoHandler.CanUndo ();
}

bool NodeUIManager::CanRedo () const
{
	return undoHandler.CanRedo ();
}

void NodeUIManager::Undo (NE::EvaluationEnv& evalEnv, NodeUIInteractionEnvironment& interactionEnv)
{
	NodeUIManagerUpdateEventHandler eventHandler (*this, interactionEnv, evalEnv);
	UndoHandler::ChangeResult undoResult = undoHandler.Undo (nodeManager, eventHandler);
	HandleUndoStateChanged (undoResult, interactionEnv);
	InvalidateDrawingsForInvalidatedNodes ();
	RequestRecalculateAndRedraw ();
}

void NodeUIManager::Redo (NE::EvaluationEnv& evalEnv, NodeUIInteractionEnvironment& interactionEnv)
{
	NodeUIManagerUpdateEventHandler eventHandler (*this, interactionEnv, evalEnv);
	UndoHandler::ChangeResult undoResult = undoHandler.Redo (nodeManager, eventHandler);
	HandleUndoStateChanged (undoResult, interactionEnv);
	InvalidateDrawingsForInvalidatedNodes ();
	RequestRecalculateAndRedraw ();
}

UINodeGroupPtr NodeUIManager::AddNodeGroup (const UINodeGroupPtr& group)
{
	NE::NodeGroupPtr resultGroup = nodeManager.AddNodeGroup (group);
	if (resultGroup == nullptr) {
		return nullptr;
	}
	InvalidateAllNodeGroupsDrawing ();
	return group;
}

void NodeUIManager::DeleteNodeGroup (const UINodeGroupPtr& group)
{
	nodeManager.DeleteNodeGroup (group->GetId ());
	RequestRedraw ();
}

void NodeUIManager::AddNodesToGroup (const UINodeGroupPtr& group, const NE::NodeCollection& nodeCollection)
{
	nodeCollection.Enumerate ([&] (const NE::NodeId& nodeId) {
		nodeManager.AddNodeToGroup (group->GetId (), nodeId);
		return true;
	});
	InvalidateAllNodeGroupsDrawing ();
}

bool NodeUIManager::RemoveNodesFromGroup (const NE::NodeCollection& nodeCollection)
{
	nodeCollection.Enumerate ([&] (const NE::NodeId& nodeId) {
		nodeManager.RemoveNodeFromGroup (nodeId);
		return true;	
	});
	InvalidateAllNodeGroupsDrawing ();
	return true;
}

const NE::NodeCollection& NodeUIManager::GetGroupNodes (const UINodeGroupConstPtr& group) const
{
	return nodeManager.GetGroupNodes (group->GetId ());
}

UINodeGroupConstPtr NodeUIManager::GetNodeGroup (const NE::NodeId& nodeId) const
{
	return std::static_pointer_cast<const UINodeGroup> (nodeManager.GetNodeGroup (nodeId));
}

void NodeUIManager::EnumerateNodeGroups (const std::function<bool (const UINodeGroupConstPtr&)>& processor) const
{
	nodeManager.EnumerateNodeGroups ([&] (const NE::NodeGroupConstPtr& nodeGroup) {
		return processor (std::static_pointer_cast<const UINodeGroup> (nodeGroup));
	});
}

void NodeUIManager::EnumerateNodeGroups (const std::function<bool (const UINodeGroupPtr&)>& processor)
{
	nodeManager.EnumerateNodeGroups ([&] (const NE::NodeGroupPtr& nodeGroup) {
		return processor (std::static_pointer_cast<UINodeGroup> (nodeGroup));
	});
}

void NodeUIManager::ExecuteCommand (NodeUIManagerCommand& command, NodeUIInteractionEnvironment& interactionEnv)
{
	if (command.IsUndoable ()) {
		UndoHandler::ChangeResult result = undoHandler.AddUndoStep (nodeManager);
		HandleUndoStateChanged (result, interactionEnv);
	}
	command.Do (*this);
	status.RequestSave ();
}

void NodeUIManager::ExecuteCommand (NodeUIManagerCommandPtr& command, NodeUIInteractionEnvironment& interactionEnv)
{
	if (DBGERROR (command == nullptr)) {
		return;
	}
	ExecuteCommand (*command, interactionEnv);
}

void NodeUIManager::Clear (NodeUIEnvironment& uiEnvironment)
{
	double windowScale = uiEnvironment.GetWindowScale ();

	Selection::ChangeResult selResult = selection.Clear ();
	HandleSelectionChanged (selResult, uiEnvironment);

	UndoHandler::ChangeResult undoResult = undoHandler.Clear ();
	HandleUndoStateChanged (undoResult, uiEnvironment);

	nodeManager.Clear ();

	viewBox.Set (Point (0.0, 0.0), windowScale);
	status.Reset ();
}

void NodeUIManager::InvalidateDrawingsForInvalidatedNodes ()
{
	std::vector<UINodePtr> nodesToInvalidate;
	EnumerateNodes ([&] (const UINodePtr& uiNode) {
		NE::Node::CalculationStatus calcStatus = uiNode->GetCalculationStatus ();
		if (calcStatus == NE::Node::CalculationStatus::NeedToCalculate || calcStatus == NE::Node::CalculationStatus::NeedToCalculateButDisabled) {
			nodesToInvalidate.push_back (uiNode);
		}
		return true;
	});
	for (const UINodePtr& uiNode : nodesToInvalidate) {
		InvalidateNodeDrawing (uiNode);
	}
	status.RequestRedraw ();
}

void NodeUIManager::UpdateInternal (NodeUICalculationEnvironment& calcEnv, InternalUpdateMode mode)
{
	if (status.NeedToRecalculate ()) {
		calcEnv.OnEvaluationBegin ();
		if (mode == InternalUpdateMode::Normal) {
			nodeManager.EvaluateAllNodes (calcEnv.GetEvaluationEnv ());
		} else if (mode == InternalUpdateMode::Manual) {
			nodeManager.ForceEvaluateAllNodes (calcEnv.GetEvaluationEnv ());
		}
		calcEnv.OnEvaluationEnd ();

		calcEnv.OnValuesRecalculated ();
		status.ResetRecalculate ();
	}
	if (status.NeedToRedraw ()) {
		calcEnv.OnRedrawRequested ();
		status.ResetRedraw ();
	}
}

void NodeUIManager::HandleSelectionChanged (Selection::ChangeResult changeResult, NodeUIInteractionEnvironment& interactionEnv)
{
	if (changeResult == Selection::ChangeResult::Changed) {
		interactionEnv.OnSelectionChanged (selection);
	}
}

void NodeUIManager::HandleUndoStateChanged (UndoHandler::ChangeResult changeResult, NodeUIInteractionEnvironment&interactionEnv)
{
	if (changeResult == UndoHandler::ChangeResult::Changed) {
		UndoState undoState (undoHandler.CanUndo (), undoHandler.CanRedo ());
		interactionEnv.OnUndoStateChanged (undoState);
	}
}

NE::Stream::Status NodeUIManager::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	nodeManager.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NodeUIManager::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	nodeManager.Write (outputStream);
	return outputStream.GetStatus ();
}

}
