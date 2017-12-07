#ifndef NODEUIPANELDRAWER_HPP
#define NODEUIPANELDRAWER_HPP

#include "NodeUIEnvironment.hpp"
#include "NodeDrawingImage.hpp"

namespace NUIE
{

class NodeUIPanel
{
public:
	NodeUIPanel ();
	virtual ~NodeUIPanel ();

	virtual Size	GetMinSize (NodeUIEnvironment& env) const = 0;
	virtual void	Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const = 0;
};

typedef std::shared_ptr<NodeUIPanel> NodeUIPanelPtr;
typedef std::shared_ptr<const NodeUIPanel> NodeUIPanelConstPtr;

class NodeUIPanelDrawer
{
public:
	NodeUIPanelDrawer ();
	virtual ~NodeUIPanelDrawer ();

	void	AddPanel (const NodeUIPanelPtr& panel);
	void	Draw (NodeUIEnvironment& env, NodeDrawingImage& drawingImage) const;

private:
	std::vector<NodeUIPanelPtr> panels;
};

}

#endif
