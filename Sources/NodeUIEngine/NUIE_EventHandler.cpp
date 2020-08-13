#include "NUIE_EventHandler.hpp"

namespace NUIE
{

EventHandler::EventHandler ()
{

}

EventHandler::~EventHandler ()
{

}

NullEventHandler::NullEventHandler ()
{
}

NullEventHandler::~NullEventHandler ()
{
}

NUIE::MenuCommandPtr NullEventHandler::OnContextMenu (const Point&, const MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr NullEventHandler::OnContextMenu (const Point&, const UINodePtr&, const MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr NullEventHandler::OnContextMenu (const Point&, const UIOutputSlotConstPtr&, const MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr NullEventHandler::OnContextMenu (const Point&, const UIInputSlotConstPtr&, const MenuCommandStructure&)
{
	return NUIE::MenuCommandPtr ();
}

NUIE::MenuCommandPtr NullEventHandler::OnContextMenu (const Point&, const UINodeGroupPtr&, const MenuCommandStructure&)
{
	return nullptr;
}

void NullEventHandler::OnDoubleClick (const Point&, MouseButton)
{

}

bool NullEventHandler::OnParameterSettings (ParameterInterfacePtr, const UINodePtr&)
{
	return false;
}

bool NullEventHandler::OnParameterSettings (ParameterInterfacePtr, const UINodeGroupPtr&)
{
	return false;
}

}
