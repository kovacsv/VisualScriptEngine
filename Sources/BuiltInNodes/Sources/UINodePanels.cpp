#include "UINodePanels.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SkinParams.hpp"
#include <algorithm>

namespace NUIE
{

SlotRectCollection::SlotRectCollection ()
{

}

void SlotRectCollection::AddSlotSize (const NE::SlotId& slotId, const Size& size)
{
	slotSizes.push_back (std::pair<NE::SlotId, Size> (slotId, size));
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

void SlotRectCollection::Enumerate (RefPointMode refPointMode, const Point& refPoint, double padding, const std::function<void (const NE::SlotId&, const Rect&)>& processor) const
{
	Point currentPosition = refPoint;
	double width = GetWidth ();
	for (const auto& it : slotSizes) {
		Size slotSize = it.second;
		slotSize.SetWidth (width);
		Rect rect;
		if (refPointMode == RefPointMode::TopLeft) {
			rect = Rect::FromPositionAndSize (currentPosition, slotSize);
		} else if (refPointMode == RefPointMode::TopRight) {
			Point leftPosition = currentPosition - Point (slotSize.GetWidth (), 0.0);
			rect = Rect::FromPositionAndSize (leftPosition, slotSize);
		} else {
			DBGBREAK ();
		}
		processor (it.first, rect);
		currentPosition = currentPosition + Point (0.0, slotSize.GetHeight () + padding);
	}
}

NodeUITextPanelBase::NodeUITextPanelBase (const std::wstring& nodeText) :
	NodeUIPanel (),
	nodeText (nodeText)
{
	
}

Size NodeUITextPanelBase::GetMinSize (NodeUIEnvironment& env) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	Size minSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), nodeText);
	minSize = minSize.Grow (2.0 * skinParams.GetNodePadding (), 2.0 * skinParams.GetNodePadding ());
	return minSize;
}

void NodeUITextPanelBase::Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (rect, GetBackgroundColor (env))));
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingText (rect, skinParams.GetNodeTextFont (), nodeText, HorizontalAnchor::Center, VerticalAnchor::Center, GetBackgroundColor (env), GetTextColor (env))));
}

NodeUITextPanel::NodeUITextPanel (const std::wstring& nodeText) :
	NodeUITextPanelBase (nodeText)
{
	
}

Color NodeUITextPanel::GetTextColor (NodeUIEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeContentTextColor ();
}

Color NodeUITextPanel::GetBackgroundColor (NodeUIEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeContentBackgroundColor ();
}

NodeUIHeaderPanel::NodeUIHeaderPanel (const std::wstring& headerText) :
	NodeUITextPanelBase (headerText)
{
	
}

Color NodeUIHeaderPanel::GetTextColor (NodeUIEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeHeaderTextColor ();
}

Color NodeUIHeaderPanel::GetBackgroundColor (NodeUIEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeHeaderBackgroundColor ();
}

NodeUIStatusHeaderPanel::NodeUIStatusHeaderPanel (const std::wstring& headerText, UINode::Status nodeStatus) :
	NodeUIPanel (),
	headerText (headerText),
	nodeStatus (nodeStatus)
{
	
}

Size NodeUIStatusHeaderPanel::GetMinSize (NodeUIEnvironment& env) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	Size minSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), headerText);
	minSize = minSize.Grow (2.0 * skinParams.GetNodePadding (), 2.0 * skinParams.GetNodePadding ());
	minSize = minSize.Grow (StatusRectSize * 2, 0);
	return minSize;
}

void NodeUIStatusHeaderPanel::Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (rect, skinParams.GetNodeHeaderBackgroundColor ())));
	Rect textRect = Rect::FromPositionAndSize (Point (rect.GetLeft () + StatusRectSize * 2, rect.GetTop ()), Size (rect.GetWidth () - StatusRectSize * 2, rect.GetHeight ()));
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingText (textRect, skinParams.GetNodeTextFont (), headerText, HorizontalAnchor::Center, VerticalAnchor::Center, skinParams.GetNodeHeaderBackgroundColor (), skinParams.GetNodeHeaderTextColor ())));
	Rect statusRect = Rect::FromPositionAndSize (Point (rect.GetLeft () + StatusRectSize, rect.GetTop () + StatusRectSize), Size (StatusRectSize, rect.GetHeight () - StatusRectSize * 2));
	Color statusColor = (nodeStatus == UINode::Status::HasValue ? skinParams.GetHasValueStatusColor () : skinParams.GetHasNoValueStatusColor ());
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (statusRect, statusColor)));
}

NodeUIMultiLineTextPanel::NodeUIMultiLineTextPanel (const std::vector<std::wstring>& nodeTexts, NodeUIEnvironment& env, size_t textsPerPage, size_t currentPage) :
	NodeUIPanel (),
	nodeTexts (nodeTexts),
	textsPerPage (textsPerPage),
	currentPage (currentPage)
{
	const SkinParams& skinParams = env.GetSkinParams ();
	for (const std::wstring& nodeText : nodeTexts) {
		Size textSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), nodeText);
		textSize = textSize.Grow (2.0 * skinParams.GetNodePadding (), skinParams.GetNodePadding ());
		maxTextSize.SetWidth (std::max (maxTextSize.GetWidth (), textSize.GetWidth ()));
		maxTextSize.SetHeight (std::max (maxTextSize.GetHeight (), textSize.GetHeight ()));
	}	
}

Size NodeUIMultiLineTextPanel::GetMinSize (NodeUIEnvironment& env) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	Size minSize (maxTextSize.GetWidth (), maxTextSize.GetHeight () * std::min (nodeTexts.size (), textsPerPage));
	return minSize.Grow (2.0 * nodePadding, 2.0 * nodePadding);
}

void NodeUIMultiLineTextPanel::Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const
{
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (rect, GetBackgroundColor (env))));
	
	const SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	double xOffset = nodePadding;
	double yOffset = nodePadding;
	double textRectWidth = std::max (maxTextSize.GetWidth (), rect.GetWidth () - 2.0 * nodePadding);
	for (size_t i = 0; i < textsPerPage; ++i) {
		size_t textIndex = (currentPage - 1) * textsPerPage + i;
		if (textIndex < nodeTexts.size ()) {
			const std::wstring& nodeText = nodeTexts[textIndex];
			Point textRectPosition (rect.GetLeft () + xOffset, rect.GetTop () + yOffset);
			Size textRectSize (textRectWidth, maxTextSize.GetHeight ());
			Rect textRect = Rect::FromPositionAndSize (textRectPosition, textRectSize);
			drawingImage.AddItem (DrawingItemConstPtr (new DrawingText (textRect, skinParams.GetNodeTextFont (), nodeText, HorizontalAnchor::Center, VerticalAnchor::Center, GetBackgroundColor (env), GetTextColor (env))));
		}
		yOffset += maxTextSize.GetHeight ();
	}
}

Color NodeUIMultiLineTextPanel::GetTextColor (NodeUIEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeContentTextColor ();
}

Color NodeUIMultiLineTextPanel::GetBackgroundColor (NodeUIEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeContentBackgroundColor ();
}

NodeUISlotPanel::NodeUISlotPanel (const UINode& node, NodeUIEnvironment& env) :
	NodeUIPanel (),
	node (node)
{
	const SkinParams& skinParams = env.GetSkinParams ();

	node.EnumerateUIInputSlots ([&] (const UIInputSlotConstPtr& slot) -> bool {
		Size slotRectSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), slot->GetName ());
		slotRectSize = slotRectSize.Grow (2.0 * skinParams.GetNodePadding (), skinParams.GetNodePadding ());
		inputSlots.AddSlotSize (slot->GetId (), slotRectSize);
		return true;
	});

	node.EnumerateUIOutputSlots ([&] (const UIOutputSlotConstPtr& slot) -> bool {
		Size slotRectSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), slot->GetName ());
		slotRectSize = slotRectSize.Grow (2.0 * skinParams.GetNodePadding (), skinParams.GetNodePadding ());
		outputSlots.AddSlotSize (slot->GetId (), slotRectSize);
		return true;
	});
}

Size NodeUISlotPanel::GetMinSize (NodeUIEnvironment& env) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	double inputSlotsWidth = inputSlots.GetWidth ();
	double outputSlotsWidth = outputSlots.GetWidth ();
	double inputSlotsHeight = inputSlots.GetHeight (nodePadding);
	double outputSlotsHeight = outputSlots.GetHeight (nodePadding);
	Size minSize (inputSlotsWidth + outputSlotsWidth, std::max (inputSlotsHeight, outputSlotsHeight));
	minSize = minSize.Grow (2.0 * nodePadding, 2.0 * nodePadding);
	return minSize;
}

void NodeUISlotPanel::Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();

	drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (rect, skinParams.GetNodeContentBackgroundColor ())));

	Point inputSlotsStartPoint = rect.GetTopLeft () + Point (0.0, nodePadding);
	inputSlots.Enumerate (SlotRectCollection::RefPointMode::TopLeft, inputSlotsStartPoint, nodePadding, [&] (const NE::SlotId& slotId, const Rect& slotRect) {
		UIInputSlotConstPtr uiSlot = node.GetUIInputSlot (slotId);
		Rect textRect = Rect::FromCenterAndSize (slotRect.GetCenter (), slotRect.GetSize ().Grow (2.0 * -nodePadding, -nodePadding));
		drawingImage.AddInputSlotConnPosition (slotId, slotRect.GetLeftCenter ());
		drawingImage.AddInputSlotRect (slotId, slotRect);
		drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (slotRect, skinParams.GetSlotTextBackgroundColor ())));
		drawingImage.AddItem (DrawingItemConstPtr (new DrawingText (textRect, skinParams.GetNodeTextFont (), uiSlot->GetName (), HorizontalAnchor::Left, VerticalAnchor::Center, skinParams.GetSlotTextBackgroundColor (), skinParams.GetSlotTextColor ())));
	});

	Point outputSlotsStartPoint = rect.GetTopRight () + Point (0.0, nodePadding);
	outputSlots.Enumerate (SlotRectCollection::RefPointMode::TopRight, outputSlotsStartPoint, nodePadding, [&] (const NE::SlotId& slotId, const Rect& slotRect) {
		UIOutputSlotConstPtr uiSlot = node.GetUIOutputSlot (slotId);
		Rect textRect = Rect::FromCenterAndSize (slotRect.GetCenter (), slotRect.GetSize ().Grow (2.0 * -nodePadding, -nodePadding));
		drawingImage.AddOutputSlotConnPosition (slotId, slotRect.GetRightCenter ());
		drawingImage.AddOutputSlotRect (slotId, slotRect);
		drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (slotRect, skinParams.GetSlotTextBackgroundColor ())));
		drawingImage.AddItem (DrawingItemConstPtr (new DrawingText (textRect, skinParams.GetNodeTextFont (), uiSlot->GetName (), HorizontalAnchor::Right, VerticalAnchor::Center, skinParams.GetSlotTextBackgroundColor (), skinParams.GetSlotTextColor ())));
	});
}

const Pen NodeUILeftRightButtonsPanel::DefaultButtonBorderPen = Pen (Color (50, 75, 100), 1.0);
const Color NodeUILeftRightButtonsPanel::DefaultButtonBackgroundColor = Color (150, 175, 200);

NodeUILeftRightButtonsPanel::NodeUILeftRightButtonsPanel (	const std::string& leftButtonId,
															const std::wstring& leftButtonText,
															const std::string& rightButtonId,
															const std::wstring& rightButtonText,
															const std::wstring& nodeText,
															NodeUIEnvironment& env) :
	NodeUIPanel (),
	leftButtonId (leftButtonId),
	leftButtonText (leftButtonText),
	rightButtonId (rightButtonId),
	rightButtonText (rightButtonText),
	nodeText (nodeText)
{
	const SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	leftButtonSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), leftButtonText).Grow (2.0 * nodePadding, nodePadding);
	nodeTextSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), nodeText).Grow (2.0 * nodePadding, nodePadding);
	rightButtonSize = env.GetDrawingContext ().MeasureText (skinParams.GetNodeTextFont (), rightButtonText).Grow (2.0 * nodePadding, nodePadding);
}

Size NodeUILeftRightButtonsPanel::GetMinSize (NodeUIEnvironment& env) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	Size minSize;
	minSize.SetWidth (leftButtonSize.GetWidth () + nodeTextSize.GetWidth () + rightButtonSize.GetWidth ());
	minSize.SetHeight (std::max (std::max (leftButtonSize.GetHeight (), rightButtonSize.GetHeight ()), nodeTextSize.GetHeight ()));
	minSize = minSize.Grow (2.0 * nodePadding, 2.0 * nodePadding);
	return minSize;
}

void NodeUILeftRightButtonsPanel::Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	double nodePadding = skinParams.GetNodePadding ();
	
	const Color backgroundColor = GetBackgroundColor (env);
	const Color buttonBackgroundColor = GetButtonBackgroundColor (env);
	const Color textColor = GetTextColor (env);

	drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (rect, backgroundColor)));

	Rect leftButtonRect = Rect::FromPositionAndSize (rect.GetTopLeft () + Point (nodePadding, nodePadding), leftButtonSize);
	Rect rightButtonRect = Rect::FromPositionAndSize (rect.GetTopRight () - Point (rightButtonSize.GetWidth () + nodePadding, -nodePadding), rightButtonSize);
	Rect textRect = Rect::FromPositionAndSize (leftButtonRect.GetTopRight (), Size (rightButtonRect.GetLeft () - leftButtonRect.GetRight (), nodeTextSize.GetHeight ()));
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingText (textRect, skinParams.GetNodeTextFont (), nodeText, HorizontalAnchor::Center, VerticalAnchor::Center, backgroundColor, textColor)));

	drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (leftButtonRect, buttonBackgroundColor)));
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingText (leftButtonRect, skinParams.GetNodeTextFont (), leftButtonText, HorizontalAnchor::Center, VerticalAnchor::Center, buttonBackgroundColor, textColor)));
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingRect (leftButtonRect, GetButtonBorderPen (env))));
	drawingImage.AddSpecialRect (leftButtonId, leftButtonRect);

	drawingImage.AddItem (DrawingItemConstPtr (new DrawingFillRect (rightButtonRect, buttonBackgroundColor)));
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingText (rightButtonRect, skinParams.GetNodeTextFont (), rightButtonText, HorizontalAnchor::Center, VerticalAnchor::Center, buttonBackgroundColor, textColor)));
	drawingImage.AddItem (DrawingItemConstPtr (new DrawingRect (rightButtonRect, GetButtonBorderPen (env))));
	drawingImage.AddSpecialRect (rightButtonId, rightButtonRect);
}

Color NodeUILeftRightButtonsPanel::GetTextColor (NodeUIEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeContentTextColor ();
}

Color NodeUILeftRightButtonsPanel::GetBackgroundColor (NodeUIEnvironment& env) const
{
	return env.GetSkinParams ().GetNodeContentBackgroundColor ();
}

Pen NodeUILeftRightButtonsPanel::GetButtonBorderPen (NodeUIEnvironment&) const
{
	return DefaultButtonBorderPen;
}

Color NodeUILeftRightButtonsPanel::GetButtonBackgroundColor (NodeUIEnvironment&) const
{
	return DefaultButtonBackgroundColor;
}

}
