#include "UINodeCommands.hpp"
#include "NodeUIManager.hpp"
#include "UIItemFinder.hpp"
#include "Debug.hpp"

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

const std::wstring& NodeCommandBase::GetName () const
{
	return name;
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
