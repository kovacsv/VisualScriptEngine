#include "BI_UINodePanels.hpp"
#include "NUIE_SkinParams.hpp"
#include <algorithm>

namespace BI
{

SlotRectCollection::SlotRectCollection ()
{

}

void SlotRectCollection::AddSlotSize (const NE::SlotId& slotId, const NUIE::Size& size)
{
	slotSizes.push_back (std::pair<NE::SlotId, NUIE::Size> (slotId, size));
}

double SlotRectCollection::GetWidth () const
{
	double width = 0.0;
	for (const auto& it : slotSizes) {
		width = std::max (width, it.second.GetWidth ());
	}
	return width;
}

double SlotRectCollection::GetHeight (double padding) const
{
	double height = 0.0;
	size_t count = slotSizes.size ();
	size_t current = 0;
	for (const auto& it : slotSizes) {
		height += it.second.GetHeight ();
		if (current < count - 1) {
			height += padding;
		}
		current++;
	}
	return height;
}

void SlotRectCollection::Enumerate (RefPointMode refPointMode, const NUIE::Point& refPoint, double padding, const std::function<void (const NE::SlotId&, const NUIE::Rect&)>& processor) const
{
	NUIE::Point currentPosition = refPoint;
	double width = GetWidth ();
	for (const auto& it : slotSizes) {
		NUIE::Size slotSize = it.second;
		slotSize.SetWidth (width);
		NUIE::Rect rect;
		if (refPointMode == RefPointMode::TopLeft) {
			rect = NUIE::Rect::FromPositionAndSize (currentPosition, slotSize);
		} else if (refPointMode == RefPointMode::TopRight) {
			NUIE::Point leftPosition = currentPosition - NUIE::Point (slotSize.GetWidth (), 0.0);
			rect = NUIE::Rect::FromPositionAndSize (leftPosition, slotSize);
		} else {
			DBGBREAK ();
		}
		processor (it.first, rect);
		currentPosition = currentPosition + NUIE::Point (0.0, slotSize.GetHeight () + padding);
	}
}

NodeUITextPanelBase::NodeUITextPanelBase (const std::wstring& nodeText) :
	NodePanel (),
	nodeText (nodeText)
{
	
}

NUIE::Size NodeUITextPanelBase::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	NUIE::Size minSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), nodeText);
	minSize = minSize.Grow (2.0 * skinParams.GetNodePadding (), 2.0 * skinParams.GetNodePadding ());
	return minSize;
}

void NodeUITextPanelBase::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, GetBackgroundColor (env))));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (rect, skinParams.GetNodeTextFont (), nodeText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, GetTextColor (env))));
}

NodeUITextPanel::NodeUITextPanel (const std::wstring& nodeText) :
	NodeUITextPanelBase (nodeText)
{
	
}

NUIE::Color NodeUITextPanel::GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeContentTextColor ();
}

NUIE::Color NodeUITextPanel::GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeContentBackgroundColor ();
}

NodeUIHeaderPanel::NodeUIHeaderPanel (const std::wstring& headerText) :
	NodeUITextPanelBase (headerText)
{
	
}

NUIE::Color NodeUIHeaderPanel::GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeHeaderTextColor ();
}

NUIE::Color NodeUIHeaderPanel::GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeHeaderBackgroundColor ();
}

NodeUIStatusHeaderPanel::NodeUIStatusHeaderPanel (const std::wstring& headerText, NodeStatus nodeStatus) :
	NodePanel (),
	headerText (headerText),
	nodeStatus (nodeStatus)
{
	
}

NUIE::Size NodeUIStatusHeaderPanel::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	NUIE::Size minSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), headerText);
	minSize = minSize.Grow (2.0 * skinParams.GetNodePadding (), 2.0 * skinParams.GetNodePadding ());
	minSize = minSize.Grow (StatusRectSize * 2, 0);
	return minSize;
}

void NodeUIStatusHeaderPanel::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, skinParams.GetNodeHeaderBackgroundColor ())));
	NUIE::Rect textRect = NUIE::Rect::FromPositionAndSize (NUIE::Point (rect.GetLeft () + StatusRectSize * 2, rect.GetTop ()), NUIE::Size (rect.GetWidth () - StatusRectSize * 2, rect.GetHeight ()));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, skinParams.GetNodeTextFont (), headerText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, skinParams.GetNodeHeaderTextColor ())));
	NUIE::Rect statusRect = NUIE::Rect::FromPositionAndSize (NUIE::Point (rect.GetLeft () + StatusRectSize, rect.GetTop () + StatusRectSize), NUIE::Size (StatusRectSize, rect.GetHeight () - StatusRectSize * 2));
	NUIE::Color statusColor = (nodeStatus == NodeStatus::HasValue ? skinParams.GetHasValueStatusColor () : skinParams.GetHasNoValueStatusColor ());
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (statusRect, statusColor)));
}

NodeUIMultiLineTextPanel::NodeUIMultiLineTextPanel (const std::vector<std::wstring>& nodeTexts, NUIE::NodeUIDrawingEnvironment& env, size_t allTextCount, size_t textsPerPage) :
	NodePanel (),
	nodeTexts (nodeTexts),
	maxTextSize (0.0, 0.0),
	allTextCount (allTextCount),
	textsPerPage (textsPerPage)
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	for (const std::wstring& nodeText : nodeTexts) {
		NUIE::Size textSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), nodeText);
		textSize = textSize.Grow (2.0 * skinParams.GetNodePadding (), skinParams.GetNodePadding ());
		maxTextSize.SetWidth (std::max (maxTextSize.GetWidth (), textSize.GetWidth ()));
		maxTextSize.SetHeight (std::max (maxTextSize.GetHeight (), textSize.GetHeight ()));
	}
}

NUIE::Size NodeUIMultiLineTextPanel::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	double minHeight = maxTextSize.GetHeight () * textsPerPage;
	if (allTextCount <= textsPerPage) {
		minHeight = maxTextSize.GetHeight () * std::min (nodeTexts.size (), textsPerPage);
	}

	NUIE::Size minSize (maxTextSize.GetWidth (), minHeight);
	return minSize.Grow (2.0 * nodePadding, 2.0 * nodePadding);
}

void NodeUIMultiLineTextPanel::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, GetBackgroundColor (env))));
	
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	double xOffset = nodePadding;
	double yOffset = nodePadding;
	double textRectWidth = std::max (maxTextSize.GetWidth (), rect.GetWidth () - 2.0 * nodePadding);
	for (const std::wstring& nodeText : nodeTexts) {
		NUIE::Point textRectPosition (rect.GetLeft () + xOffset, rect.GetTop () + yOffset);
		NUIE::Size textRectSize (textRectWidth, maxTextSize.GetHeight ());
		NUIE::Rect textRect = NUIE::Rect::FromPositionAndSize (textRectPosition, textRectSize);
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, skinParams.GetNodeTextFont (), nodeText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, GetTextColor (env))));
		yOffset += maxTextSize.GetHeight ();
	}
}

NUIE::Color NodeUIMultiLineTextPanel::GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	return env.GetSkinParams ().GetTextPanelTextColor ();
}

NUIE::Color NodeUIMultiLineTextPanel::GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	return env.GetSkinParams ().GetTextPanelBackgroundColor ();
}

NodeUISlotPanel::NodeUISlotPanel (const NUIE::UINode& node, NUIE::NodeUIDrawingEnvironment& env) :
	NodePanel (),
	node (node)
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();

	node.EnumerateUIInputSlots ([&] (const NUIE::UIInputSlotConstPtr& slot) {
		NUIE::Size slotRectSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), slot->GetName ());
		slotRectSize = slotRectSize.Grow (2.0 * skinParams.GetNodePadding (), skinParams.GetNodePadding ());
		inputSlots.AddSlotSize (slot->GetId (), slotRectSize);
		return true;
	});

	node.EnumerateUIOutputSlots ([&] (const NUIE::UIOutputSlotConstPtr& slot) {
		NUIE::Size slotRectSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), slot->GetName ());
		slotRectSize = slotRectSize.Grow (2.0 * skinParams.GetNodePadding (), skinParams.GetNodePadding ());
		outputSlots.AddSlotSize (slot->GetId (), slotRectSize);
		return true;
	});
}

NUIE::Size NodeUISlotPanel::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	double inputSlotsWidth = inputSlots.GetWidth ();
	double outputSlotsWidth = outputSlots.GetWidth ();
	double inputSlotsHeight = inputSlots.GetHeight (nodePadding);
	double outputSlotsHeight = outputSlots.GetHeight (nodePadding);
	NUIE::Size minSize (inputSlotsWidth + outputSlotsWidth, std::max (inputSlotsHeight, outputSlotsHeight));
	minSize = minSize.Grow (2.0 * nodePadding, 2.0 * nodePadding);
	return minSize;
}

void NodeUISlotPanel::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, skinParams.GetNodeContentBackgroundColor ())));

	NUIE::Point inputSlotsStartPoint = rect.GetTopLeft () + NUIE::Point (0.0, nodePadding);
	inputSlots.Enumerate (SlotRectCollection::RefPointMode::TopLeft, inputSlotsStartPoint, nodePadding, [&] (const NE::SlotId& slotId, const NUIE::Rect& slotRect) {
		NUIE::UIInputSlotConstPtr uiSlot = node.GetUIInputSlot (slotId);
		NUIE::Rect textRect = slotRect.Expand (NUIE::Size (2.0 * -nodePadding, -nodePadding));
		drawingImage.AddInputSlotConnPosition (slotId, slotRect.GetLeftCenter ());
		drawingImage.AddInputSlotRect (slotId, slotRect);
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (slotRect, skinParams.GetSlotTextBackgroundColor ())));
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, skinParams.GetNodeTextFont (), uiSlot->GetName (), NUIE::HorizontalAnchor::Left, NUIE::VerticalAnchor::Center, skinParams.GetSlotTextColor ())));
	});

	NUIE::Point outputSlotsStartPoint = rect.GetTopRight () + NUIE::Point (0.0, nodePadding);
	outputSlots.Enumerate (SlotRectCollection::RefPointMode::TopRight, outputSlotsStartPoint, nodePadding, [&] (const NE::SlotId& slotId, const NUIE::Rect& slotRect) {
		NUIE::UIOutputSlotConstPtr uiSlot = node.GetUIOutputSlot (slotId);
		NUIE::Rect textRect = slotRect.Expand (NUIE::Size (2.0 * -nodePadding, -nodePadding));
		drawingImage.AddOutputSlotConnPosition (slotId, slotRect.GetRightCenter ());
		drawingImage.AddOutputSlotRect (slotId, slotRect);
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (slotRect, skinParams.GetSlotTextBackgroundColor ())));
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, skinParams.GetNodeTextFont (), uiSlot->GetName (), NUIE::HorizontalAnchor::Right, NUIE::VerticalAnchor::Center, skinParams.GetSlotTextColor ())));
	});
}

NodeUILeftRightButtonsPanel::NodeUILeftRightButtonsPanel (	const std::string& leftButtonId,
															const std::wstring& leftButtonText,
															const std::string& rightButtonId,
															const std::wstring& rightButtonText,
															const std::wstring& panelText,
															NUIE::NodeUIDrawingEnvironment& env) :
	NodePanel (),
	leftButtonId (leftButtonId),
	leftButtonText (leftButtonText),
	rightButtonId (rightButtonId),
	rightButtonText (rightButtonText),
	panelText (panelText)
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	leftButtonSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), leftButtonText).Grow (2.0 * nodePadding, nodePadding);
	panelTextSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), panelText).Grow (2.0 * nodePadding, nodePadding);
	rightButtonSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), rightButtonText).Grow (2.0 * nodePadding, nodePadding);
}

NUIE::Size NodeUILeftRightButtonsPanel::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	NUIE::Size minSize;
	minSize.SetWidth (leftButtonSize.GetWidth () + panelTextSize.GetWidth () + rightButtonSize.GetWidth ());
	minSize.SetHeight (std::max (std::max (leftButtonSize.GetHeight (), rightButtonSize.GetHeight ()), panelTextSize.GetHeight ()));
	minSize = minSize.Grow (2.0 * nodePadding, 2.0 * nodePadding);
	return minSize;
}

void NodeUILeftRightButtonsPanel::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	
	const NUIE::Color backgroundColor = skinParams.GetNodeContentBackgroundColor ();
	const NUIE::Color textColor = skinParams.GetNodeContentTextColor ();

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, backgroundColor)));

	NUIE::Rect leftButtonRect = NUIE::Rect::FromPositionAndSize (rect.GetTopLeft () + NUIE::Point (nodePadding, nodePadding), leftButtonSize);
	NUIE::Rect rightButtonRect = NUIE::Rect::FromPositionAndSize (rect.GetTopRight () - NUIE::Point (rightButtonSize.GetWidth () + nodePadding, -nodePadding), rightButtonSize);
	NUIE::Rect textRect = NUIE::Rect::FromPositionAndSize (leftButtonRect.GetTopRight (), NUIE::Size (rightButtonRect.GetLeft () - leftButtonRect.GetRight (), panelTextSize.GetHeight ()));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, skinParams.GetNodeTextFont (), panelText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, textColor)));

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (leftButtonRect, skinParams.GetButtonBackgroundColor ())));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (leftButtonRect, skinParams.GetNodeTextFont (), leftButtonText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, textColor)));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingRect (leftButtonRect, skinParams.GetButtonBorderPen ())));
	drawingImage.AddSpecialRect (leftButtonId, leftButtonRect);

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rightButtonRect, skinParams.GetButtonBackgroundColor ())));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (rightButtonRect, skinParams.GetNodeTextFont (), rightButtonText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, textColor)));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingRect (rightButtonRect, skinParams.GetButtonBorderPen ())));
	drawingImage.AddSpecialRect (rightButtonId, rightButtonRect);
}

NodeUISwitchPanel::NodeUISwitchPanel (	const std::string& switchRectId,
										const std::wstring& leftSwitchText,
										const std::wstring& rightSwitchText,
										short selectedIndex,
										NUIE::NodeUIDrawingEnvironment& env) :
	switchRectId (switchRectId),
	leftSwitchText (leftSwitchText),
	rightSwitchText (rightSwitchText),
	selectedIndex (selectedIndex)
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	leftSwitchSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), leftSwitchText).Grow (2.0 * nodePadding, nodePadding);
	rightSwitchSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), rightSwitchText).Grow (2.0 * nodePadding, nodePadding);
}

NUIE::Size NodeUISwitchPanel::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	NUIE::Size minSize;
	minSize.SetWidth (leftSwitchSize.GetWidth () + rightSwitchSize.GetWidth ());
	minSize.SetHeight (std::max (leftSwitchSize.GetHeight (), rightSwitchSize.GetHeight ()));
	minSize = minSize.Grow (2.0 * nodePadding, 2.0 * nodePadding);
	return minSize;
}

void NodeUISwitchPanel::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	const NUIE::Color backgroundColor = skinParams.GetNodeContentBackgroundColor ();
	const NUIE::Color textColor = skinParams.GetNodeContentTextColor ();

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, backgroundColor)));

	NUIE::Size switchSize (
		leftSwitchSize.GetWidth () + rightSwitchSize.GetWidth (),
		std::max (leftSwitchSize.GetHeight (), rightSwitchSize.GetHeight ())
	);
	NUIE::Rect switchRect = NUIE::Rect::FromCenterAndSize (rect.GetCenter (), switchSize);
	NUIE::Rect leftSwitchRect = NUIE::Rect::FromPositionAndSize (switchRect.GetTopLeft (), leftSwitchSize);
	NUIE::Rect rightSwitchRect = NUIE::Rect::FromPositionAndSize (leftSwitchRect.GetTopRight (), rightSwitchSize);

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (switchRect, skinParams.GetButtonBackgroundColor ())));
	if (selectedIndex == 0) {
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (leftSwitchRect, skinParams.GetSelectedButtonBackgroundColor ())));
	} else if (selectedIndex == 1) {
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rightSwitchRect, skinParams.GetSelectedButtonBackgroundColor ())));
	}

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (leftSwitchRect, skinParams.GetNodeTextFont (), leftSwitchText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, textColor)));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (rightSwitchRect, skinParams.GetNodeTextFont (), rightSwitchText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, textColor)));
	
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingRect (switchRect, skinParams.GetButtonBorderPen ())));
	drawingImage.AddSpecialRect (switchRectId, switchRect);
}

}
