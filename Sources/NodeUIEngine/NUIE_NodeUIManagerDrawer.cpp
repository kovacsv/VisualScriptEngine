#include "NUIE_NodeUIManagerDrawer.hpp"
#include "NUIE_NodeDrawingModifier.hpp"
#include "NUIE_ContextDecorators.hpp"
#include "NUIE_EnvironmentDecorators.hpp"
#include "NUIE_SkinParams.hpp"

#include <cmath>

namespace NUIE
{

NodeIdToNodeMap::NodeIdToNodeMap (const NodeUIManager& uiManager)
{
	uiManager.EnumerateNodes ([&] (const UINodeConstPtr& uiNode) {
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
	
void NodeUIManagerDrawer::Draw (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier) const
{
	DrawingContext& drawingContext = drawingEnv.GetDrawingContext ();
	
	drawingContext.BeginDraw ();
	DrawBackground (drawingEnv);

	{
		NodeUIScaleIndependentData scaleIndependentData (uiManager, drawingEnv.GetSkinParams ());
		PreviewContextDecorator textSkipperContext (drawingContext, uiManager.IsPreviewMode ());
		ViewBoxContextDecorator viewBoxContext (textSkipperContext, uiManager.GetViewBox ());
		DrawingEnvironmentContextDecorator drawEnv (drawingEnv, viewBoxContext);
		
		DrawGroups (drawEnv, drawModifier);
		DrawConnections (drawEnv, scaleIndependentData, drawModifier);
		DrawNodes (drawEnv, scaleIndependentData, drawModifier);
	}

	DrawSelectionRect (drawingEnv, drawModifier);
	drawingContext.EndDraw ();
}

void NodeUIManagerDrawer::DrawBackground (NodeUIDrawingEnvironment& drawingEnv) const
{
	DrawingContext& drawingContext = drawingEnv.GetDrawingContext ();
	Rect contextRect (0.0, 0.0, drawingContext.GetWidth (), drawingContext.GetHeight ());
	drawingContext.FillRect (contextRect, drawingEnv.GetSkinParams ().GetBackgroundColor ());
}

void NodeUIManagerDrawer::DrawGroups (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier) const
{
	class ModifiedNodeRectGetter : public NodeRectGetter
	{
	public:
		ModifiedNodeRectGetter (const NodeUIManager& uiManager, const NodeUIManagerDrawer& uiManagerDrawer, const NodeDrawingModifier* drawModifier, NodeUIDrawingEnvironment& drawingEnv) :
			uiManager (uiManager),
			uiManagerDrawer (uiManagerDrawer),
			drawModifier (drawModifier),
			drawingEnv (drawingEnv)
		{
	
		}

		virtual Rect GetNodeRect (const NE::NodeId& nodeId) const override
		{
			UINodeConstPtr uiNode = uiManager.GetNode (nodeId);
			return uiManagerDrawer.GetNodeRect (drawingEnv, drawModifier, uiNode.get ());
		}

	private:
		const NodeUIManager& uiManager;
		const NodeUIManagerDrawer& uiManagerDrawer;
		const NodeDrawingModifier* drawModifier;
		NodeUIDrawingEnvironment& drawingEnv;
	};

	ModifiedNodeRectGetter rectGetter (uiManager, *this, drawModifier, drawingEnv);
	uiManager.EnumerateNodeGroups ([&] (const UINodeGroupConstPtr& group) {
		Rect groupRect = group->GetRect (drawingEnv, rectGetter, uiManager.GetGroupNodes (group));
		if (IsRectVisible (drawingEnv, groupRect)) {
			group->Draw (drawingEnv, rectGetter, uiManager.GetGroupNodes (group));
		}
		return true;
	});
}

void NodeUIManagerDrawer::DrawConnections (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier) const
{
	const SkinParams& skinParams = drawingEnv.GetSkinParams ();
	const Pen& pen = skinParams.GetConnectionLinePen ();
	Pen selectionPen (skinParams.GetNodeSelectionRectPen ().GetColor (), scaleIndependentData.GetSelectionThickness ());

	const Selection& selection = uiManager.GetSelection ();
	const NE::NodeCollection& selectedNodes = selection.GetNodes ();
	for (const UINode* begNode : sortedNodeList) {
		bool begSelected = selectedNodes.Contains (begNode->GetId ());
		begNode->EnumerateUIOutputSlots ([&] (const UIOutputSlotConstPtr& outputSlot) {
			Point beg = GetOutputSlotConnPosition (drawingEnv, drawModifier, begNode, outputSlot->GetId ());
			uiManager.EnumerateConnectedUIInputSlots (outputSlot, [&] (const UIInputSlotConstPtr& inputSlot) {
				const UINode* endNode = nodeIdToNodeMap.GetUINode (inputSlot->GetOwnerNodeId ());
				if (DBGERROR (endNode == nullptr)) {
					return;
				}
				if (!drawModifier->NeedToDrawConnection (begNode->GetId (), outputSlot->GetId (), endNode->GetId (), inputSlot->GetId ())) {
					return;
				}
				bool endSelected = selectedNodes.Contains (endNode->GetId ());
				Point end = GetInputSlotConnPosition (drawingEnv, drawModifier, endNode, inputSlot->GetId ());
				if (IsConnectionVisible (drawingEnv, beg, end)) {
					if (begSelected || endSelected) {
						DrawConnection (drawingEnv, selectionPen, beg, end);
					} else {
						DrawConnection (drawingEnv, pen, beg, end);
					}
				}
			});
			return true;
		});
	}

	if (drawModifier != nullptr) {
		drawModifier->EnumerateTemporaryConnections ([&] (const Point& beg, const Point& end) {
			if (IsConnectionVisible (drawingEnv, beg, end)) {
				DrawConnection (drawingEnv, pen, beg, end);
			}
		});
	}
}

void NodeUIManagerDrawer::DrawConnection (NodeUIDrawingEnvironment& drawingEnv, const Pen& pen, const Point& beg, const Point& end) const
{
	double bezierOffsetVal = std::fabs (beg.GetX () - end.GetX ()) / 2.0;
	Point bezierOffset (bezierOffsetVal, 0.0);
	drawingEnv.GetDrawingContext ().DrawBezier (beg, beg + bezierOffset, end - bezierOffset, end, pen);
}

void NodeUIManagerDrawer::DrawNodes (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier) const
{
	for (const UINode* uiNode: sortedNodeList) {
		if (!IsNodeVisible (drawingEnv, scaleIndependentData, drawModifier, uiNode)) {
			continue;
		}
		
		SelectionMode selectionMode = SelectionMode::NotSelected;
		if (uiManager.GetSelection ().ContainsNode (uiNode->GetId ())) {
			selectionMode = SelectionMode::Selected;
		}

		Point nodeOffset = drawModifier->GetNodeOffset (uiNode->GetId ());
		DrawNode (drawingEnv, scaleIndependentData, nodeOffset, selectionMode, uiNode);
	}

	std::vector<std::pair<NE::NodeId, Point>> duplicatedNodes;
	drawModifier->EnumerateDuplicatedNodes ([&] (const NE::NodeId& nodeId, const Point& offset) {
		duplicatedNodes.push_back ({ nodeId, offset });
	});
	if (!duplicatedNodes.empty ()) {
		std::sort (duplicatedNodes.begin (), duplicatedNodes.end (), [&] (const std::pair<NE::NodeId, Point>& a, const std::pair<NE::NodeId, Point>& b) {
			return a.first < b.first;
		});
		for (const auto& duplicatedNode : duplicatedNodes) {
			const UINode* uiNode = nodeIdToNodeMap.GetUINode (duplicatedNode.first);
			Point nodeOffset = drawModifier->GetNodeOffset (uiNode->GetId ());
			DrawNode (drawingEnv, scaleIndependentData, nodeOffset + duplicatedNode.second, SelectionMode::NotSelected, uiNode);
		}
	}
}

void NodeUIManagerDrawer::DrawNode (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const Point& offset, SelectionMode selectionMode, const UINode* uiNode) const
{
	ViewBox offsetViewBox (offset, 1.0);
	ViewBoxContextDecorator offsetContext (drawingEnv.GetDrawingContext (), offsetViewBox);
	DrawingEnvironmentContextDecorator offsetEnv (drawingEnv, offsetContext);
	DrawNode (offsetEnv, scaleIndependentData, selectionMode, uiNode);
}

void NodeUIManagerDrawer::DrawNode (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, SelectionMode selectionMode, const UINode* uiNode) const
{
	Rect nodeRect = uiNode->GetRect (drawingEnv);
	double selectionThickness = scaleIndependentData.GetSelectionThickness ();
	Rect selectionRect = nodeRect.Expand (Size (selectionThickness * 2.0, selectionThickness * 2.0));
	if (selectionMode == SelectionMode::Selected) {
		drawingEnv.GetDrawingContext ().FillRect (selectionRect, drawingEnv.GetSkinParams ().GetNodeSelectionRectPen ().GetColor ());
		ColorBlenderContextDecorator selectionContext (drawingEnv.GetDrawingContext (), drawingEnv.GetSkinParams ().GetSelectionBlendColor ());
		DrawingEnvironmentContextDecorator selectionEnv (drawingEnv, selectionContext);
		uiNode->Draw (selectionEnv);
	} else {
		uiNode->Draw (drawingEnv);
	}
}

void NodeUIManagerDrawer::DrawSelectionRect (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier) const
{
	if (drawModifier != nullptr) {
		drawModifier->EnumerateSelectionRectangles ([&] (const Rect& viewRect) {
			drawingEnv.GetDrawingContext ().DrawRect (viewRect, drawingEnv.GetSkinParams ().GetSelectionRectPen ());
		});
	}
}

void NodeUIManagerDrawer::InitSortedNodeList () const
{
	nodeIdToNodeMap.Enumerate ([&] (const UINode* uiNode) {
		sortedNodeList.push_back (uiNode);
	});

	std::sort (sortedNodeList.begin (), sortedNodeList.end (), [&] (const UINode* a, const UINode* b) {
		return a->GetId () < b->GetId ();
	});
}

bool NodeUIManagerDrawer::IsConnectionVisible (NodeUIDrawingEnvironment& drawingEnv, const Point& beg, const Point& end) const
{
	Rect connectionRect = Rect::FromTwoPoints (beg, end);
	return IsRectVisible (drawingEnv, connectionRect);
}

bool NodeUIManagerDrawer::IsNodeVisible (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier, const UINode* uiNode) const
{
	Rect boundingRect = GetNodeRect (drawingEnv, drawModifier, uiNode);
	boundingRect = ExtendNodeRect (drawingEnv, boundingRect);

	double selectionThickness = scaleIndependentData.GetSelectionThickness ();
	boundingRect = boundingRect.Expand (Size (selectionThickness * 2.0, selectionThickness * 2.0));

	return IsRectVisible (drawingEnv, boundingRect);
}

bool NodeUIManagerDrawer::IsRectVisible (NodeUIDrawingEnvironment& drawingEnv, const Rect& rect) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	const DrawingContext& context = drawingEnv.GetDrawingContext ();
	return Rect::IsInBounds (viewBox.ModelToView (rect), context.GetWidth (), context.GetHeight ());
}

Rect NodeUIManagerDrawer::GetNodeRect (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINode* uiNode) const
{
	Rect nodeRect = uiNode->GetRect (drawingEnv);
	return nodeRect.Offset (drawModifier->GetNodeOffset (uiNode->GetId ()));
}

Point NodeUIManagerDrawer::GetOutputSlotConnPosition (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const
{
	Point position = uiNode->GetOutputSlotConnPosition (drawingEnv, slotId);
	return position + drawModifier->GetNodeOffset (uiNode->GetId ());
}

Point NodeUIManagerDrawer::GetInputSlotConnPosition (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const
{
	Point position = uiNode->GetInputSlotConnPosition (drawingEnv, slotId);
	return position + drawModifier->GetNodeOffset (uiNode->GetId ());
}

Rect ExtendNodeRect (NodeUIDrawingEnvironment& drawingEnv, const Rect& originalRect)
{
	const SkinParams& skinParams = drawingEnv.GetSkinParams ();
	if (!skinParams.NeedToDrawSlotCircles ()) {
		return originalRect;
	}
	const Size& slotCircleSize = skinParams.GetSlotCircleSize ();
	return originalRect.Expand (Size (slotCircleSize.GetWidth (), 0.0));
}

Rect GetNodeExtendedRect (NodeUIDrawingEnvironment& drawingEnv, const UINode* uiNode)
{
	Rect nodeRect = uiNode->GetRect (drawingEnv);
	return ExtendNodeRect (drawingEnv, nodeRect);
}

}
