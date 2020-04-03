#ifndef NUIE_NODEINVALIDATOR_HPP
#define NUIE_NODEINVALIDATOR_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_NodeUIManager.hpp"

namespace NUIE
{

class NodeInvalidator
{
public:
	NodeInvalidator ();
	virtual ~NodeInvalidator ();

	void			InvalidateValueAndDrawing ();

	virtual void	InvalidateValue () = 0;
	virtual void	InvalidateDrawing () = 0;
};

class NodeUIManagerNodeInvalidator : public NodeInvalidator
{
public:
	NodeUIManagerNodeInvalidator (NodeUIManager& uiManager, UINodePtr& uiNode);
	virtual ~NodeUIManagerNodeInvalidator ();

	virtual void	InvalidateValue () override;
	virtual void	InvalidateDrawing () override;

private:
	NodeUIManager& uiManager;
	UINodePtr& uiNode;
};

}

#endif
