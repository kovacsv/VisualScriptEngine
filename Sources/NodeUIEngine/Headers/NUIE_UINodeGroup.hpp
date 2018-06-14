#ifndef UINODEGROUP_HPP
#define UINODEGROUP_HPP

#include "NE_NodeGroup.hpp"
#include "NUIE_UINode.hpp"

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

class NodeRectGetter
{
public:
	NodeRectGetter ();
	virtual ~NodeRectGetter ();

	virtual Rect GetNodeRect (const NE::NodeId& nodeId) const = 0;
};

class UINodeGroup : public NE::NodeGroup
{
	DYNAMIC_SERIALIZABLE (UINodeGroup);

public:
	UINodeGroup ();
	UINodeGroup (const std::wstring& name, const NE::NodeCollection& nodes);
	~UINodeGroup ();

	const std::wstring&			GetName () const;
	void						SetName (const std::wstring& newName);

	Rect						GetRect (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter) const;
	void						Draw (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter) const;
	void						InvalidateGroupDrawing ();

	NE::Stream::Status			Read (NE::InputStream& inputStream);
	NE::Stream::Status			Write (NE::OutputStream& outputStream) const;

private:
	const GroupDrawingImage&	GetDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter) const;
	void						UpdateDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter) const;

	std::wstring				name;
	mutable GroupDrawingImage	drawingImage;
};

typedef std::shared_ptr<UINodeGroup> UINodeGroupPtr;
typedef std::shared_ptr<const UINodeGroup> UINodeGroupConstPtr;

}

#endif
