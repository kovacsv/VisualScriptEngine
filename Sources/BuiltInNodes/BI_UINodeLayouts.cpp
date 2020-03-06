#include "BI_UINodeLayouts.hpp"
#include "BI_UINodePanels.hpp"
#include "NE_Localization.hpp"
#include "NUIE_UINode.hpp"

namespace BI
{

StatusHeaderWithSlotsLayout::StatusHeaderWithSlotsLayout ()
{
}

void StatusHeaderWithSlotsLayout::Draw (const NUIE::UINode& uiNode,
										NUIE::NodeUIDrawingEnvironment& env,
										NUIE::NodeDrawingImage& drawingImage) const
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

HeaderWithSlotsAndButtonsLayout::ClickHandler::ClickHandler ()
{

}

HeaderWithSlotsAndButtonsLayout::ClickHandler::~ClickHandler ()
{

}

HeaderWithSlotsAndButtonsLayout::HeaderWithSlotsAndButtonsLayout (	const std::string& leftButtonId,
																	const std::wstring& leftButtonText,
																	const std::string& rightButtonId,
																	const std::wstring& rightButtonText) :
	leftButtonId (leftButtonId),
	leftButtonText (leftButtonText),
	rightButtonId (rightButtonId),
	rightButtonText (rightButtonText)
{
}

void HeaderWithSlotsAndButtonsLayout::Draw (const NUIE::UINode& uiNode,
											const std::wstring& middleText,
											NUIE::NodeUIDrawingEnvironment& env,
											NUIE::NodeDrawingImage & drawingImage) const
{
	NUIE::NodePanelDrawer drawer;
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIHeaderPanel (uiNode.GetNodeName ())));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUILeftRightButtonsPanel (leftButtonId, leftButtonText, rightButtonId, rightButtonText, middleText, env)));
	drawer.Draw (env, drawingImage);
}

NUIE::EventHandlerResult HeaderWithSlotsAndButtonsLayout::HandleMouseClick (const NUIE::UINode& uiNode,
																			NUIE::NodeUIEnvironment& env,
																			NUIE::MouseButton mouseButton,
																			const NUIE::Point& position,
																			NUIE::UINodeCommandInterface& commandInterface,
																			ClickHandler& clickHandler)
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

HeaderWithSlotsAndSwitchLayout::ClickHandler::ClickHandler ()
{

}

HeaderWithSlotsAndSwitchLayout::ClickHandler::~ClickHandler ()
{

}

HeaderWithSlotsAndSwitchLayout::HeaderWithSlotsAndSwitchLayout (const std::string& switchButtonId,
																const std::wstring& firstSwitchText,
																const std::wstring& secondSwitchText) :
	switchButtonId (switchButtonId),
	firstSwitchText (firstSwitchText),
	secondSwitchText (secondSwitchText)
{
}

void HeaderWithSlotsAndSwitchLayout::Draw (	const NUIE::UINode& uiNode,
											SelectedItem selectedItem,
											NUIE::NodeUIDrawingEnvironment& env,
											NUIE::NodeDrawingImage & drawingImage) const
{
	NUIE::NodePanelDrawer drawer;
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIHeaderPanel (uiNode.GetNodeName ())));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	std::wstring buttonText = (selectedItem == SelectedItem::First ? firstSwitchText : secondSwitchText);
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIButtonPanel (switchButtonId, buttonText, env)));
	drawer.Draw (env, drawingImage);
}

NUIE::EventHandlerResult HeaderWithSlotsAndSwitchLayout::HandleMouseClick (const NUIE::UINode& uiNode,
																			NUIE::NodeUIEnvironment& env,
																			NUIE::MouseButton mouseButton,
																			const NUIE::Point& position,
																			NUIE::UINodeCommandInterface& commandInterface,
																			ClickHandler& clickHandler)
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
