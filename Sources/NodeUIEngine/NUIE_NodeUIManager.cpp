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
	NodeUIManagerUpdateEventHandler (NodeUIManager& uiManager, NE::EvaluationEnv& env) :
		uiManager (uiManager),
		env (env)
	{
	
	}

	virtual ~NodeUIManagerUpdateEventHandler ()
	{
	
	}

	virtual void BeforeNodeDelete (const NE::NodeId& nodeId) override
	{
		UINodePtr uiNode = uiManager.GetUINode (nodeId);
		if (DBGERROR (uiNode == nullptr)) {
			return;
		}
		uiNode->OnDelete (env);

		NE::NodeCollection selectedNodes = uiManager.GetSelectedNodes ();
		selectedNodes.Erase (nodeId);
		uiManager.SetSelectedNodes (selectedNodes);
	}

private:
	NodeUIManager&		uiManager;
	NE::EvaluationEnv&	env;
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

NodeUIManagerNodeRectGetter::NodeUIManagerNodeRectGetter (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) :
	uiManager (uiManager),
	env (env)
{
}

Rect NodeUIManagerNodeRectGetter::GetNodeRect (const NE::NodeId& nodeId) const
{
	UINodeConstPtr uiNode = uiManager.GetUINode (nodeId);
	return uiNode->GetRect (env);
}

NodeUIManager::NodeUIManager (NodeUIEnvironment& env) :
	nodeManager (),
	selectedNodes (),
	undoHandler (),
	viewBox (),
	status ()
{
	New (env);
}

NodeUIManager::~NodeUIManager ()
{

}

UINodePtr NodeUIManager::AddNode (const UINodePtr& uiNode, NE::EvaluationEnv& env)
{
	if (DBGERROR (uiNode == nullptr)) {
		return nullptr;
	}
	NE::NodePtr resultNode = nodeManager.AddNode (uiNode);
	if (resultNode == nullptr) {
		return nullptr;
	}
	uiNode->OnAdd (env);
	RequestRecalculateAndRedraw ();
	return uiNode;
}

bool NodeUIManager::DeleteNode (const UINodePtr& uiNode, NE::EvaluationEnv& env)
{
	if (DBGERROR (uiNode == nullptr)) {
		return false;
	}
	uiNode->OnDelete (env);
	selectedNodes.Erase (uiNode->GetId ());
	InvalidateNodeDrawing (uiNode);
	if (!nodeManager.DeleteNode (uiNode)) {
		return false;
	}
	RequestRecalculateAndRedraw ();
	return true;
}

bool NodeUIManager::DeleteNode (const NE::NodeId& nodeId, NE::EvaluationEnv& env)
{
	if (DBGERROR (!nodeManager.ContainsNode (nodeId))) {
		return false;
	}
	UINodePtr node = GetUINode (nodeId);
	return DeleteNode (node, env);
}

const NE::NodeCollection& NodeUIManager::GetSelectedNodes () const
{
	return selectedNodes;
}

void NodeUIManager::SetSelectedNodes (const NE::NodeCollection& newSelectedNodes)
{
	selectedNodes = newSelectedNodes;
	selectedNodes.MakeSorted ();
	status.RequestRedraw ();
}

bool NodeUIManager::IsOutputSlotConnectedToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot);
}

bool NodeUIManager::CanConnectMoreOutputSlotToInputSlot (const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.CanConnectMoreOutputSlotToInputSlot (inputSlot);
}

bool NodeUIManager::CanConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}

bool NodeUIManager::ConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot)
{
	DBGASSERT (CanConnectOutputSlotToInputSlot (outputSlot, inputSlot));
	RequestRecalculateAndRedraw ();
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	return nodeManager.ConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}

bool NodeUIManager::DisconnectOutputSlotFromInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot)
{
	RequestRecalculateAndRedraw ();
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	return nodeManager.DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot);
}

bool NodeUIManager::DisconnectAllInputSlotsFromOutputSlot (const UIOutputSlotConstPtr& outputSlot)
{
	RequestRecalculateAndRedraw ();
	InvalidateNodeDrawing (outputSlot->GetOwnerNodeId ());
	return nodeManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
}

bool NodeUIManager::DisconnectAllOutputSlotsFromInputSlot (const UIInputSlotConstPtr& inputSlot)
{
	RequestRecalculateAndRedraw ();
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	return nodeManager.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
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

bool NodeUIManager::ContainsUINode (const NE::NodeId& nodeId) const
{
	return nodeManager.ContainsNode (nodeId);
}

UINodePtr NodeUIManager::GetUINode (const NE::NodeId& nodeId)
{
	NE::NodePtr node = nodeManager.GetNode (nodeId);
	return NE::Node::Cast<UINode> (node);
}

UINodeConstPtr NodeUIManager::GetUINode (const NE::NodeId& nodeId) const
{
	NE::NodeConstPtr node = nodeManager.GetNode (nodeId);
	return std::dynamic_pointer_cast<const UINode> (node);
}

void NodeUIManager::EnumerateUINodes (const std::function<bool (const UINodePtr&)>& processor)
{
	nodeManager.EnumerateNodes ([&] (const NE::NodePtr& node) {
		return processor (std::static_pointer_cast<UINode> (node));
	});
}

void NodeUIManager::EnumerateUINodes (const std::function<bool (const UINodeConstPtr&)>& processor) const
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
	EnumerateUINodes ([&] (const UINodePtr& uiNode) {
		uiNode->InvalidateDrawing ();
		return true;
	});
	RequestRedraw ();
}

void NodeUIManager::InvalidateAllNodeGroupsDrawing ()
{
	EnumerateUINodeGroups ([&] (const UINodeGroupConstPtr& group) {
		group->InvalidateGroupDrawing ();
		return true;
	});
	RequestRedraw ();
}

void NodeUIManager::InvalidateNodeValue (const NE::NodeId& nodeId)
{
	UINodePtr uiNode = GetUINode (nodeId);
	InvalidateNodeValue (uiNode);
}

void NodeUIManager::InvalidateNodeValue (const UINodePtr& uiNode)
{
	uiNode->InvalidateValue ();
	RequestRecalculateAndRedraw ();
}

void NodeUIManager::InvalidateNodeDrawing (const NE::NodeId& nodeId)
{
	UINodePtr uiNode = GetUINode (nodeId);
	InvalidateNodeDrawing (uiNode);
}

void NodeUIManager::InvalidateNodeDrawing (const UINodePtr& uiNode)
{
	uiNode->InvalidateDrawing ();
	InvalidateNodeGroupDrawing (uiNode);
	nodeManager.EnumerateDependentNodes (uiNode, [&] (const NE::NodeId& dependentNodeId) {
		UINodePtr dependentNode = GetUINode (dependentNodeId);
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

void NodeUIManager::Update (NodeUICalculationEnvironment& env)
{
	UpdateInternal (env, InternalUpdateMode::Normal);
}

void NodeUIManager::ManualUpdate (NodeUICalculationEnvironment& env)
{
	InvalidateDrawingsForInvalidatedNodes ();
	UpdateInternal (env, InternalUpdateMode::Manual);
}

void NodeUIManager::Draw (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawingModifier)
{
	NodeUIManagerDrawer drawer (*this);
	drawer.Draw (env, drawingModifier);
}

void NodeUIManager::ResizeContext (NodeUIDrawingEnvironment& env, int newWidth, int newHeight)
{
	env.GetDrawingContext ().Resize (newWidth, newHeight);
	status.RequestRedraw ();
}

bool NodeUIManager::GetBoundingRect (NodeUIDrawingEnvironment& env, Rect& boundingRect) const
{
	BoundingRectCalculator boundingRectCalculator;
	EnumerateUINodes ([&] (const UINodeConstPtr& uiNode) {
		Rect nodeRect = GetNodeExtendedRect (env, uiNode.get ());
		boundingRectCalculator.AddRect (nodeRect);
		return true;
	});

	NodeUIManagerNodeRectGetter nodeRectGetter (*this, env);
	EnumerateUINodeGroups ([&] (const UINodeGroupConstPtr& uiGroup) {
		Rect groupRect = uiGroup->GetRect (env, nodeRectGetter, GetUIGroupNodes (uiGroup));
		boundingRectCalculator.AddRect (groupRect);
		return true;
	});

	if (!boundingRectCalculator.IsValid ()) {
		return false;
	}

	boundingRect = boundingRectCalculator.GetRect ();
	return true;
}

void NodeUIManager::AlignToWindow (NodeUIDrawingEnvironment& env)
{
	Rect boundingRect;
	if (!GetBoundingRect (env, boundingRect)) {
		return;
	}

	double scale = env.GetWindowScale ();
	double viewPadding = env.GetSkinParams ().GetNodePadding ();
	ViewBox newViewBox (-boundingRect.GetTopLeft () * scale + Point (viewPadding, viewPadding), scale);
	SetViewBox (newViewBox);
	status.RequestRedraw ();
}

void NodeUIManager::CenterToWindow (NodeUIDrawingEnvironment& env)
{
	Rect boundingRect;
	if (!GetBoundingRect (env, boundingRect)) {
		return;
	}

	double scale = env.GetWindowScale ();
	const DrawingContext& drawingContext = env.GetDrawingContext ();
	Size contextSize (drawingContext.GetWidth (), drawingContext.GetHeight ());
	Point boundingRectPosition = boundingRect.GetPosition () * scale;
	Size boundingRectSize = boundingRect.GetSize () * scale;

	Point centerTopLeft (
		boundingRectPosition.GetX () - (contextSize.GetWidth () - boundingRectSize.GetWidth ()) / 2.0,
		boundingRectPosition.GetY () - (contextSize.GetHeight () - boundingRectSize.GetHeight ()) / 2.0
	);
	ViewBox newViewBox (-centerTopLeft, scale);
	SetViewBox (newViewBox);
	status.RequestRedraw ();
}

void NodeUIManager::FitToWindow (NodeUIDrawingEnvironment& env)
{
	Rect boundingRect;
	if (!GetBoundingRect (env, boundingRect)) {
		return;
	}

	double viewPadding = env.GetSkinParams ().GetNodePadding ();
	const DrawingContext& drawingContext = env.GetDrawingContext ();
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

void NodeUIManager::New (NodeUIEnvironment& env)
{
	Clear (env);
	RequestRecalculateAndRedraw ();
}

bool NodeUIManager::Open (NodeUIEnvironment& env, NE::InputStream& inputStream)
{
	Clear (env);
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

bool NodeUIManager::CopyToNodeManager (const NE::NodeCollection& nodeCollection, NE::NodeManager& result) const
{
	NE::NodeCollectionFilter nodeFilter (nodeCollection);
	NE::EmptyAppendEventHandler eventHandler;
	return NE::NodeManagerMerge::AppendNodeManager (nodeManager, result, nodeFilter, eventHandler);
}

NE::NodeCollection NodeUIManager::PasteFromNodeManager (const NE::NodeManager& source)
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
	CopyToNodeManager (nodeCollection, tempNodeManager);
	return PasteFromNodeManager (tempNodeManager);
}

void NodeUIManager::SaveUndoState ()
{
	undoHandler.SaveUndoState (nodeManager);
}

bool NodeUIManager::CanUndo () const
{
	return undoHandler.CanUndo ();
}

bool NodeUIManager::CanRedo () const
{
	return undoHandler.CanRedo ();
}

bool NodeUIManager::Undo (NE::EvaluationEnv& env)
{
	NodeUIManagerUpdateEventHandler eventHandler (*this, env);
	bool success = undoHandler.Undo (nodeManager, eventHandler);
	InvalidateDrawingsForInvalidatedNodes ();
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::Redo (NE::EvaluationEnv& env)
{
	NodeUIManagerUpdateEventHandler eventHandler (*this, env);
	bool success = undoHandler.Redo (nodeManager, eventHandler);
	InvalidateDrawingsForInvalidatedNodes ();
	RequestRecalculateAndRedraw ();
	return success;
}

bool NodeUIManager::AddUINodeGroup (const UINodeGroupPtr& group)
{
	bool success = nodeManager.AddNodeGroup (group);
	InvalidateAllNodeGroupsDrawing ();
	return success;
}

void NodeUIManager::DeleteUINodeGroup (const UINodeGroupPtr& group)
{
	nodeManager.DeleteNodeGroup (group);
	RequestRedraw ();
}

void NodeUIManager::AddNodesToUIGroup (const UINodeGroupPtr& group, const NE::NodeCollection& nodeCollection)
{
	nodeCollection.Enumerate ([&] (const NE::NodeId& nodeId) {
		nodeManager.AddNodeToGroup (group, nodeId);
		return true;
	});
	InvalidateAllNodeGroupsDrawing ();
}

bool NodeUIManager::RemoveNodesFromUIGroup (const NE::NodeCollection& nodeCollection)
{
	nodeCollection.Enumerate ([&] (const NE::NodeId& nodeId) {
		nodeManager.RemoveNodeFromGroup (nodeId);
		return true;	
	});
	InvalidateAllNodeGroupsDrawing ();
	return true;
}

const NE::NodeCollection& NodeUIManager::GetUIGroupNodes (const UINodeGroupConstPtr& group) const
{
	return nodeManager.GetGroupNodes (group);
}

UINodeGroupConstPtr NodeUIManager::GetUINodeGroup (const NE::NodeId& nodeId) const
{
	return std::static_pointer_cast<const UINodeGroup> (nodeManager.GetNodeGroup (nodeId));
}

void NodeUIManager::EnumerateUINodeGroups (const std::function<bool (const UINodeGroupConstPtr&)>& processor) const
{
	nodeManager.EnumerateNodeGroups ([&] (const NE::NodeGroupConstPtr& nodeGroup) {
		return processor (std::static_pointer_cast<const UINodeGroup> (nodeGroup));
	});
}

void NodeUIManager::EnumerateUINodeGroups (const std::function<bool (const UINodeGroupPtr&)>& processor)
{
	nodeManager.EnumerateNodeGroups ([&] (const NE::NodeGroupPtr& nodeGroup) {
		return processor (std::static_pointer_cast<UINodeGroup> (nodeGroup));
	});
}

void NodeUIManager::ExecuteCommand (NodeUIManagerCommand& command)
{
	if (command.IsUndoable ()) {
		SaveUndoState ();
	}
	command.Do (*this);
	status.RequestSave ();
}

void NodeUIManager::ExecuteCommand (NodeUIManagerCommandPtr& command)
{
	ExecuteCommand (*command);
}

void NodeUIManager::Clear (NodeUIEnvironment& env)
{
	double windowScale = env.GetWindowScale ();

	selectedNodes.Clear ();
	undoHandler.Clear ();
	nodeManager.Clear ();

	viewBox.Set (Point (0.0, 0.0), windowScale);
	status.Reset ();
}

void NodeUIManager::InvalidateDrawingsForInvalidatedNodes ()
{
	std::vector<UINodePtr> nodesToInvalidate;
	EnumerateUINodes ([&] (const UINodePtr& uiNode) {
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

void NodeUIManager::UpdateInternal (NodeUICalculationEnvironment& env, InternalUpdateMode mode)
{
	if (status.NeedToRecalculate ()) {
		env.OnEvaluationBegin ();
		if (mode == InternalUpdateMode::Normal) {
			nodeManager.EvaluateAllNodes (env.GetEvaluationEnv ());
		} else if (mode == InternalUpdateMode::Manual) {
			nodeManager.ForceEvaluateAllNodes (env.GetEvaluationEnv ());
		}
		env.OnEvaluationEnd ();

		env.OnValuesRecalculated ();
		status.ResetRecalculate ();
	}
	if (status.NeedToRedraw ()) {
		env.OnRedrawRequested ();
		status.ResetRedraw ();
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
