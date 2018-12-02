#ifndef NUIE_NODEPANEL_HPP
#define NUIE_NODEPANEL_HPP

#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeDrawingImage.hpp"

namespace NUIE
{

class NodePanel
{
public:
	NodePanel ();
	virtual ~NodePanel ();

	virtual Size	GetMinSize (NodeUIDrawingEnvironment& env) const = 0;
	virtual void	Draw (NodeUIDrawingEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const = 0;
};

typedef std::shared_ptr<NodePanel> NodeUIPanelPtr;
typedef std::shared_ptr<const NodePanel> NodeUIPanelConstPtr;

}

#endif
