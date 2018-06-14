#include "NUIE_UINodeGroup.hpp"
#include "NUIE_SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

NE::DynamicSerializationInfo UINodeGroup::serializationInfo (NE::ObjectId ("{FCC7498F-03C5-49C8-B4C0-C88D768F18CD}"), NE::ObjectVersion (1), UINodeGroup::CreateSerializableInstance);

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
	UINodeGroup (L"", NE::NodeCollection ())
{

}

UINodeGroup::UINodeGroup (const std::wstring& name, const NE::NodeCollection& nodes) :
	NE::NodeGroup (nodes),
	name (name)
{

}

UINodeGroup::~UINodeGroup ()
{

}

const std::wstring& UINodeGroup::GetName () const
{
	return name;
}

void UINodeGroup::SetName (const std::wstring& newName)
{
	name = newName;
	InvalidateGroupDrawing ();
}

Rect UINodeGroup::GetRect (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter) const
{
	return GetDrawingImage (env, rectGetter).GetRect ();
}

void UINodeGroup::Draw (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	GetDrawingImage (env, rectGetter).Draw (drawingContext);
}

void UINodeGroup::InvalidateGroupDrawing ()
{
	drawingImage.Reset ();
}

NE::Stream::Status UINodeGroup::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::NodeGroup::Read (inputStream);
	inputStream.Read (name);
	return inputStream.GetStatus ();
}

NE::Stream::Status UINodeGroup::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::NodeGroup::Write (outputStream);
	outputStream.Write (name);
	return outputStream.GetStatus ();
}

const GroupDrawingImage& UINodeGroup::GetDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter) const
{
	if (drawingImage.IsEmpty ()) {
		UpdateDrawingImage (env, rectGetter);
	}
	return drawingImage;
}

void UINodeGroup::UpdateDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	SkinParams& skinParams = env.GetSkinParams ();

	Rect boundingRect;
	bool isEmptyRect = true;
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		Rect nodeRect = rectGetter.GetNodeRect (nodeId);
		if (isEmptyRect) {
			boundingRect = nodeRect;
			isEmptyRect = false;
		} else {
			boundingRect = Rect::FromTwoPoints (
				Point (std::min (nodeRect.GetLeft (), boundingRect.GetLeft ()), std::min (nodeRect.GetTop (), boundingRect.GetTop ())),
				Point (std::max (nodeRect.GetRight (), boundingRect.GetRight ()), std::max (nodeRect.GetBottom (), boundingRect.GetBottom ()))
			);
		}
		return true;
	});

	double groupPadding = skinParams.GetGroupPadding ();

	Size textSize = drawingContext.MeasureText (skinParams.GetGroupNameFont (), name);
	double maxWidth = std::max (textSize.GetWidth (), boundingRect.GetWidth ());
	boundingRect = Rect::FromPositionAndSize (
		boundingRect.GetTopLeft () - Point (groupPadding, 2.0 * groupPadding + textSize.GetHeight ()),
		Size (maxWidth, boundingRect.GetHeight ()) + Size (2.0 * groupPadding, 3.0 * groupPadding + textSize.GetHeight ())
	);

	Rect textRect = Rect::FromPositionAndSize (
		Point (boundingRect.GetX () + groupPadding, boundingRect.GetY () + groupPadding),
		Size (maxWidth, textSize.GetHeight ())
	);

	drawingImage.SetRect (boundingRect);
	drawingImage.AddItem (DrawingItemPtr (new DrawingFillRect (boundingRect, skinParams.GetGroupBackgroundColor ())));
	drawingImage.AddItem (DrawingItemPtr (new DrawingText (textRect, skinParams.GetGroupNameFont (), name, HorizontalAnchor::Left, VerticalAnchor::Center, skinParams.GetGroupNameColor ())));
}

}
