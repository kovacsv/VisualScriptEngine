#include "NUIE_UINodeGroup.hpp"
#include "NUIE_SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

NE::SerializationInfo UINodeGroup::serializationInfo (NE::ObjectVersion (1));
NE::SerializationInfo UINodeGroupList::serializationInfo (NE::ObjectVersion (1));

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

UINodeGroup::UINodeGroup () :
	UINodeGroup (L"")
{

}

UINodeGroup::UINodeGroup (const std::wstring& name) :
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

bool UINodeGroup::IsEmpty () const
{
	return nodes.IsEmpty ();
}

bool UINodeGroup::ContainsNode (const NE::NodeId& nodeId) const
{
	return nodes.Contains (nodeId);
}

bool UINodeGroup::AddNode (const NE::NodeId& nodeId)
{
	if (nodes.Contains (nodeId)) {
		return false;
	}
	nodes.Insert (nodeId);
	return true;
}

bool UINodeGroup::DeleteNode (const NE::NodeId& nodeId)
{
	if (!nodes.Contains (nodeId)) {
		return false;
	}
	nodes.Erase (nodeId);
	InvalidateGroupDrawing ();
	return true;
}

const NodeCollection& UINodeGroup::GetNodes () const
{
	return nodes;
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
	inputStream.Read (name);
	nodes.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status UINodeGroup::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (name);
	nodes.Write (outputStream);
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
	
UINodeGroupList::UINodeGroupList ()
{

}

UINodeGroupList::~UINodeGroupList ()
{

}

void UINodeGroupList::Enumerate (const std::function<bool (const UINodeGroupPtr&)>& processor) const
{
	for (const UINodeGroupPtr& group : groups) {
		if (!processor (group)) {
			break;
		}
	}
}

bool UINodeGroupList::CreateGroup (const std::wstring& name, const NodeCollection& nodes)
{
	if (DBGERROR (nodes.IsEmpty ())) {
		return false;
	}

	UINodeGroupPtr group (new UINodeGroup (name));
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		RemoveNodeFromGroup (nodeId);
		group->AddNode (nodeId);
		nodeToGroup.insert ({ nodeId, group });
		return true;
	});

	groups.push_back (group);
	return true;
}

void UINodeGroupList::DeleteGroup (const UINodeGroupPtr& group)
{
	NodeCollection nodes = group->GetNodes ();
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		RemoveNodeFromGroup (nodeId);
		return true;
	});
}

void UINodeGroupList::RemoveNodeFromGroup (const NE::NodeId& nodeId)
{
	auto found = nodeToGroup.find (nodeId);
	if (found == nodeToGroup.end ()) {
		return;
	}
	UINodeGroupPtr group = found->second;
	group->DeleteNode (nodeId);
	if (group->IsEmpty ()) {
		auto foundInGroups = std::find (groups.begin (), groups.end (), group);
		DBGASSERT (foundInGroups != groups.end ());
		groups.erase (foundInGroups);
	}
	nodeToGroup.erase (nodeId);
}

void UINodeGroupList::InvalidateNodeGroupDrawing (const NE::NodeId& nodeId)
{
	auto found = nodeToGroup.find (nodeId);
	if (found == nodeToGroup.end ()) {
		return;
	}
	found->second->InvalidateGroupDrawing ();
}

void UINodeGroupList::Clear ()
{
	groups.clear ();
	nodeToGroup.clear ();
}

NE::Stream::Status UINodeGroupList::Read (NE::InputStream& inputStream)
{
	DBGASSERT (groups.empty ());
	NE::ObjectHeader header (inputStream);
	size_t groupCount = 0;
	inputStream.Read (groupCount);
	for (size_t i = 0; i < groupCount; i++) {
		UINodeGroupPtr group (new UINodeGroup ());
		group->Read (inputStream);
		const NodeCollection& nodes = group->GetNodes ();
		groups.push_back (group);
		nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			nodeToGroup.insert ({ nodeId, group });
			return true;
		});
	}
	return inputStream.GetStatus ();
}

NE::Stream::Status UINodeGroupList::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (groups.size ());
	for (const UINodeGroupPtr& group : groups) {
		group->Write (outputStream);
	}
	return outputStream.GetStatus ();
}

}
