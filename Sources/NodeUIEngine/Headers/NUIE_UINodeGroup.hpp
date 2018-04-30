#ifndef UINODEGROUP_HPP
#define UINODEGROUP_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_NodeCollection.hpp"

namespace NUIE
{

class GroupDrawingImage : public DrawingImage
{
public:
	GroupDrawingImage ();
	virtual ~GroupDrawingImage ();

	void			Reset ();
	
	const Rect&		GetRect () const;
	void			SetRect (const Rect& newRect);

private:
	Rect rect;
};

class UINodeGroup
{
public:
	UINodeGroup (const std::wstring& name);
	~UINodeGroup ();

	bool	IsEmpty () const;
	bool	ContainsNode (const UINodePtr& node) const;
	bool	AddNode (const UINodePtr& node);
	bool	DeleteNode (const UINodePtr& node);

	void	Draw (NodeUIDrawingEnvironment& env) const;
	void	InvalidateGroupDrawing ();

private:
	const DrawingImage&				GetDrawingImage (NodeUIDrawingEnvironment& env) const;
	void							UpdateDrawingImage (NodeUIDrawingEnvironment& env) const;

	std::wstring					name;
	std::unordered_set<UINodePtr>	nodes;
	mutable GroupDrawingImage		drawingImage;
};

typedef std::shared_ptr<UINodeGroup> UINodeGroupPtr;
typedef std::shared_ptr<const UINodeGroup> UINodeGroupConstPtr;

class UINodeGroupList
{
public:
	UINodeGroupList ();
	~UINodeGroupList ();

	void	Enumerate (const std::function<bool (const UINodeGroupPtr&)>& processor) const;
	bool	CreateGroup (const std::wstring& name, const std::vector<UINodePtr>& nodes);
	void	DeleteGroup (const UINodeGroupPtr& group);
	void	RemoveNodeFromGroup (const UINodePtr& node);
	void	InvalidateNodeGroupDrawing (const UINodePtr& node);

private:
	std::vector<UINodeGroupPtr>						groups;
	std::unordered_map<NE::NodeId, UINodeGroupPtr>	nodeToGroup;	
};

}

#endif
