#include "UINodeLayouts.hpp"
#include "UINodePanels.hpp"
#include "UINode.hpp"

namespace NUIE
{

void DrawStatusHeaderWithSlotsLayout (	const UINode& uiNode,
										NodeUIDrawingEnvironment& env,
										NodeDrawingImage& drawingImage)
{
	DBGASSERT (uiNode.ValueIsCalculated ());
	NodeUIStatusHeaderPanel::NodeStatus nodeStatus = NodeUIStatusHeaderPanel::NodeStatus::HasNoValue;
	if (uiNode.GetCalculatedValue () != nullptr) {
		nodeStatus = NodeUIStatusHeaderPanel::NodeStatus::HasValue;
	}

	NodeUIPanelDrawer drawer;
	drawer.AddPanel (NodeUIPanelPtr (new NodeUIStatusHeaderPanel (uiNode.GetNodeName (), nodeStatus)));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.Draw (env, drawingImage);
}

void DrawHeaderWithSlotsAndButtonsLayout (	const UINode& uiNode,
											const std::string& leftButtonId,
											const std::wstring& leftButtonText,
											const std::string& rightButtonId,
											const std::wstring& rightButtonText,
											NodeUIDrawingEnvironment& env,
											NodeDrawingImage& drawingImage)
{
	std::wstring nodeText = L"<empty>";
	DBGASSERT (uiNode.ValueIsCalculated ());
	NE::ValuePtr nodeValue = uiNode.GetCalculatedValue ();
	if (nodeValue != nullptr) {
		nodeText = nodeValue->ToString ();
	}

	NodeUIPanelDrawer drawer;
	drawer.AddPanel (NodeUIPanelPtr (new NodeUIHeaderPanel (uiNode.GetNodeName ())));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUILeftRightButtonsPanel (leftButtonId, leftButtonText, rightButtonId, rightButtonText, nodeText, env)));
	drawer.Draw (env, drawingImage);
}

}
