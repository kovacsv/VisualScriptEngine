#include "NUIE_NodeUIManagerDrawer.hpp"
#include "NUIE_NodeDrawingModifier.hpp"
#include "NUIE_ContextDecorators.hpp"
#include "NUIE_SkinParams.hpp"

namespace NUIE
{

// TODO: Delete
static Point NodeOffset (20.0, 20.0);

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
	
void NodeUIManagerDrawer::Draw (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	drawingContext.BeginDraw ();

	DrawBackground (env);
	{
		TextSkipperContextDecorator textSkipperContext (drawingContext, uiManager.IsPreviewMode ());
		ViewBoxContextDecorator viewBoxContext (textSkipperContext, uiManager.GetViewBox ());
		NodeUIDrawingEnvironmentContextDecorator drawEnv (env, viewBoxContext);
		DrawGroups (drawEnv, drawModifier);
		DrawConnections (drawEnv, drawModifier);
		DrawNodes (drawEnv, drawModifier);
	}
	DrawSelectionRect (env, drawModifier);

	drawingContext.EndDraw ();
}

void NodeUIManagerDrawer::DrawBackground (NodeUIDrawingEnvironment& env) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	Rect contextRect (0.0, 0.0, drawingContext.GetWidth (), drawingContext.GetHeight ());
	drawingContext.FillRect (contextRect, env.GetSkinParams ().GetBackgroundColor ());
}

void NodeUIManagerDrawer::DrawGroups (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const
{
	class NodeUIManagerNodeRectGetter : public NodeRectGetter
	{
	public:
		NodeUIManagerNodeRectGetter (const NodeUIManager& uiManager, const NodeDrawingModfier* drawModifier, NodeUIDrawingEnvironment& env) :
			uiManager (uiManager),
			drawModifier (drawModifier),
			env (env)
		{
	
		}

		virtual Rect GetNodeRect (const NE::NodeId& nodeId) const override
		{
			UINodeConstPtr uiNode = uiManager.GetUINode (nodeId);
			return uiNode->GetNodeRect (env).Offset (drawModifier->GetNodeOffset (nodeId));
		}

	private:
		const NodeUIManager& uiManager;
		const NodeDrawingModfier* drawModifier;
		NodeUIDrawingEnvironment& env;
	};

	NodeUIManagerNodeRectGetter rectGetter (uiManager, drawModifier, env);
	uiManager.EnumerateUINodeGroups ([&] (const UINodeGroupPtr& group) {
		Rect groupRect = group->GetRect (env, rectGetter);
		if (IsRectVisible (env, groupRect)) {
			group->Draw (env, rectGetter);
		}
		return true;
	});
}

void NodeUIManagerDrawer::DrawConnections (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const
{
	for (const UINode* uiNode : sortedNodeList) {
		uiNode->EnumerateUIOutputSlots ([&] (const UIOutputSlotConstPtr& outputSlot) {
			Point beg = GetOutputSlotConnPosition (env, drawModifier, uiNode, outputSlot->GetId ());
			uiManager.EnumerateConnectedInputSlots (outputSlot, [&] (const UIInputSlotConstPtr& inputSlot) {
				const UINode* endNode = nodeIdToNodeMap.GetUINode (inputSlot->GetOwnerNodeId ());
				if (DBGERROR (endNode == nullptr)) {
					return;
				}
				Point end = GetInputSlotConnPosition (env, drawModifier, endNode, inputSlot->GetId ());
				if (IsConnectionVisible (env, beg, end)) {
					DrawConnection (env, beg, end);
				}
			});
			return true;
		});
	}

	if (drawModifier != nullptr) {
		drawModifier->EnumerateTemporaryConnections ([&] (const Point& beg, const Point& end) {
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

void NodeUIManagerDrawer::DrawNodes (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const
{
	for (const UINode* uiNode: sortedNodeList) {
		if (!IsNodeVisible (env, drawModifier, uiNode)) {
			continue;
		}
		DrawNode (env, drawModifier, uiNode);
	}
}

void NodeUIManagerDrawer::DrawNode (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode) const
{
	ViewBox offsetViewBox (drawModifier->GetNodeOffset (uiNode->GetId ()), 1.0);
	ViewBoxContextDecorator offsetContext (env.GetDrawingContext (), offsetViewBox);
	NodeUIDrawingEnvironmentContextDecorator offsetEnv (env, offsetContext);
	DrawNode (offsetEnv, uiNode);
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

void NodeUIManagerDrawer::DrawSelectionRect (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const
{
	if (drawModifier != nullptr) {
		drawModifier->EnumerateSelectionRectangles ([&] (const Rect& viewRect) {
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

bool NodeUIManagerDrawer::IsNodeVisible (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode) const
{
	Rect nodeRect = GetNodeRect (env, drawModifier, uiNode);
	return IsRectVisible (env, nodeRect);
}

bool NodeUIManagerDrawer::IsRectVisible (NodeUIDrawingEnvironment& env, const Rect& rect) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	const DrawingContext& context = env.GetDrawingContext ();
	return Rect::IsInBounds (viewBox.ModelToView (rect), context.GetWidth (), context.GetHeight ());
}

Rect NodeUIManagerDrawer::GetNodeRect (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode) const
{
	Rect nodeRect = uiNode->GetNodeRect (env);
	return nodeRect.Offset (drawModifier->GetNodeOffset (uiNode->GetId ()));
}

Point NodeUIManagerDrawer::GetOutputSlotConnPosition (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const
{
	Point position = uiNode->GetOutputSlotConnPosition (env, slotId);
	return position + drawModifier->GetNodeOffset (uiNode->GetId ());
}

Point NodeUIManagerDrawer::GetInputSlotConnPosition (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const
{
	Point position = uiNode->GetInputSlotConnPosition (env, slotId);
	return position + drawModifier->GetNodeOffset (uiNode->GetId ());
}

}
