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

NUIE::MenuCommandPtr NullEventHandler::OnContextMenu (ContextMenuType, const Point&, const MenuCommandStructure&)
{
	return nullptr;
}

bool NullEventHandler::OnParameterSettings (ParameterSettingsType, ParameterInterfacePtr)
{
	return false;
}

void NullEventHandler::OnDoubleClick (const Point&, MouseButton)
{

}

}
