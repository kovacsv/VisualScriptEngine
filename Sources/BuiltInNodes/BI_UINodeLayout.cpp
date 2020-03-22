#include "BI_UINodeLayout.hpp"

namespace BI
{

UINodeLayout::UINodeLayout ()
{

}

UINodeLayout::~UINodeLayout ()
{

}

void UINodeLayout::Draw (	const BasicUINode& uiNode,
							NUIE::NodeUIDrawingEnvironment& env,
							NUIE::NodeDrawingImage& drawingImage) const
{
	NUIE::NodePanelDrawer drawer;
	AddPanels (uiNode, env, drawer);
	drawer.Draw (env, drawingImage);
}

}
