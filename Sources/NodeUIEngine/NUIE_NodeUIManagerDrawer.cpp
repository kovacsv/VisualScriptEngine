#include "NUIE_NodeUIManagerDrawer.hpp"
#include "NUIE_ContextDecorators.hpp"
#include "NUIE_EnvironmentDecorators.hpp"
#include "NUIE_SkinParams.hpp"

#include <cmath>

namespace NUIE
{

static void GetBezierControlPoints (const Point& beg, const Point& end, Point& controlPoint1, Point& controlPoint2)
{
	double bezierOffsetVal = std::fabs (beg.GetX () - end.GetX ()) / 2.0;
	Point bezierOffset (bezierOffsetVal, 0.0);
	controlPoint1 = beg + bezierOffset;
	controlPoint2 = end - bezierOffset;
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
	uiManager (uiManager)
{
	
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
			return uiManagerDrawer.GetNodeRect (drawingEnv, drawModifier, uiNode);
		}

	private:
		const NodeUIManager& uiManager;
		const NodeUIManagerDrawer& uiManagerDrawer;
		const NodeDrawingModifier* drawModifier;
		NodeUIDrawingEnvironment& drawingEnv;
	};

	ModifiedNodeRectGetter rectGetter (uiManager, *this, drawModifier, drawingEnv);
	uiManager.EnumerateNodeGroups ([&] (UINodeGroupConstPtr group) {
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
	uiManager.EnumerateNodes ([&] (UINodeConstPtr begNode) {
		bool begSelected = selectedNodes.Contains (begNode->GetId ());
		begNode->EnumerateUIOutputSlots ([&] (UIOutputSlotConstPtr outputSlot) {
			Point beg = GetOutputSlotConnPosition (drawingEnv, drawModifier, begNode, outputSlot->GetId ());
			uiManager.EnumerateConnectedUIInputSlots (outputSlot, [&] (UIInputSlotConstPtr inputSlot) {
				UINodeConstPtr endNode = uiManager.GetNode (inputSlot->GetOwnerNodeId ());
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
						if (inputSlot->GetConnectionDisplayMode () == ConnectionDisplayMode::Normal) {
							DrawConnection (drawingEnv, pen, beg, end);
						}
					}
				}
			});
			return true;
		});
		return true;
	});

	if (drawModifier != nullptr) {
		drawModifier->EnumerateTemporaryConnections ([&] (const Point& beg, const Point& end, NodeDrawingModifier::Direction dir) {
			if (IsConnectionVisible (drawingEnv, beg, end)) {
				DrawTemporaryConnection (drawingEnv, pen, beg, end, dir);
			}
		});
	}
}

void NodeUIManagerDrawer::DrawConnection (NodeUIDrawingEnvironment& drawingEnv, const Pen& pen, const Point& beg, const Point& end) const
{
	DrawingContext& context = drawingEnv.GetDrawingContext ();
	Point controlPoint1, controlPoint2;
	GetBezierControlPoints (beg, end, controlPoint1, controlPoint2);
	context.DrawBezier (beg, controlPoint1, controlPoint2, end, pen);
}

void NodeUIManagerDrawer::DrawTemporaryConnection (NodeUIDrawingEnvironment& drawingEnv, const Pen& pen, const Point& beg, const Point& end, NodeDrawingModifier::Direction dir) const
{
	const SkinParams& skinParams = drawingEnv.GetSkinParams ();
	DrawingContext& context = drawingEnv.GetDrawingContext ();
	DrawConnection (drawingEnv, pen, beg, end);
	if (skinParams.GetConnectionMarker () == SkinParams::ConnectionMarker::Circle) {
		const Color& markerColor = skinParams.GetSlotTextBackgroundColor ();
		const Size& markerSize = skinParams.GetConnectionMarkerSize ();
		if (dir == NodeDrawingModifier::Direction::Forward) {
			Rect markerRect = Rect::FromCenterAndSize (end, markerSize);
			context.FillEllipse (markerRect, markerColor);
			context.DrawEllipse (markerRect, pen);
		} else if (dir == NodeDrawingModifier::Direction::Backward) {
			Rect markerRect = Rect::FromCenterAndSize (beg, markerSize);
			context.FillEllipse (markerRect, markerColor);
			context.DrawEllipse (markerRect, pen);
		}
	}
}

void NodeUIManagerDrawer::DrawNodes (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier) const
{
	uiManager.EnumerateNodes ([&] (UINodeConstPtr uiNode) {
		if (!IsNodeVisible (drawingEnv, scaleIndependentData, drawModifier, uiNode)) {
			return true;
		}
		
		SelectionMode selectionMode = SelectionMode::NotSelected;
		if (uiManager.GetSelection ().ContainsNode (uiNode->GetId ())) {
			selectionMode = SelectionMode::Selected;
		}

		Point nodeOffset = drawModifier->GetNodeOffset (uiNode->GetId ());
		DrawNode (drawingEnv, scaleIndependentData, nodeOffset, selectionMode, uiNode);
		return true;
	});

	std::vector<std::pair<NE::NodeId, Point>> duplicatedNodes;
	drawModifier->EnumerateDuplicatedNodes ([&] (const NE::NodeId& nodeId, const Point& offset) {
		duplicatedNodes.push_back ({ nodeId, offset });
	});
	if (!duplicatedNodes.empty ()) {
		for (const auto& duplicatedNode : duplicatedNodes) {
			UINodeConstPtr uiNode = uiManager.GetNode (duplicatedNode.first);
			Point nodeOffset = drawModifier->GetNodeOffset (uiNode->GetId ());
			DrawNode (drawingEnv, scaleIndependentData, nodeOffset + duplicatedNode.second, SelectionMode::NotSelected, uiNode);
		}
	}
}

void NodeUIManagerDrawer::DrawNode (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const Point& offset, SelectionMode selectionMode, const UINodeConstPtr& uiNode) const
{
	ViewBox offsetViewBox (offset, 1.0);
	ViewBoxContextDecorator offsetContext (drawingEnv.GetDrawingContext (), offsetViewBox);
	DrawingEnvironmentContextDecorator offsetEnv (drawingEnv, offsetContext);
	DrawNode (offsetEnv, scaleIndependentData, selectionMode, uiNode);
}

void NodeUIManagerDrawer::DrawNode (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, SelectionMode selectionMode, const UINodeConstPtr& uiNode) const
{
	if (selectionMode == SelectionMode::Selected) {
		Rect nodeRect = uiNode->GetRect (drawingEnv);
		double selectionThickness = scaleIndependentData.GetSelectionThickness ();
		Rect selectionRect = nodeRect.Expand (Size (selectionThickness * 2.0, selectionThickness * 2.0));
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

bool NodeUIManagerDrawer::IsConnectionVisible (NodeUIDrawingEnvironment& drawingEnv, const Point& beg, const Point& end) const
{
	Point controlPoint1, controlPoint2;
	GetBezierControlPoints (beg, end, controlPoint1, controlPoint2);
	Rect boundingRect = GetBezierBoundingRect (beg, controlPoint1, controlPoint2, end);
	return IsRectVisible (drawingEnv, boundingRect);
}

bool NodeUIManagerDrawer::IsNodeVisible (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier, const UINodeConstPtr& uiNode) const
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

Rect NodeUIManagerDrawer::GetNodeRect (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINodeConstPtr& uiNode) const
{
	Rect nodeRect = uiNode->GetRect (drawingEnv);
	return nodeRect.Offset (drawModifier->GetNodeOffset (uiNode->GetId ()));
}

Point NodeUIManagerDrawer::GetOutputSlotConnPosition (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINodeConstPtr& uiNode, const NE::SlotId& slotId) const
{
	Point position = uiNode->GetOutputSlotConnPosition (drawingEnv, slotId);
	return position + drawModifier->GetNodeOffset (uiNode->GetId ());
}

Point NodeUIManagerDrawer::GetInputSlotConnPosition (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINodeConstPtr& uiNode, const NE::SlotId& slotId) const
{
	Point position = uiNode->GetInputSlotConnPosition (drawingEnv, slotId);
	return position + drawModifier->GetNodeOffset (uiNode->GetId ());
}

Rect ExtendNodeRect (NodeUIDrawingEnvironment& drawingEnv, const Rect& originalRect)
{
	const SkinParams& skinParams = drawingEnv.GetSkinParams ();
	const Size& slotMarkerSize = skinParams.GetSlotMarkerSize ();
	double extendSize = 0.0;
	if (skinParams.GetHiddenSlotMarker () != SkinParams::HiddenSlotMarker::None) {
		extendSize = slotMarkerSize.GetWidth () + slotMarkerSize.GetWidth () / 2.0;
	} else if (skinParams.GetSlotMarker () != SkinParams::SlotMarker::None) {
		extendSize = slotMarkerSize.GetWidth () / 2.0;
	}
	return originalRect.ExpandHorizontally (extendSize, extendSize);
}

Rect GetNodeExtendedRect (NodeUIDrawingEnvironment& drawingEnv, const UINodeConstPtr& uiNode)
{
	Rect nodeRect = uiNode->GetRect (drawingEnv);
	return ExtendNodeRect (drawingEnv, nodeRect);
}

}
