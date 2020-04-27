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

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (const Point&, const MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (const Point&, const UINodePtr&, const MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (const Point&, const UIOutputSlotConstPtr&, const MenuCommandStructure&)
{
	return nullptr;
}

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (const Point&, const UIInputSlotConstPtr&, const MenuCommandStructure&)
{
	return NUIE::MenuCommandPtr ();
}

NUIE::MenuCommandPtr NullEventHandlers::OnContextMenu (const Point&, const UINodeGroupPtr&, const MenuCommandStructure&)
{
	return nullptr;
}

void NullEventHandlers::OnDoubleClick (const Point&)
{

}

bool NullEventHandlers::OnParameterSettings (ParameterInterfacePtr, const UINodePtr&)
{
	return false;
}

bool NullEventHandlers::OnParameterSettings (ParameterInterfacePtr, const UINodeGroupPtr&)
{
	return false;
}

}
