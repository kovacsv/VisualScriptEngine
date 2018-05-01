#ifndef UINODEGROUP_HPP
#define UINODEGROUP_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_NodeCollection.hpp"

namespace NUIE
{

class NodeUIManager;

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
	SERIALIZABLE;

public:
	UINodeGroup ();
	UINodeGroup (const std::wstring& name);
	~UINodeGroup ();

	const std::wstring&		GetName () const;
	void					SetName (const std::wstring& newName);

	bool					IsEmpty () const;
	bool					ContainsNode (const NE::NodeId& nodeId) const;

	bool					AddNode (const NE::NodeId& nodeId);
	bool					DeleteNode (const NE::NodeId& nodeId);
	const NodeCollection&	GetNodes () const;

	Rect					GetRect (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) const;
	void					Draw (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) const;
	void					InvalidateGroupDrawing ();

	NE::Stream::Status		Read (NE::InputStream& inputStream);
	NE::Stream::Status		Write (NE::OutputStream& outputStream) const;

private:
	const GroupDrawingImage&		GetDrawingImage (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) const;
	void							UpdateDrawingImage (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) const;

	std::wstring					name;
	NodeCollection					nodes;
	mutable GroupDrawingImage		drawingImage;
};

typedef std::shared_ptr<UINodeGroup> UINodeGroupPtr;
typedef std::shared_ptr<const UINodeGroup> UINodeGroupConstPtr;

class UINodeGroupList
{
	SERIALIZABLE;

public:
	UINodeGroupList ();
	~UINodeGroupList ();

	void				Enumerate (const std::function<bool (const UINodeGroupPtr&)>& processor) const;
	bool				CreateGroup (const std::wstring& name, const NodeCollection& nodes);
	void				DeleteGroup (const UINodeGroupPtr& group);
	void				RemoveNodeFromGroup (const NE::NodeId& nodeId);
	void				InvalidateNodeGroupDrawing (const NE::NodeId& nodeId);
	void				Clear ();

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

private:
	std::vector<UINodeGroupPtr>						groups;
	std::unordered_map<NE::NodeId, UINodeGroupPtr>	nodeToGroup;	
};

}

#endif
