#include "BI_UINodeLayouts.hpp"
#include "BI_UINodePanels.hpp"
#include "NUIE_UINode.hpp"

namespace BI
{

void DrawStatusHeaderWithSlotsLayout (	const NUIE::UINode& uiNode,
										NUIE::NodeUIDrawingEnvironment& env,
										NUIE::NodeDrawingImage& drawingImage)
{
	DBGASSERT (uiNode.ValueIsCalculated ());
	NodeUIStatusHeaderPanel::NodeStatus nodeStatus = NodeUIStatusHeaderPanel::NodeStatus::HasNoValue;
	if (uiNode.GetCalculatedValue () != nullptr) {
		nodeStatus = NodeUIStatusHeaderPanel::NodeStatus::HasValue;
	}

	NUIE::NodeUIPanelDrawer drawer;
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIStatusHeaderPanel (uiNode.GetNodeName (), nodeStatus)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.Draw (env, drawingImage);
}

void DrawHeaderWithSlotsAndButtonsLayout (	const NUIE::UINode& uiNode,
											const std::string& leftButtonId,
											const std::wstring& leftButtonText,
											const std::string& rightButtonId,
											const std::wstring& rightButtonText,
											NUIE::NodeUIDrawingEnvironment& env,
											NUIE::NodeDrawingImage& drawingImage)
{
	std::wstring nodeText = L"<empty>";
	DBGASSERT (uiNode.ValueIsCalculated ());
	NE::ValuePtr nodeValue = uiNode.GetCalculatedValue ();
	if (nodeValue != nullptr) {
		nodeText = nodeValue->ToString (env.GetStringSettings ());
	}

	NUIE::NodeUIPanelDrawer drawer;
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIHeaderPanel (uiNode.GetNodeName ())));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUILeftRightButtonsPanel (leftButtonId, leftButtonText, rightButtonId, rightButtonText, nodeText, env)));
	drawer.Draw (env, drawingImage);
}

}
