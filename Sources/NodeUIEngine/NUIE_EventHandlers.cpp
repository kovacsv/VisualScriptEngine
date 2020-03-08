#include "NUIE_EventHandlers.hpp"

namespace NUIE
{

EventHandlers::EventHandlers ()
{

}

EventHandlers::~EventHandlers ()
{

}

NullEventHandlers::NullEventHandlers ()
{
}

NullEventHandlers::~NullEventHandlers ()
{
}

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodePtr&, const MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UIOutputSlotConstPtr&, const MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UIInputSlotConstPtr&, const MenuCommandStructure&)
{
	return NUIE::MenuCommandPtr ();
}

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodeGroupPtr&, const MenuCommandStructure&)
{
	return nullptr;
}

bool NullEventHandlers::OnParameterSettings (ParameterInterfacePtr)
{
	return false;
}

}
