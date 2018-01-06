#include "NodeUIManager.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "Debug.hpp"
#include "SkinParams.hpp"
#include "NodeUIManagerDrawer.hpp"

namespace NUIE
{

static const size_t NodeUIManagerVersion = 1;

NodeUIManager::SelectedNodes::SelectedNodes ()
{

}

NodeUIManager::SelectedNodes::SelectedNodes (const std::unordered_set<NE::NodeId>& selection) :
	selection (selection)
{

}

NodeUIManager::SelectedNodes::~SelectedNodes ()
{

}

bool NodeUIManager::SelectedNodes::Contains (const NE::NodeId& nodeId) const
{
	return selection.find (nodeId) != selection.end ();
}

size_t NodeUIManager::SelectedNodes::Count () const
{
	return selection.size ();
}

void NodeUIManager::SelectedNodes::Enumerate (const std::function<void (const NE::NodeId&)>& processor) const
{
	for (const NE::NodeId& nodeId : selection) {
		processor (nodeId);
	}
}

void NodeUIManager::SelectedNodes::Insert (const NE::NodeId& nodeId)
{
	selection.insert (nodeId);
}

void NodeUIManager::SelectedNodes::Erase (const NE::NodeId& nodeId)
{
	selection.erase (nodeId);
}

void NodeUIManager::SelectedNodes::Clear ()
{
	selection.clear ();
}

NodeUIManager::Status::Status ()
{
	Reset ();
}

void NodeUIManager::Status::Reset ()
{
	needToRecalculate = false;
	needToRedraw = false;
}

void NodeUIManager::Status::RequestRecalculate ()
{
	needToRecalculate = true;
}

void NodeUIManager::Status::RequestRedraw ()
{
	needToRedraw = true;
}

bool NodeUIManager::Status::NeedToRecalculate () const
{
	return needToRecalculate;
}

bool NodeUIManager::Status::NeedToRedraw () const
{
	return needToRedraw;
}

NodeUIManager::NodeUIManager () :
	nodeManager (),
	selectedNodes (),
	viewBox (Point (0, 0), 1.0),
	status ()
{

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
	status.RequestRecalculate ();
	uiNode->OnCreated (env);
	return uiNode;
}

bool NodeUIManager::DeleteNode (const UINodePtr& uiNode, NE::EvaluationEnv& env)
{
	if (DBGERROR (uiNode == nullptr)) {
		return false;
	}
	selectedNodes.Erase (uiNode->GetId ());
	uiNode->OnDeleted (env);
	InvalidateNodeDrawing (uiNode);
	if (!nodeManager.DeleteNode (uiNode)) {
		return false;
	}
	status.RequestRecalculate ();
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

const NodeUIManager::SelectedNodes& NodeUIManager::GetSelectedNodes () const
{
	return selectedNodes;
}

void NodeUIManager::SetSelectedNodes (const SelectedNodes& newSelectedNodes)
{
	selectedNodes = newSelectedNodes;
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
	status.RequestRecalculate ();
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	return nodeManager.ConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}

bool NodeUIManager::DisconnectOutputSlotFromInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot)
{
	status.RequestRecalculate ();
	InvalidateNodeDrawing (inputSlot->GetOwnerNodeId ());
	return nodeManager.DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot);
}

bool NodeUIManager::HasConnectedInputSlots (const UIOutputSlotConstPtr& outputSlot) const
{
	return nodeManager.HasConnectedInputSlots (outputSlot);
}

bool NodeUIManager::HasConnectedOutputSlots (const UIInputSlotConstPtr& inputSlot) const
{
	return nodeManager.HasConnectedOutputSlots (inputSlot);
}

void NodeUIManager::EnumerateConnections (const std::function<void (const NE::NodeId&, const NE::SlotId&, const NE::NodeId&, const NE::SlotId&)>& processor) const
{
	nodeManager.EnumerateConnections (processor);
}

void NodeUIManager::EnumerateConnectedInputSlots (const NE::OutputSlotConstPtr& outputSlot, const std::function<void (const NE::InputSlotConstPtr&)>& processor) const
{
	nodeManager.EnumerateConnectedInputSlots (outputSlot, processor);
}

void NodeUIManager::EnumerateConnectedOutputSlots (const NE::InputSlotConstPtr& inputSlot, const std::function<void (const NE::OutputSlotConstPtr&)>& processor) const
{
	nodeManager.EnumerateConnectedOutputSlots (inputSlot, processor);
}

void NodeUIManager::EnumerateConnectedInputSlots (const UIOutputSlotConstPtr& outputSlot, const std::function<void (UIInputSlotConstPtr)>& processor) const
{
	nodeManager.EnumerateConnectedInputSlots (outputSlot, [&] (const NE::InputSlotConstPtr& inputSlot) {
		processor (std::dynamic_pointer_cast<const UIInputSlot> (inputSlot));
	});
}

void NodeUIManager::EnumerateConnectedOutputSlots (const UIInputSlotConstPtr& inputSlot, const std::function<void (UIOutputSlotConstPtr)>& processor) const
{
	nodeManager.EnumerateConnectedOutputSlots (inputSlot, [&] (const NE::OutputSlotConstPtr& outputSlot) {
		processor (std::dynamic_pointer_cast<const UIOutputSlot> (outputSlot));
	});
}

UINodePtr NodeUIManager::GetUINode (const NE::NodeId& nodeId)
{
	NE::NodePtr node = nodeManager.GetNode (nodeId);
	return std::dynamic_pointer_cast<UINode> (node);
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

void NodeUIManager::RequestRecalculate ()
{
	status.RequestRecalculate ();
}

void NodeUIManager::RequestRedraw ()
{
	status.RequestRedraw ();
}

void NodeUIManager::InvalidateAllNodesDrawing ()
{
	EnumerateUINodes ([&] (const UINodePtr& uiNode) {
		uiNode->InvalidateDrawing ();
		return true;
	});
}

void NodeUIManager::InvalidateNodeDrawing (const NE::NodeId& nodeId)
{
	UINodePtr uiNode = GetUINode (nodeId);
	InvalidateNodeDrawing (uiNode);
}

void NodeUIManager::InvalidateNodeDrawing (const UINodePtr& uiNode)
{
	uiNode->InvalidateDrawing ();
	nodeManager.EnumerateDependentNodes (uiNode, [&] (const NE::NodeId& dependentNodeId) {
		UINodePtr dependentNode = GetUINode (dependentNodeId);
		InvalidateNodeDrawing (dependentNode);
	});
	status.RequestRedraw ();
}

void NodeUIManager::Update (NodeUICalculationEnvironment& env)
{
	if (status.NeedToRecalculate ()) {
		EnumerateUINodes ([&] (const UINodePtr& uiNode) {
			uiNode->Evaluate (env.GetEvaluationEnv ());
			return true;
		});
		status.RequestRedraw ();
	}
	if (status.NeedToRedraw ()) {
		env.RequestRedraw ();
	}
	status.Reset ();
}

void NodeUIManager::Draw (NodeUIDrawingEnvironment& env, const NodeDrawingExtension* drawingExtension)
{
	NodeUIManagerDrawer drawer (*this);
	drawer.Draw (env, drawingExtension);
}

void NodeUIManager::ResizeContext (NodeUIDrawingEnvironment& env, int newWidth, int newHeight)
{
	env.GetDrawingContext ().Resize (newWidth, newHeight);
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

void NodeUIManager::Clear ()
{
	nodeManager.Clear ();
	viewBox.Reset ();
	selectedNodes.Clear ();
	status.RequestRecalculate ();
}

bool NodeUIManager::Load (NE::InputStream& inputStream)
{
	bool success = true;
	Clear ();

	size_t version;
	inputStream.Read (version);
	ReadViewBox (inputStream, viewBox);
	if (nodeManager.Read (inputStream) != NE::Stream::Status::NoError) {
		success = false;
	}
	status.RequestRecalculate ();
	return success;
}

bool NodeUIManager::Save (NE::OutputStream& outputStream) const
{
	bool success = true;
	outputStream.Write (NodeUIManagerVersion);
	WriteViewBox (outputStream, viewBox);
	if (nodeManager.Write (outputStream) != NE::Stream::Status::NoError) {
		success = false;
	}
	return success;
}

}
