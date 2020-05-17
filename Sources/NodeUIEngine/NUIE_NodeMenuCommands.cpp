#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UIItemFinder.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

NodeCommandBase::NodeCommandBase (const std::wstring& name, bool isChecked) :
	name (name),
	isChecked (isChecked)
{

}

NodeCommandBase::~NodeCommandBase ()
{

}

std::wstring NodeCommandBase::GetName () const
{
	return NE::LocalizeString (name);
}

bool NodeCommandBase::IsChecked () const
{
	return isChecked;
}

NodeCommand::NodeCommand (const std::wstring& name, bool isChecked) :
	NodeCommandBase (name, isChecked)
{

}

NodeCommand::~NodeCommand ()
{

}

InputSlotCommand::InputSlotCommand (const std::wstring& name, bool isChecked) :
	NodeCommandBase (name, isChecked)
{

}

InputSlotCommand::~InputSlotCommand ()
{

}

OutputSlotCommand::OutputSlotCommand (const std::wstring& name, bool isChecked) :
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
