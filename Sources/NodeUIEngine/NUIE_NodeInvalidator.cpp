#include "NUIE_NodeInvalidator.hpp"

namespace NUIE
{

NodeInvalidator::NodeInvalidator ()
{

}

NodeInvalidator::~NodeInvalidator ()
{

}

void NodeInvalidator::InvalidateValueAndDrawing ()
{
	InvalidateValue ();
	InvalidateDrawing ();
}

NodeUIManagerNodeInvalidator::NodeUIManagerNodeInvalidator (NodeUIManager& uiManager, UINodePtr& uiNode) :
	NodeInvalidator (),
	uiManager (uiManager),
	uiNode (uiNode)
{
}

NodeUIManagerNodeInvalidator::~NodeUIManagerNodeInvalidator ()
{

}

void NodeUIManagerNodeInvalidator::InvalidateValue ()
{
	uiManager.InvalidateNodeValue (uiNode);
}

void NodeUIManagerNodeInvalidator::InvalidateDrawing ()
{
	uiManager.InvalidateNodeDrawing (uiNode);
}

}
