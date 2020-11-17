#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UIItemFinder.hpp"

namespace NUIE
{

NodeCommandBase::NodeCommandBase (const NE::LocString& name, bool isChecked) :
	name (name),
	isChecked (isChecked)
{

}

NodeCommandBase::~NodeCommandBase ()
{

}

std::wstring NodeCommandBase::GetName () const
{
	return name.GetLocalized ();
}

bool NodeCommandBase::IsChecked () const
{
	return isChecked;
}

NodeCommand::NodeCommand (const NE::LocString& name, bool isChecked) :
	NodeCommandBase (name, isChecked)
{

}

NodeCommand::~NodeCommand ()
{

}

InputSlotCommand::InputSlotCommand (const NE::LocString& name, bool isChecked) :
	NodeCommandBase (name, isChecked)
{

}

InputSlotCommand::~InputSlotCommand ()
{

}

OutputSlotCommand::OutputSlotCommand (const NE::LocString& name, bool isChecked) :
	NodeCommandBase (name, isChecked)
{

}

OutputSlotCommand::~OutputSlotCommand ()
{

}

InputSlotCommandRegistrator::InputSlotCommandRegistrator ()
{

}

InputSlotCommandRegistrator::~InputSlotCommandRegistrator ()
{

}

NodeCommandRegistrator::NodeCommandRegistrator ()
{

}

NodeCommandRegistrator::~NodeCommandRegistrator ()
{

}

OutputSlotCommandRegistrator::OutputSlotCommandRegistrator ()
{

}

OutputSlotCommandRegistrator::~OutputSlotCommandRegistrator ()
{

}

}
