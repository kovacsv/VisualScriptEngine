#ifndef NODEUIPANELDRAWER_HPP
#define NODEUIPANELDRAWER_HPP

#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeDrawingImage.hpp"

namespace NUIE
{

class NodeUIPanel
{
public:
	virtual Size	GetMinSize (NodeUIDrawingEnvironment& env) const = 0;
	virtual void	Draw (NodeUIDrawingEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const = 0;
};

typedef std::shared_ptr<NodeUIPanel> NodeUIPanelPtr;
typedef std::shared_ptr<const NodeUIPanel> NodeUIPanelConstPtr;

class NodeUIPanelDrawer
{
public:
	NodeUIPanelDrawer ();
	virtual ~NodeUIPanelDrawer ();

	void	AddPanel (const NodeUIPanelPtr& panel);
	void	Draw (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const;

private:
	std::vector<NodeUIPanelPtr> panels;
};

}

#endif
