#include "NUIE_UINodeGroup.hpp"
#include "NUIE_SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

DYNAMIC_SERIALIZATION_INFO (UINodeGroup, 1, "{FCC7498F-03C5-49C8-B4C0-C88D768F18CD}");

GroupDrawingImage::GroupDrawingImage ()
{

}

GroupDrawingImage::~GroupDrawingImage ()
{

}

void GroupDrawingImage::Reset ()
{
	Clear ();
	rect = Rect ();
}

const Rect& GroupDrawingImage::GetRect () const
{
	return rect;
}

void GroupDrawingImage::SetRect (const Rect& newRect)
{
	rect = newRect;
}

NodeRectGetter::NodeRectGetter ()
{

}

NodeRectGetter::~NodeRectGetter ()
{

}

UINodeGroup::UINodeGroup () :
	UINodeGroup (NE::LocString ())
{

}

UINodeGroup::UINodeGroup (const NE::LocString& name) :
	NE::NodeGroup (),
	name (name),
	backgroundColorIndex (0)
{

}

UINodeGroup::~UINodeGroup ()
{

}

const NE::LocString& UINodeGroup::GetName () const
{
	return name;
}

void UINodeGroup::SetName (const std::wstring& newName)
{
	name.SetCustom (newName);
	InvalidateGroupDrawing ();
}

size_t UINodeGroup::GetBackgroundColorIndex () const
{
	return backgroundColorIndex;
}

void UINodeGroup::SetBackgroundColorIndex (size_t newColorIndex)
{
	backgroundColorIndex = newColorIndex;
	InvalidateGroupDrawing ();
}

Rect UINodeGroup::GetRect (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const
{
	return GetDrawingImage (env, rectGetter, nodes).GetRect ();
}

void UINodeGroup::Draw (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	GetDrawingImage (env, rectGetter, nodes).Draw (drawingContext);
}

void UINodeGroup::InvalidateGroupDrawing () const
{
	drawingImage.Reset ();
}

NE::Stream::Status UINodeGroup::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::NodeGroup::Read (inputStream);
	name.Read (inputStream);
	inputStream.Read (backgroundColorIndex);
	return inputStream.GetStatus ();
}

NE::Stream::Status UINodeGroup::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::NodeGroup::Write (outputStream);
	name.Write (outputStream);
	outputStream.Write (backgroundColorIndex);
	return outputStream.GetStatus ();
}

const GroupDrawingImage& UINodeGroup::GetDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const
{
	if (drawingImage.IsEmpty ()) {
		UpdateDrawingImage (env, rectGetter, nodes);
	}
	return drawingImage;
}

void UINodeGroup::UpdateDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const
{
	const SkinParams& skinParams = env.GetSkinParams ();
	DrawingContext& drawingContext = env.GetDrawingContext ();

	BoundingRect boundingRect;
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		Rect nodeRect = rectGetter.GetNodeRect (nodeId);
		boundingRect.AddRect (nodeRect);
		return true;
	});

	double groupPadding = skinParams.GetGroupPadding ();

	std::wstring groupName = name.GetLocalized ();
	Size textSize = drawingContext.MeasureText (skinParams.GetGroupNameFont (), groupName);
	Rect groupRect = boundingRect.GetRect ();
	double maxWidth = std::max (textSize.GetWidth (), groupRect.GetWidth ());
	Rect fullRect = Rect::FromPositionAndSize (
		groupRect.GetTopLeft () - Point (groupPadding, 2.0 * groupPadding + textSize.GetHeight ()),
		Size (maxWidth, groupRect.GetHeight ()) + Size (2.0 * groupPadding, 3.0 * groupPadding + textSize.GetHeight ())
	);

	Rect textRect = Rect::FromPositionAndSize (
		Point (fullRect.GetLeft () + groupPadding, fullRect.GetTop () + groupPadding),
		Size (maxWidth, textSize.GetHeight ())
	);

	drawingImage.SetRect (fullRect);

	const std::vector<NamedColorSet::NamedColor>& backgroundColors = skinParams.GetGroupBackgroundColors ().GetColors ();
	drawingImage.AddItem (DrawingItemPtr (new DrawingFillRect (fullRect, backgroundColors[backgroundColorIndex].color)));
	drawingImage.AddItem (DrawingItemPtr (new DrawingText (textRect, skinParams.GetGroupNameFont (), groupName, HorizontalAnchor::Left, VerticalAnchor::Center, skinParams.GetGroupNameColor ())), DrawingContext::ItemPreviewMode::HideInPreview);
}

}
