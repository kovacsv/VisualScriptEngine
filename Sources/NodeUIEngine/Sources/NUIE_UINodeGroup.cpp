#include "NUIE_UINodeGroup.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_NodeUIManager.hpp"

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

Rect UINodeGroup::GetRect (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) const
{
	return GetDrawingImage (uiManager, env).GetRect ();
}

void UINodeGroup::Draw (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	GetDrawingImage (uiManager, env).Draw (drawingContext);
}

void UINodeGroup::InvalidateGroupDrawing ()
{
	drawingImage.Reset ();
}

const GroupDrawingImage& UINodeGroup::GetDrawingImage (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) const
{
	if (drawingImage.IsEmpty ()) {
		UpdateDrawingImage (uiManager, env);
	}
	return drawingImage;
}

void UINodeGroup::UpdateDrawingImage (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) const
{
	DrawingContext& drawingContext = env.GetDrawingContext ();
	SkinParams& skinParams = env.GetSkinParams ();

	Rect boundingRect;
	bool isEmptyRect = true;
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		UINodeConstPtr uiNode = uiManager.GetUINode (nodeId);
		Rect nodeRect = uiNode->GetNodeRect (env);
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
	auto foundInGroups = std::find (groups.begin (), groups.end (), group);
	DBGASSERT (foundInGroups != groups.end ());
	groups.erase (foundInGroups);
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
		DeleteGroup (group);
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

}
