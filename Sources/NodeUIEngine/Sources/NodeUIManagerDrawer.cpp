#include "NodeUIManagerDrawer.hpp"
#include "NodeDrawingExtension.hpp"
#include "ContextDecorators.hpp"
#include "SkinParams.hpp"

namespace NUIE
{

NodeIdToNodeMap::NodeIdToNodeMap (const NodeUIManager& uiManager)
{
	uiManager.EnumerateUINodes ([&] (const UINodeConstPtr& uiNode) {
		const UINode* uiNodePtr = uiNode.get ();
		Insert (uiNodePtr->GetId (), uiNodePtr);
		return true;
	});
}

void NodeIdToNodeMap::Insert (const NE::NodeId& nodeId, const UINode* uiNode)
{
	nodeIdToNodeMap.insert ({ nodeId, uiNode });
}

const UINode* NodeIdToNodeMap::GetUINode (const NE::NodeId& nodeId) const
{
	auto found = nodeIdToNodeMap.find (nodeId);
	if (found == nodeIdToNodeMap.end ()) {
		return nullptr;
	}
	return found->second;
}

void NodeIdToNodeMap::Enumerate (const std::function<void (const UINode*)>& processor) const
{
	for (const auto& it : nodeIdToNodeMap) {
		processor (it.second);
	}
}

NodeUIManagerDrawer::NodeUIManagerDrawer (const NodeUIManager& uiManager) :
	uiManager (uiManager),
	nodeIdToNodeMap (uiManager)
{
	InitSortedNodeList ();
}
	
void NodeUIManagerDrawer::Draw (NodeUIDrawingEnvironment& env, const NodeDrawingExtension* drawExt) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	drawingContext.BeginDraw ();

	DrawBackground (env);
	{
		TextSkipperContextDecorator textSkipperContext (drawingContext, uiManager.IsPreviewMode ());
		ViewBoxContextDecorator viewBoxContext (textSkipperContext, uiManager.GetViewBox ());
		NodeUIDrawingEnvironmentContextDecorator drawEnv (env, viewBoxContext);
		DrawConnections (drawEnv, drawExt);
		DrawNodes (drawEnv);
	}
	DrawSelectionRect (env, drawExt);

	drawingContext.EndDraw ();
}

void NodeUIManagerDrawer::DrawBackground (NodeUIDrawingEnvironment& env) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	Rect contextRect (0.0, 0.0, drawingContext.GetWidth (), drawingContext.GetHeight ());
	drawingContext.FillRect (contextRect, env.GetSkinParams ().GetBackgroundColor ());
}

void NodeUIManagerDrawer::DrawConnections (NodeUIDrawingEnvironment& env, const NodeDrawingExtension* drawExt) const
{
	for (const UINode* uiNode : sortedNodeList) {
		uiNode->EnumerateOutputSlots ([&] (const NE::OutputSlotConstPtr& outputSlot) {
			Point beg = uiNode->GetOutputSlotConnPosition (env, outputSlot->GetId ());
			uiManager.EnumerateConnectedInputSlots (outputSlot, [&] (const NE::InputSlotConstPtr& inputSlot) {
				const UINode* endNode = nodeIdToNodeMap.GetUINode (inputSlot->GetOwnerNodeId ());
				if (DBGERROR (endNode == nullptr)) {
					return;
				}
				Point end = endNode->GetInputSlotConnPosition (env, inputSlot->GetId ());
				if (IsConnectionVisible (env, beg, end)) {
					DrawConnection (env, beg, end);
				}
			});
			return true;
		});
	}

	if (drawExt != nullptr) {
		drawExt->EnumerateTemporaryConnections ([&] (const Point& beg, const Point& end) {
			DrawConnection (env, beg, end);
		});
	}
}

void NodeUIManagerDrawer::DrawConnection (NodeUIDrawingEnvironment& env, const Point& beg, const Point& end) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	const Pen& pen = skinParams.GetConnectionLinePen ();

	double bezierOffsetVal = beg.DistanceTo (end) / 2.0;
	Point bezierOffset (bezierOffsetVal, 0.0);
	env.GetDrawingContext ().DrawBezier (beg, beg + bezierOffset, end - bezierOffset, end, pen);
}

void NodeUIManagerDrawer::DrawNodes (NodeUIDrawingEnvironment& env) const
{
	for (const UINode* uiNode: sortedNodeList) {
		if (!IsNodeVisible (env, uiNode)) {
			continue;
		}
		DrawNode (env, uiNode);
	}
}

void NodeUIManagerDrawer::DrawNode (NodeUIDrawingEnvironment& env, const UINode* uiNode) const
{
	const NE::NodeId& nodeId = uiNode->GetId ();
	const NodeCollection& selectedNodes = uiManager.GetSelectedNodes ();
	if (selectedNodes.Contains (nodeId)) {
		ColorBlenderContextDecorator selectionContext (env.GetDrawingContext (), env.GetSkinParams ().GetSelectionBlendColor ());
		NodeUIDrawingEnvironmentContextDecorator selectionEnv (env, selectionContext);
		uiNode->Draw (selectionEnv);
	} else {
		uiNode->Draw (env);
	}
}

void NodeUIManagerDrawer::DrawSelectionRect (NodeUIDrawingEnvironment& env, const NodeDrawingExtension* drawExt) const
{
	if (drawExt != nullptr) {
		drawExt->EnumerateSelectionRectangles ([&] (const Rect& viewRect) {
			env.GetDrawingContext ().DrawRect (viewRect, env.GetSkinParams ().GetSelectionRectPen ());
		});
	}
}

void NodeUIManagerDrawer::InitSortedNodeList () const
{
	std::unordered_set<const UINode*> visibleConnectedNodeSet;
	nodeIdToNodeMap.Enumerate ([&] (const UINode* uiNode) {
		sortedNodeList.push_back (uiNode);
	});

	std::sort (sortedNodeList.begin (), sortedNodeList.end (), [&] (const UINode* a, const UINode* b) -> bool {
		return a->GetId () < b->GetId ();
	});
}

bool NodeUIManagerDrawer::IsConnectionVisible (NodeUIDrawingEnvironment& env, const Point& beg, const Point& end) const
{
	Rect connectionRect = Rect::FromTwoPoints (beg, end);
	return IsRectVisible (env, connectionRect);
}

bool NodeUIManagerDrawer::IsNodeVisible (NodeUIDrawingEnvironment& env, const UINode* uiNode) const
{
	Rect nodeRect = uiNode->GetNodeRect (env);
	return IsRectVisible (env, nodeRect);
}

bool NodeUIManagerDrawer::IsRectVisible (NodeUIDrawingEnvironment& env, const Rect& rect) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	const DrawingContext& context = env.GetDrawingContext ();
	return Rect::IsInBounds (viewBox.ModelToView (rect), context.GetWidth (), context.GetHeight ());
}

}
