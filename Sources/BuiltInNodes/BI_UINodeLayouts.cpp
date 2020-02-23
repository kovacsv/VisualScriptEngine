#include "BI_UINodeLayouts.hpp"
#include "BI_UINodePanels.hpp"
#include "NE_Localization.hpp"
#include "NUIE_UINode.hpp"

namespace BI
{

void DrawStatusHeaderWithSlotsLayout (	const NUIE::UINode& uiNode,
										NUIE::NodeUIDrawingEnvironment& env,
										NUIE::NodeDrawingImage& drawingImage)
{
	NodeUIStatusHeaderPanel::NodeStatus nodeStatus = NodeUIStatusHeaderPanel::NodeStatus::HasNoValue;
	if (uiNode.HasCalculatedValue () && uiNode.GetCalculatedValue () != nullptr) {
		nodeStatus = NodeUIStatusHeaderPanel::NodeStatus::HasValue;
	}

	NUIE::NodePanelDrawer drawer;
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIStatusHeaderPanel (uiNode.GetNodeName (), nodeStatus)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.Draw (env, drawingImage);
}

void DrawHeaderWithSlotsAndButtonsLayout (	const NUIE::UINode& uiNode,
											const std::string& leftButtonId,
											const std::wstring& leftButtonText,
											const std::string& rightButtonId,
											const std::wstring& rightButtonText,
											const std::wstring& middleText,
											NUIE::NodeUIDrawingEnvironment& env,
											NUIE::NodeDrawingImage& drawingImage)
{
	NUIE::NodePanelDrawer drawer;
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIHeaderPanel (uiNode.GetNodeName ())));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUILeftRightButtonsPanel (leftButtonId, leftButtonText, rightButtonId, rightButtonText, middleText, env)));
	drawer.Draw (env, drawingImage);
}

void DrawHeaderWithSlotsAndSwitchLayout (	const NUIE::UINode& uiNode,
											const std::string& switchButtonId,
											const std::wstring& leftSwitchText,
											const std::wstring& rightSwitchText,
											short selectedIndex,
											NUIE::NodeUIDrawingEnvironment& env,
											NUIE::NodeDrawingImage& drawingImage)
{
	NUIE::NodePanelDrawer drawer;
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIHeaderPanel (uiNode.GetNodeName ())));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISwitchPanel (switchButtonId, leftSwitchText, rightSwitchText, selectedIndex, env)));
	drawer.Draw (env, drawingImage);
}

ButtonClickHandler::ButtonClickHandler ()
{

}

ButtonClickHandler::~ButtonClickHandler ()
{

}

SwitchClickHandler::SwitchClickHandler ()
{

}

SwitchClickHandler::~SwitchClickHandler ()
{

}

NUIE::EventHandlerResult HandleMouseClickOnButtonsLayout (	const NUIE::UINode& uiNode,
															const std::string& leftButtonId,
															const std::string& rightButtonId,
															NUIE::NodeUIEnvironment& env,
															NUIE::MouseButton mouseButton,
															const NUIE::Point& position,
															NUIE::UINodeCommandInterface& commandInterface,
															ButtonClickHandler& clickHandler)
{
	if (mouseButton != NUIE::MouseButton::Left) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	NUIE::Rect minusButtonRect = uiNode.GetSpecialRect (env, leftButtonId);
	NUIE::Rect plusButtonRect = uiNode.GetSpecialRect (env, rightButtonId);

	if (minusButtonRect.Contains (position)) {
		commandInterface.RunUndoableCommand ([&] () {
			clickHandler.LeftButtonClicked ();
		});
		return NUIE::EventHandlerResult::EventHandled;
	} else if (plusButtonRect.Contains (position)) {
		commandInterface.RunUndoableCommand ([&] () {
			clickHandler.RightButtonClicked ();
		});
		return NUIE::EventHandlerResult::EventHandled;
	}
	return NUIE::EventHandlerResult::EventNotHandled;
}

NUIE::EventHandlerResult HandleMouseClickOnSwitchLayout (	const NUIE::UINode& uiNode,
															const std::string& switchButtonId,
															NUIE::NodeUIEnvironment& env,
															NUIE::MouseButton mouseButton,
															const NUIE::Point& position,
															NUIE::UINodeCommandInterface& commandInterface,
															SwitchClickHandler& clickHandler)
{
	if (mouseButton != NUIE::MouseButton::Left) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	NUIE::Rect switchRect = uiNode.GetSpecialRect (env, switchButtonId);

	if (switchRect.Contains (position)) {
		commandInterface.RunUndoableCommand ([&] () {
			clickHandler.SwitchClicked ();
		});
		return NUIE::EventHandlerResult::EventHandled;
	}
	return NUIE::EventHandlerResult::EventNotHandled;
}

}
