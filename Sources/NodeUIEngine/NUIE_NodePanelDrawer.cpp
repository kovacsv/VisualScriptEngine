#include "NUIE_NodePanelDrawer.hpp"
#include "NUIE_SkinParams.hpp"

#include <vector>
#include <algorithm>

namespace NUIE
{

static Rect ExtendNodeRect (const Rect& originalRect, const SkinParams& skinParams)
{
	const Size& slotMarkerSize = skinParams.GetSlotMarkerSize ();
	double extendSize = 0.0;
	if (skinParams.GetHiddenSlotMarker () != SkinParams::HiddenSlotMarker::None) {
		extendSize = slotMarkerSize.GetWidth () + slotMarkerSize.GetWidth () / 2.0;
	} else if (skinParams.GetSlotMarker () != SkinParams::SlotMarker::None) {
		extendSize = slotMarkerSize.GetWidth () / 2.0;
	}
	return originalRect.ExpandHorizontally (extendSize, extendSize);
}

NodePanelDrawer::NodePanelDrawer ()
{

}

NodePanelDrawer::~NodePanelDrawer ()
{

}

void NodePanelDrawer::AddPanel (const NodeUIPanelPtr& panel)
{
	panels.push_back (panel);
}

void NodePanelDrawer::Draw (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const
{
	const SkinParams& skinParams = env.GetSkinParams ();

	std::vector<Size> panelMinSizes;
	Size fullSize (0.0, 0.0);
	for (size_t i = 0; i < panels.size (); i++) {
		const NodeUIPanelPtr& panel = panels[i];
		Size panelMinSize = panel->GetMinSize (env);
		fullSize.SetWidth (std::max (fullSize.GetWidth (), panelMinSize.GetWidth ()));
		fullSize.SetHeight (fullSize.GetHeight () + panelMinSize.GetHeight ());
		panelMinSizes.push_back (panelMinSize);
	}

	Rect nodeRect = Rect::FromCenterAndSize (Point (0.0, 0.0), fullSize);
	double panelYOffset = 0.0;
	for (size_t i = 0; i < panels.size (); i++) {
		const NodeUIPanelPtr& panel = panels[i];
		const Size& panelMinSize = panelMinSizes[i];
		Point panelRectPosition (nodeRect.GetTopLeft () + Point (0.0, panelYOffset));
		Size panelRectSize (fullSize.GetWidth (), panelMinSize.GetHeight ());
		Rect panelRect = Rect::FromPositionAndSize (panelRectPosition, panelRectSize);
		panel->Draw (env, panelRect, drawingImage);
		panelYOffset += panelMinSize.GetHeight ();
	}

	drawingImage.AddItem (std::make_shared<DrawingRect> (nodeRect, skinParams.GetNodeBorderPen ()));
	drawingImage.SetNodeRect (nodeRect);

	Rect extendedNodeRect = ExtendNodeRect (nodeRect, skinParams);
	drawingImage.SetExtendedNodeRect (extendedNodeRect);
}

}
