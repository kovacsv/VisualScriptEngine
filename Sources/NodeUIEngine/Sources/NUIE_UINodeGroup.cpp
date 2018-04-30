#include "NUIE_UINodeGroup.hpp"
#include "NUIE_SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

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

UINodeGroup::UINodeGroup (const std::wstring& name) :
	name (name)
{

}

UINodeGroup::~UINodeGroup ()
{

}

bool UINodeGroup::IsEmpty () const
{
	return nodes.empty ();
}

bool UINodeGroup::ContainsNode (const UINodePtr& node) const
{
	return nodes.find (node) != nodes.end ();
}

bool UINodeGroup::AddNode (const UINodePtr& node)
{
	if (nodes.find (node) != nodes.end ()) {
		return false;
	}
	nodes.insert (node);
	return true;
}

bool UINodeGroup::DeleteNode (const UINodePtr& node)
{
	if (nodes.find (node) == nodes.end ()) {
		return false;
	}
	nodes.erase (node);
	InvalidateGroupDrawing ();
	return true;
}

NodeCollection UINodeGroup::GetNodes () const
{
	NodeCollection nodeCollection;
	for (const UINodePtr& node : nodes) {
		nodeCollection.Insert (node->GetId ());
	}
	return nodeCollection;
}

Rect UINodeGroup::GetRect (NodeUIDrawingEnvironment& env) const
{
	return GetDrawingImage (env).GetRect ();
}

void UINodeGroup::Draw (NodeUIDrawingEnvironment& env) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	GetDrawingImage (env).Draw (drawingContext);
}

void UINodeGroup::InvalidateGroupDrawing ()
{
	drawingImage.Reset ();
}

const GroupDrawingImage& UINodeGroup::GetDrawingImage (NodeUIDrawingEnvironment& env) const
{
	if (drawingImage.IsEmpty ()) {
		UpdateDrawingImage (env);
	}
	return drawingImage;
}

void UINodeGroup::UpdateDrawingImage (NodeUIDrawingEnvironment& env) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	SkinParams& skinParams = env.GetSkinParams ();

	Rect boundingRect;
	bool isEmptyRect = true;
	for (const UINodePtr& node : nodes) {
		Rect nodeRect = node->GetNodeRect (env);
		if (isEmptyRect) {
			boundingRect = nodeRect;
			isEmptyRect = false;
		} else {
			boundingRect = Rect::FromTwoPoints (
				Point (std::min (nodeRect.GetLeft (), boundingRect.GetLeft ()), std::min (nodeRect.GetTop (), boundingRect.GetTop ())),
				Point (std::max (nodeRect.GetRight (), boundingRect.GetRight ()), std::max (nodeRect.GetBottom (), boundingRect.GetBottom ()))
			);
		}
	}

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

bool UINodeGroupList::CreateGroup (const std::wstring& name, const std::vector<UINodePtr>& nodes)
{
	if (DBGERROR (nodes.empty ())) {
		return false;
	}

	UINodeGroupPtr group (new UINodeGroup (name));
	for (const UINodePtr& node : nodes) {
		RemoveNodeFromGroup (node);
		group->AddNode (node);
		nodeToGroup.insert ({ node->GetId (), group });
	}

	groups.push_back (group);
	return true;
}

void UINodeGroupList::DeleteGroup (const UINodeGroupPtr& group)
{
	auto foundInGroups = std::find (groups.begin (), groups.end (), group);
	DBGASSERT (foundInGroups != groups.end ());
	groups.erase (foundInGroups);
}

void UINodeGroupList::RemoveNodeFromGroup (const UINodePtr& node)
{
	auto found = nodeToGroup.find (node->GetId ());
	if (found == nodeToGroup.end ()) {
		return;
	}
	UINodeGroupPtr group = found->second;
	group->DeleteNode (node);
	if (group->IsEmpty ()) {
		DeleteGroup (group);
	}
	nodeToGroup.erase (node->GetId ());
}

void UINodeGroupList::InvalidateNodeGroupDrawing (const UINodePtr& node)
{
	auto found = nodeToGroup.find (node->GetId ());
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

}
