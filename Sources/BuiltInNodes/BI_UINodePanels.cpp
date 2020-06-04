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

NodeUIHeaderPanel::NodeUIHeaderPanel (const std::wstring& headerText, NodeStatus nodeStatus) :
	NUIE::NodePanel (),
	headerText (headerText),
	nodeStatus (nodeStatus)
{

}

NUIE::Size NodeUIHeaderPanel::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	NUIE::Size minSize = env.GetDrawingContext ().MeasureText (GetTextFont (env), headerText);
	minSize = minSize.Grow (2.0 * skinParams.GetNodePadding (), 2.0 * skinParams.GetNodePadding ());
	return minSize;
}

void NodeUIHeaderPanel::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, GetBackgroundColor (env))));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (rect, GetTextFont (env), headerText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, GetTextColor (env))), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
}

const NUIE::Font& NodeUIHeaderPanel::GetTextFont (NUIE::NodeUIDrawingEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeHeaderTextFont ();
}

const NUIE::Color& NodeUIHeaderPanel::GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	if (nodeStatus == NodeUIHeaderPanel::NodeStatus::HasValue) {
		return env.GetSkinParams ().GetNodeHeaderTextColor ();
	} else if (nodeStatus == NodeUIHeaderPanel::NodeStatus::HasNoValue) {
		return env.GetSkinParams ().GetNodeHeaderErrorTextColor ();
	}
	DBGBREAK ();
	return env.GetSkinParams ().GetNodeHeaderTextColor ();
}

const NUIE::Color& NodeUIHeaderPanel::GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	if (nodeStatus == NodeUIHeaderPanel::NodeStatus::HasValue) {
		return env.GetSkinParams ().GetNodeHeaderBackgroundColor ();
	} else if (nodeStatus == NodeUIHeaderPanel::NodeStatus::HasNoValue) {
		return env.GetSkinParams ().GetNodeHeaderErrorBackgroundColor ();
	}
	DBGBREAK ();
	return env.GetSkinParams ().GetNodeHeaderBackgroundColor ();
}

NodeUIIconHeaderPanel::NodeUIIconHeaderPanel (const std::wstring& headerText, NodeStatus nodeStatus, const NUIE::IconId& iconId, NUIE::NodeUIDrawingEnvironment& env) :
	NodeUIHeaderPanel (headerText, nodeStatus),
	iconId (iconId),
	textSize (0.0, 0.0),
	iconSize (0.0),
	minWidth (0.0)
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();

	textSize = env.GetDrawingContext ().MeasureText (GetTextFont (env), headerText);
	iconSize = textSize.GetHeight ();
	minWidth = iconSize + nodePadding + textSize.GetWidth ();
}

NUIE::Size NodeUIIconHeaderPanel::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();

	NUIE::Size minSize (minWidth, std::max (textSize.GetHeight (), iconSize));
	minSize = minSize.Grow (2.0 * nodePadding, 2.0 * nodePadding);
	return minSize;
}

void NodeUIIconHeaderPanel::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();

	double centerOffset = (rect.GetWidth () - minWidth) / 2.0;
	NUIE::Rect iconRect = NUIE::Rect::FromPositionAndSize (NUIE::Point (rect.GetLeft () + centerOffset, rect.GetTop () + nodePadding), NUIE::Size (iconSize, iconSize));
	NUIE::Rect textRect = NUIE::Rect::FromPositionAndSize (NUIE::Point (rect.GetLeft () + centerOffset + iconSize + nodePadding, rect.GetTop ()), NUIE::Size (textSize.GetWidth (), rect.GetHeight ()));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, GetBackgroundColor (env))));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingIcon (iconRect, iconId)), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, GetTextFont (env), headerText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, GetTextColor (env))), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
}

NodeUITextPanel::NodeUITextPanel (const std::wstring& text) :
	NUIE::NodePanel (),
	text (text)
{

}

NUIE::Size NodeUITextPanel::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	NUIE::Size minSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeContentTextFont (), text);
	minSize = minSize.Grow (2.0 * skinParams.GetNodePadding (), 2.0 * skinParams.GetNodePadding ());
	return minSize;
}

void NodeUITextPanel::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, skinParams.GetTextPanelBackgroundColor ())));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (rect, skinParams.GetNodeContentTextFont (), text, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, skinParams.GetNodeContentTextColor ())), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
}

NodeUIMultiLineTextPanel::NodeUIMultiLineTextPanel (const std::vector<std::wstring>& nodeTexts, size_t allTextCount, size_t textsPerPage, NUIE::NodeUIDrawingEnvironment& env) :
	NodePanel (),
	nodeTexts (nodeTexts),
	maxTextSize (0.0, 0.0),
	allTextCount (allTextCount),
	textsPerPage (textsPerPage)
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	for (const std::wstring& nodeText : nodeTexts) {
		NUIE::Size textSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeContentTextFont (), nodeText);
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
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, skinParams.GetNodeContentTextFont (), nodeText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, GetTextColor (env))), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
		yOffset += maxTextSize.GetHeight ();
	}
}

const NUIE::Color& NodeUIMultiLineTextPanel::GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	return env.GetSkinParams ().GetTextPanelTextColor ();
}

const NUIE::Color& NodeUIMultiLineTextPanel::GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const
{
	return env.GetSkinParams ().GetTextPanelBackgroundColor ();
}

NodeUISlotPanel::NodeUISlotPanel (const NUIE::UINode& node, NUIE::NodeUIDrawingEnvironment& env) :
	NodePanel (),
	node (node)
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();

	node.EnumerateUIInputSlots ([&] (const NUIE::UIInputSlotConstPtr& slot) {
		NUIE::Size slotRectSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeContentTextFont (), slot->GetName ().GetLocalized ());
		slotRectSize = slotRectSize.Grow (2.0 * skinParams.GetNodePadding (), skinParams.GetNodePadding ());
		inputSlots.AddSlotSize (slot->GetId (), slotRectSize);
		return true;
	});

	node.EnumerateUIOutputSlots ([&] (const NUIE::UIOutputSlotConstPtr& slot) {
		NUIE::Size slotRectSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeContentTextFont (), slot->GetName ().GetLocalized ());
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
		if (skinParams.NeedToDrawSlotCircles ()) {
			NUIE::Rect connCircleRect = NUIE::Rect::FromCenterAndSize (slotRect.GetLeftCenter (), skinParams.GetSlotCircleSize ());
			drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillEllipse (connCircleRect, skinParams.GetSlotTextBackgroundColor ())), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
			drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingEllipse (connCircleRect, skinParams.GetConnectionLinePen ())), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
		}
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (slotRect, skinParams.GetSlotTextBackgroundColor ())));
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, skinParams.GetNodeContentTextFont (), uiSlot->GetName ().GetLocalized (), NUIE::HorizontalAnchor::Left, NUIE::VerticalAnchor::Center, skinParams.GetSlotTextColor ())), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
	});

	NUIE::Point outputSlotsStartPoint = rect.GetTopRight () + NUIE::Point (0.0, nodePadding);
	outputSlots.Enumerate (SlotRectCollection::RefPointMode::TopRight, outputSlotsStartPoint, nodePadding, [&] (const NE::SlotId& slotId, const NUIE::Rect& slotRect) {
		NUIE::UIOutputSlotConstPtr uiSlot = node.GetUIOutputSlot (slotId);
		NUIE::Rect textRect = slotRect.Expand (NUIE::Size (2.0 * -nodePadding, -nodePadding));
		drawingImage.AddOutputSlotConnPosition (slotId, slotRect.GetRightCenter ());
		drawingImage.AddOutputSlotRect (slotId, slotRect);
		if (skinParams.NeedToDrawSlotCircles ()) {
			NUIE::Rect connCircleRect = NUIE::Rect::FromCenterAndSize (slotRect.GetRightCenter (), skinParams.GetSlotCircleSize ());
			drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillEllipse (connCircleRect, skinParams.GetSlotTextBackgroundColor ())), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
			drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingEllipse (connCircleRect, skinParams.GetConnectionLinePen ())), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
		}
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (slotRect, skinParams.GetSlotTextBackgroundColor ())));
		drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, skinParams.GetNodeContentTextFont (), uiSlot->GetName ().GetLocalized (), NUIE::HorizontalAnchor::Right, NUIE::VerticalAnchor::Center, skinParams.GetSlotTextColor ())), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
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
	leftButtonSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeContentTextFont (), leftButtonText).Grow (2.0 * nodePadding, nodePadding);
	panelTextSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeContentTextFont (), panelText).Grow (2.0 * nodePadding, nodePadding);
	rightButtonSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeContentTextFont (), rightButtonText).Grow (2.0 * nodePadding, nodePadding);
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
	
	const NUIE::Color& backgroundColor = skinParams.GetNodeContentBackgroundColor ();
	const NUIE::Color& textColor = skinParams.GetNodeContentTextColor ();

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, backgroundColor)));

	NUIE::Rect leftButtonRect = NUIE::Rect::FromPositionAndSize (rect.GetTopLeft () + NUIE::Point (nodePadding, nodePadding), leftButtonSize);
	NUIE::Rect rightButtonRect = NUIE::Rect::FromPositionAndSize (rect.GetTopRight () - NUIE::Point (rightButtonSize.GetWidth () + nodePadding, -nodePadding), rightButtonSize);
	NUIE::Rect textRect = NUIE::Rect::FromPositionAndSize (leftButtonRect.GetTopRight (), NUIE::Size (rightButtonRect.GetLeft () - leftButtonRect.GetRight (), panelTextSize.GetHeight ()));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (textRect, skinParams.GetNodeContentTextFont (), panelText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, textColor)), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (leftButtonRect, skinParams.GetButtonBackgroundColor ())));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (leftButtonRect, skinParams.GetNodeContentTextFont (), leftButtonText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, textColor)), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingRect (leftButtonRect, skinParams.GetButtonBorderPen ())));
	drawingImage.AddSpecialRect (leftButtonId, leftButtonRect);

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rightButtonRect, skinParams.GetButtonBackgroundColor ())));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (rightButtonRect, skinParams.GetNodeContentTextFont (), rightButtonText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, textColor)), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingRect (rightButtonRect, skinParams.GetButtonBorderPen ())));
	drawingImage.AddSpecialRect (rightButtonId, rightButtonRect);
}

NodeUIButtonPanel::NodeUIButtonPanel (	const std::string& buttonRectId,
										const std::wstring& buttonText,
										NUIE::NodeUIDrawingEnvironment& env) :
	buttonRectId (buttonRectId),
	buttonText (buttonText)
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	buttonSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeContentTextFont (), buttonText).Grow (2.0 * nodePadding, nodePadding);
}

NUIE::Size NodeUIButtonPanel::GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	NUIE::Size minSize = buttonSize;
	minSize = minSize.Grow (2.0 * nodePadding, 2.0 * nodePadding);
	return minSize;
}

void NodeUIButtonPanel::Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const
{
	const NUIE::SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	const NUIE::Color& backgroundColor = skinParams.GetNodeContentBackgroundColor ();
	const NUIE::Color& textColor = skinParams.GetNodeContentTextColor ();

	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (rect, backgroundColor)));

	NUIE::Rect buttonRect = NUIE::Rect::FromCenterAndSize (rect.GetCenter (), rect.GetSize ().Grow (-2.0 * nodePadding, -2.0 * nodePadding));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillRect (buttonRect, skinParams.GetButtonBackgroundColor ())));
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingText (buttonRect, skinParams.GetNodeContentTextFont (), buttonText, NUIE::HorizontalAnchor::Center, NUIE::VerticalAnchor::Center, textColor)), NUIE::DrawingContext::ItemPreviewMode::HideInPreview);
	drawingImage.AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingRect (buttonRect, skinParams.GetButtonBorderPen ())));
	drawingImage.AddSpecialRect (buttonRectId, buttonRect);
}

}
