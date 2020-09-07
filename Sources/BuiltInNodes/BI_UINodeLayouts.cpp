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
	std::wstring nodeName = uiNode.GetName ().GetLocalized ();
	if (uiNode.HasIconId () && env.GetDrawingContext ().CanDrawIcon ()) {
		drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIIconHeaderPanel (nodeName, nodeStatus, uiNode.GetIconId (), env)));
	} else {
		drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIHeaderPanel (nodeName, nodeStatus)));
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
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUITextPanel (GetText (uiNode, env.GetStringConverter ()))));
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
	std::wstring nodeText = GetMiddleText (uiNode, env.GetStringConverter ());
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

HeaderWithSlotsAndMultilineTextLayout::HeaderWithSlotsAndMultilineTextLayout (	const std::string& leftButtonId,
																				const std::wstring& leftButtonText,
																				const std::string& rightButtonId,
																				const std::wstring& rightButtonText) :
	HeaderBasedLayout (),
	leftButtonId (leftButtonId),
	leftButtonText (leftButtonText),
	rightButtonId (rightButtonId),
	rightButtonText (rightButtonText),
	pageCount (0),
	currentPage (1)
{
}

void HeaderWithSlotsAndMultilineTextLayout::AddPanels (	const BasicUINode& uiNode,
														NUIE::NodeUIDrawingEnvironment& env,
														NUIE::NodePanelDrawer& drawer) const
{
	std::vector<std::wstring> texts;
	size_t textsPerPage = 0;
	GetTextInfo (uiNode, env.GetStringConverter (), texts, textsPerPage);

	size_t textCount = texts.size ();
	pageCount = textCount / textsPerPage;
	if (textCount % textsPerPage != 0) {
		pageCount = pageCount + 1;
	}
	if (currentPage > pageCount) {
		currentPage = pageCount;
	}

	std::vector<std::wstring> visibleTexts;
	if (!texts.empty ()) {
		textCount = texts.size ();
		for (size_t i = 0; i < textsPerPage; ++i) {
			size_t textIndex = (currentPage - 1) * textsPerPage + i;
			if (textIndex < texts.size ()) {
				visibleTexts.push_back (texts[textIndex]);
			}
		}
	} else {
		textCount = 1;
		pageCount = 1;
		currentPage = 1;
		visibleTexts.push_back (NE::LocalizeString (L"<empty>"));
	}

	HeaderBasedLayout::AddPanels (uiNode, env, drawer);
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (uiNode, env)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIMultiLineTextPanel (visibleTexts, textCount, textsPerPage, env)));
	if (textCount > textsPerPage) {
		std::wstring formatString = NE::LocalizeString (L"%d / %d (%d)");
		std::wstring panelText = NE::FormatString (formatString, currentPage, pageCount, textCount);
		drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUILeftRightButtonsPanel (leftButtonId, leftButtonText, rightButtonId, rightButtonText, panelText, env)));
	}
}

NUIE::EventHandlerResult HeaderWithSlotsAndMultilineTextLayout::HandleMouseClick (	BasicUINode& uiNode,
																					NUIE::NodeUIEnvironment& env,
																					const NUIE::ModifierKeys&,
																					NUIE::MouseButton mouseButton,
																					const NUIE::Point& position,
																					NUIE::UINodeCommandInterface&) const
{
	if (mouseButton != NUIE::MouseButton::Left) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	if (!uiNode.HasSpecialRect (env, leftButtonId) || !uiNode.HasSpecialRect (env, rightButtonId)) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	NUIE::Rect minusButtonRect = uiNode.GetSpecialRect (env, leftButtonId);
	NUIE::Rect plusButtonRect = uiNode.GetSpecialRect (env, rightButtonId);

	if (minusButtonRect.Contains (position)) {
		currentPage = currentPage - 1;
		if (currentPage == 0) {
			currentPage = pageCount;
		}
		return NUIE::EventHandlerResult::EventHandled;
	} else if (plusButtonRect.Contains (position)) {
		currentPage = currentPage + 1;
		if (currentPage > pageCount) {
			currentPage = 1;
		}
		return NUIE::EventHandlerResult::EventHandled;
	}
	return NUIE::EventHandlerResult::EventNotHandled;
}

}
