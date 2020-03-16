#include "NUIE_NodeUIManagerDrawer.hpp"
#include "NUIE_NodeDrawingModifier.hpp"
#include "NUIE_ContextDecorators.hpp"
#include "NUIE_SkinParams.hpp"

#include <cmath>

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

NodeUIScaleIndependentData::NodeUIScaleIndependentData (const NodeUIManager& uiManager, const SkinParams& skinParams) :
	selectionThickness (0.0)
{
	double scale = uiManager.GetViewBox ().GetScale ();
	selectionThickness = skinParams.GetNodeSelectionRectPen ().GetThickness ();
	if (scale < 1.0) {
		selectionThickness /= scale;
	}
}

double NodeUIScaleIndependentData::GetSelectionThickness () const
{
	return selectionThickness;
}

NodeUIManagerDrawer::NodeUIManagerDrawer (const NodeUIManager& uiManager) :
	uiManager (uiManager),
	nodeIdToNodeMap (uiManager)
{
	InitSortedNodeList ();
}
	
void NodeUIManagerDrawer::Draw (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	
	drawingContext.BeginDraw ();
	DrawBackground (env);

	{
		NodeUIScaleIndependentData scaleIndependentData (uiManager, env.GetSkinParams ());
		PreviewContextDecorator textSkipperContext (drawingContext, uiManager.IsPreviewMode ());
		ViewBoxContextDecorator viewBoxContext (textSkipperContext, uiManager.GetViewBox ());
		NodeUIDrawingEnvironmentContextDecorator drawEnv (env, viewBoxContext);
		
		DrawGroups (drawEnv, drawModifier);
		DrawConnections (drawEnv, scaleIndependentData, drawModifier);
		DrawNodes (drawEnv, scaleIndependentData, drawModifier);
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

void NodeUIManagerDrawer::DrawGroups (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier) const
{
	class NodeUIManagerNodeRectGetter : public NodeRectGetter
	{
	public:
		NodeUIManagerNodeRectGetter (const NodeUIManager& uiManager, const NodeUIManagerDrawer& uiManagerDrawer, const NodeDrawingModifier* drawModifier, NodeUIDrawingEnvironment& env) :
			uiManager (uiManager),
			uiManagerDrawer (uiManagerDrawer),
			drawModifier (drawModifier),
			env (env)
		{
	
		}

		virtual Rect GetNodeRect (const NE::NodeId& nodeId) const override
		{
			UINodeConstPtr uiNode = uiManager.GetUINode (nodeId);
			return uiManagerDrawer.GetNodeRect (env, drawModifier, uiNode.get ());
		}

	private:
		const NodeUIManager& uiManager;
		const NodeUIManagerDrawer& uiManagerDrawer;
		const NodeDrawingModifier* drawModifier;
		NodeUIDrawingEnvironment& env;
	};

	NodeUIManagerNodeRectGetter rectGetter (uiManager, *this, drawModifier, env);
	uiManager.EnumerateUINodeGroups ([&] (const UINodeGroupConstPtr& group) {
		Rect groupRect = group->GetRect (env, rectGetter, uiManager.GetUIGroupNodes (group));
		if (IsRectVisible (env, groupRect)) {
			group->Draw (env, rectGetter, uiManager.GetUIGroupNodes (group));
		}
		return true;
	});
}

void NodeUIManagerDrawer::DrawConnections (NodeUIDrawingEnvironment& env, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	const Pen& pen = skinParams.GetConnectionLinePen ();
	Pen selectionPen (skinParams.GetNodeSelectionRectPen ().GetColor (), scaleIndependentData.GetSelectionThickness ());

	const NE::NodeCollection& selectedNodes = uiManager.GetSelectedNodes ();
	for (const UINode* uiNode : sortedNodeList) {
		bool begSelected = selectedNodes.Contains (uiNode->GetId ());
		uiNode->EnumerateUIOutputSlots ([&] (const UIOutputSlotConstPtr& outputSlot) {
			Point beg = GetOutputSlotConnPosition (env, drawModifier, uiNode, outputSlot->GetId ());
			uiManager.EnumerateConnectedInputSlots (outputSlot, [&] (const UIInputSlotConstPtr& inputSlot) {
				const UINode* endNode = nodeIdToNodeMap.GetUINode (inputSlot->GetOwnerNodeId ());
				if (DBGERROR (endNode == nullptr)) {
					return;
				}
				bool endSelected = selectedNodes.Contains (endNode->GetId ());
				Point end = GetInputSlotConnPosition (env, drawModifier, endNode, inputSlot->GetId ());
				if (IsConnectionVisible (env, beg, end)) {
					if (begSelected || endSelected) {
						DrawConnection (env, selectionPen, beg, end);
					} else {
						DrawConnection (env, pen, beg, end);
					}
				}
			});
			return true;
		});
	}

	if (drawModifier != nullptr) {
		drawModifier->EnumerateTemporaryConnections ([&] (const Point& beg, const Point& end) {
			if (IsConnectionVisible (env, beg, end)) {
				DrawConnection (env, pen, beg, end);
			}
		});
	}
}

void NodeUIManagerDrawer::DrawConnection (NodeUIDrawingEnvironment& env, const Pen& pen, const Point& beg, const Point& end) const
{
	double bezierOffsetVal = std::fabs (beg.GetX () - end.GetX ()) / 2.0;
	Point bezierOffset (bezierOffsetVal, 0.0);
	env.GetDrawingContext ().DrawBezier (beg, beg + bezierOffset, end - bezierOffset, end, pen);
}

void NodeUIManagerDrawer::DrawNodes (NodeUIDrawingEnvironment& env, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier) const
{
	for (const UINode* uiNode: sortedNodeList) {
		if (!IsNodeVisible (env, scaleIndependentData, drawModifier, uiNode)) {
			continue;
		}
		ViewBox offsetViewBox (drawModifier->GetNodeOffset (uiNode->GetId ()), 1.0);
		ViewBoxContextDecorator offsetContext (env.GetDrawingContext (), offsetViewBox);
		NodeUIDrawingEnvironmentContextDecorator offsetEnv (env, offsetContext);
		DrawNode (offsetEnv, scaleIndependentData, uiNode);
	}
}

void NodeUIManagerDrawer::DrawNode (NodeUIDrawingEnvironment& env, const NodeUIScaleIndependentData& scaleIndependentData, const UINode* uiNode) const
{
	Rect nodeRect = uiNode->GetNodeRect (env);
	double selectionThickness = scaleIndependentData.GetSelectionThickness ();
	Rect selectionRect = nodeRect.Expand (Size (selectionThickness * 2.0, selectionThickness * 2.0));
	if (uiManager.GetSelectedNodes ().Contains (uiNode->GetId ())) {
		env.GetDrawingContext ().FillRect (selectionRect, env.GetSkinParams ().GetNodeSelectionRectPen ().GetColor ());
		ColorBlenderContextDecorator selectionContext (env.GetDrawingContext (), env.GetSkinParams ().GetSelectionBlendColor ());
		NodeUIDrawingEnvironmentContextDecorator selectionEnv (env, selectionContext);
		uiNode->Draw (selectionEnv);
	} else {
		uiNode->Draw (env);
	}
}

void NodeUIManagerDrawer::DrawSelectionRect (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier) const
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

	std::sort (sortedNodeList.begin (), sortedNodeList.end (), [&] (const UINode* a, const UINode* b) {
		return a->GetId () < b->GetId ();
	});
}

bool NodeUIManagerDrawer::IsConnectionVisible (NodeUIDrawingEnvironment& env, const Point& beg, const Point& end) const
{
	Rect connectionRect = Rect::FromTwoPoints (beg, end);
	return IsRectVisible (env, connectionRect);
}

bool NodeUIManagerDrawer::IsNodeVisible (NodeUIDrawingEnvironment& env, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier, const UINode* uiNode) const
{
	Rect nodeRect = GetNodeRect (env, drawModifier, uiNode);
	double selectionThickness = scaleIndependentData.GetSelectionThickness ();
	Rect selectionRect = nodeRect.Expand (Size (selectionThickness * 2.0, selectionThickness * 2.0));
	return IsRectVisible (env, selectionRect);
}

bool NodeUIManagerDrawer::IsRectVisible (NodeUIDrawingEnvironment& env, const Rect& rect) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	const DrawingContext& context = env.GetDrawingContext ();
	return Rect::IsInBounds (viewBox.ModelToView (rect), context.GetWidth (), context.GetHeight ());
}

Rect NodeUIManagerDrawer::GetNodeRect (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier, const UINode* uiNode) const
{
	Rect nodeRect = uiNode->GetNodeRect (env);
	return nodeRect.Offset (drawModifier->GetNodeOffset (uiNode->GetId ()));
}

Point NodeUIManagerDrawer::GetOutputSlotConnPosition (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const
{
	Point position = uiNode->GetOutputSlotConnPosition (env, slotId);
	return position + drawModifier->GetNodeOffset (uiNode->GetId ());
}

Point NodeUIManagerDrawer::GetInputSlotConnPosition (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const
{
	Point position = uiNode->GetInputSlotConnPosition (env, slotId);
	return position + drawModifier->GetNodeOffset (uiNode->GetId ());
}

double NodeUIManagerDrawer::GetNodeSelectionThickness (NodeUIDrawingEnvironment& env) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	double selectionThickness = skinParams.GetNodeSelectionRectPen ().GetThickness ();
	double scale = uiManager.GetViewBox ().GetScale ();
	if (scale < 1.0) {
		return selectionThickness / scale;
	}
	return selectionThickness;
}

}
