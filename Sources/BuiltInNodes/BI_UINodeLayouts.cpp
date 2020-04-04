#include "BI_UINodeLayouts.hpp"
#include "BI_UINodePanels.hpp"
#include "NE_Localization.hpp"

namespace BI
{

HeaderBasedLayout::HeaderBasedLayout () :
	UINodeLayout ()
{
}

void HeaderBasedLayout::AddPanels (	const BasicUINode& uiNode,
									NUIE::NodeUIDrawingEnvironment& env,
									NUIE::NodePanelDrawer& drawer) const
{
	NodeUIHeaderPanel::NodeStatus nodeStatus = NodeUIHeaderPanel::NodeStatus::HasNoValue;
	if (uiNode.HasCalculatedValue () && uiNode.GetCalculatedValue () != nullptr) {
		nodeStatus = NodeUIHeaderPanel::NodeStatus::HasValue;
	}
	if (uiNode.HasIconId ()) {
		drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIIconHeaderPanel (uiNode.GetNodeName (), nodeStatus, uiNode.GetIconId (), env)));
	} else {
		drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIHeaderPanel (uiNode.GetNodeName (), nodeStatus)));
	}
}

HeaderWithSlotsLayout::HeaderWithSlotsLayout () :
	HeaderBasedLayout ()
{
}

void HeaderWithSlotsLayout::AddPanels (	const BasicUINode& uiNode,
										NUIE::NodeUIDrawingEnvironment& env,
										NUIE::NodePanelDrawer& drawer) const
{
	HeaderBasedLayout::AddPanels (uiNode, env, drawer);
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
}

NUIE::EventHandlerResult HeaderWithSlotsLayout::HandleMouseClick (	BasicUINode&,
																	NUIE::NodeUIEnvironment&,
																	const NUIE::ModifierKeys&,
																	NUIE::MouseButton,
																	const NUIE::Point&,
																	NUIE::UINodeCommandInterface&) const
{
	return NUIE::EventHandlerResult::EventNotHandled;
}

HeaderWithSlotsAndTextLayout::HeaderWithSlotsAndTextLayout () :
	HeaderBasedLayout ()
{
}

void HeaderWithSlotsAndTextLayout::AddPanels (	const BasicUINode& uiNode,
												NUIE::NodeUIDrawingEnvironment& env,
												NUIE::NodePanelDrawer& drawer) const
{
	HeaderBasedLayout::AddPanels (uiNode, env, drawer);
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUITextPanel (GetText (uiNode, env.GetStringSettings ()))));
}

NUIE::EventHandlerResult HeaderWithSlotsAndTextLayout::HandleMouseClick (	BasicUINode&,
																			NUIE::NodeUIEnvironment&,
																			const NUIE::ModifierKeys&,
																			NUIE::MouseButton,
																			const NUIE::Point&,
																			NUIE::UINodeCommandInterface&) const
{
	return NUIE::EventHandlerResult::EventNotHandled;
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
	HeaderBasedLayout (),
	leftButtonId (leftButtonId),
	leftButtonText (leftButtonText),
	rightButtonId (rightButtonId),
	rightButtonText (rightButtonText)
{
}

void HeaderWithSlotsAndButtonsLayout::AddPanels (	const BasicUINode& uiNode,
													NUIE::NodeUIDrawingEnvironment& env,
													NUIE::NodePanelDrawer& drawer) const
{
	std::wstring nodeText = GetMiddleText (uiNode, env.GetStringSettings ());
	HeaderBasedLayout::AddPanels (uiNode, env, drawer);
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUILeftRightButtonsPanel (leftButtonId, leftButtonText, rightButtonId, rightButtonText, nodeText, env)));
}

NUIE::EventHandlerResult HeaderWithSlotsAndButtonsLayout::HandleMouseClick (BasicUINode& uiNode,
																			NUIE::NodeUIEnvironment& env,
																			const NUIE::ModifierKeys&,
																			NUIE::MouseButton mouseButton,
																			const NUIE::Point& position,
																			NUIE::UINodeCommandInterface& commandInterface) const
{
	if (mouseButton != NUIE::MouseButton::Left) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	std::shared_ptr<ClickHandler> clickHandler = GetClickHandler (uiNode);
	NUIE::Rect minusButtonRect = uiNode.GetSpecialRect (env, leftButtonId);
	NUIE::Rect plusButtonRect = uiNode.GetSpecialRect (env, rightButtonId);

	if (minusButtonRect.Contains (position)) {
		commandInterface.RunUndoableCommand ([&] () {
			clickHandler->LeftButtonClicked ();
		});
		return NUIE::EventHandlerResult::EventHandled;
	} else if (plusButtonRect.Contains (position)) {
		commandInterface.RunUndoableCommand ([&] () {
			clickHandler->RightButtonClicked ();
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
	HeaderBasedLayout (),
	switchButtonId (switchButtonId),
	firstSwitchText (firstSwitchText),
	secondSwitchText (secondSwitchText)
{
}

void HeaderWithSlotsAndSwitchLayout::AddPanels (const BasicUINode& uiNode,
												NUIE::NodeUIDrawingEnvironment& env,
												NUIE::NodePanelDrawer& drawer) const
{
	HeaderBasedLayout::AddPanels (uiNode, env, drawer);
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	SelectedItem selectedItem = GetSelectedItem (uiNode);
	std::wstring buttonText = (selectedItem == SelectedItem::First ? firstSwitchText : secondSwitchText);
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIButtonPanel (switchButtonId, buttonText, env)));
}

NUIE::EventHandlerResult HeaderWithSlotsAndSwitchLayout::HandleMouseClick (	BasicUINode& uiNode,
																			NUIE::NodeUIEnvironment& env,
																			const NUIE::ModifierKeys&,
																			NUIE::MouseButton mouseButton,
																			const NUIE::Point& position,
																			NUIE::UINodeCommandInterface& commandInterface) const
{
	if (mouseButton != NUIE::MouseButton::Left) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	std::shared_ptr<ClickHandler> clickHandler = GetClickHandler (uiNode);
	NUIE::Rect switchRect = uiNode.GetSpecialRect (env, switchButtonId);

	if (switchRect.Contains (position)) {
		commandInterface.RunUndoableCommand ([&] () {
			clickHandler->SwitchClicked ();
		});
		return NUIE::EventHandlerResult::EventHandled;
	}
	return NUIE::EventHandlerResult::EventNotHandled;
}

}
