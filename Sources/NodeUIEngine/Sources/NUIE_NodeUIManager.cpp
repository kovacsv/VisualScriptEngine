#include "NUIE_NodeUIManager.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_Debug.hpp"
#include "NUIE_NodeDrawingModifier.hpp"
#include "NUIE_NodeUIManagerDrawer.hpp"

namespace NUIE
{

static const size_t NodeUIManagerVersion = 1;

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
	copyPasteHandler (),
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
	uiNode->OnCreated (env);
	status.RequestRecalculate ();
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

const NE::NodeCollection& NodeUIManager::GetSelectedNodes () const
{
	return selectedNodes;
}

void NodeUIManager::SetSelectedNodes (const NE::NodeCollection& newSelectedNodes)
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

bool NodeUIManager::DisconnectAllInputSlotsFromOutputSlot (const UIOutputSlotConstPtr& outputSlot)
{
	status.RequestRecalculate ();
	InvalidateNodeDrawing (outputSlot->GetOwnerNodeId ());
	return nodeManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
}

bool NodeUIManager::DisconnectAllOutputSlotsFromInputSlot (const UIInputSlotConstPtr& inputSlot)
{
	status.RequestRecalculate ();
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

void NodeUIManager::InvalidateNodeValue (const NE::NodeId& nodeId)
{
	UINodePtr uiNode = GetUINode (nodeId);
	InvalidateNodeValue (uiNode);
}

void NodeUIManager::InvalidateNodeValue (const UINodePtr& uiNode)
{
	uiNode->InvalidateValue ();
	status.RequestRecalculate ();
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
	NE::NodeGroupPtr group = nodeManager.GetNodeGroup (nodeid);
	if (group == nullptr) {
		return;
	}

	UINodeGroupPtr uiGroup = std::static_pointer_cast<UINodeGroup> (group);
	uiGroup->InvalidateGroupDrawing ();
}

void NodeUIManager::InvalidateNodeGroupDrawing (const UINodePtr& uiNode)
{
	InvalidateNodeGroupDrawing (uiNode->GetId ());
}

void NodeUIManager::InvalidateAllNodeGroupDrawing ()
{
	EnumerateUINodeGroups ([&] (const UINodeGroupPtr& group) {
		group->InvalidateGroupDrawing ();
		return true;
	});
}

void NodeUIManager::Update (NodeUICalculationEnvironment& env)
{
	if (status.NeedToRecalculate ()) {
		nodeManager.EvaluateAllNodes (env.GetEvaluationEnv ());
		env.OnValuesRecalculated ();
		status.RequestRedraw ();
	}
	if (status.NeedToRedraw ()) {
		env.OnRedrawRequested ();
	}
	status.Reset ();
}

void NodeUIManager::Draw (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawingModifier)
{
	NodeUIManagerDrawer drawer (*this);
	drawer.Draw (env, drawingModifier);
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
	selectedNodes.Clear ();
	nodeManager.Clear ();
	viewBox.Reset ();
	status.RequestRecalculate ();
}

bool NodeUIManager::Load (NE::InputStream& inputStream)
{
	Clear ();

	size_t version;
	inputStream.Read (version);
	ReadViewBox (inputStream, viewBox);
	nodeManager.Read (inputStream);
	status.RequestRecalculate ();
	bool success = (inputStream.GetStatus () == NE::Stream::Status::NoError);
	return success;
}

bool NodeUIManager::Save (NE::OutputStream& outputStream) const
{
	outputStream.Write (NodeUIManagerVersion);
	WriteViewBox (outputStream, viewBox);
	nodeManager.Write (outputStream);
	bool success = (outputStream.GetStatus () == NE::Stream::Status::NoError);
	return success;
}

bool NodeUIManager::CanPaste () const
{
	return copyPasteHandler.CanPaste ();
}

bool NodeUIManager::Copy (const NE::NodeCollection& nodeCollection)
{
	return copyPasteHandler.CopyFrom (nodeManager, nodeCollection);
}

bool NodeUIManager::Paste ()
{
	bool success = copyPasteHandler.PasteTo (nodeManager);
	RequestRecalculate ();
	RequestRedraw ();
	return success;
}

bool NodeUIManager::AddUINodeGroup (const UINodeGroupPtr& group)
{
	bool success = nodeManager.AddNodeGroup (group);
	InvalidateAllNodeGroupDrawing ();
	return success;
}

void NodeUIManager::DeleteUINodeGroup (const UINodeGroupPtr& group)
{
	nodeManager.DeleteNodeGroup (group);
}

bool NodeUIManager::RemoveNodesFromGroup (const NE::NodeCollection& nodeCollection)
{
	nodeCollection.Enumerate ([&] (const NE::NodeId& nodeId) {
		nodeManager.RemoveNodeFromGroup (nodeId);
		return true;	
	});
	InvalidateAllNodeGroupDrawing ();
	return true;
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

}
