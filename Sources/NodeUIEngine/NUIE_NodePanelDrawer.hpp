#ifndef NUIE_NODEPANELDRAWER_HPP
#define NUIE_NODEPANELDRAWER_HPP

#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeDrawingImage.hpp"
#include "NUIE_NodePanel.hpp"

namespace NUIE
{
class NodePanelDrawer
{
public:
	NodePanelDrawer ();
	virtual ~NodePanelDrawer ();

	void	AddPanel (const NodeUIPanelPtr& panel);
	void	Draw (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const;

private:
	std::vector<NodeUIPanelPtr> panels;
};

}

#endif
