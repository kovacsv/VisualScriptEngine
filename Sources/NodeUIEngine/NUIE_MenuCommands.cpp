#include "NUIE_MenuCommands.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

MenuCommand::MenuCommand (const NE::LocString& name) :
	name (name)
{

}

MenuCommand::~MenuCommand ()
{

}

void MenuCommand::Do ()
{
	if (!WillModify ()) {
		return;
	}
	DoModification ();
}

std::wstring MenuCommand::GetName () const
{
	return name.GetLocalized ();
}

SingleMenuCommand::SingleMenuCommand (const NE::LocString& name, bool isChecked) :
	MenuCommand (name),
	isChecked (isChecked)
{

}

SingleMenuCommand::~SingleMenuCommand ()
{

}

bool SingleMenuCommand::IsChecked () const
{
	return isChecked;
}

bool SingleMenuCommand::HasChildCommands () const
{
	return false;
}

void SingleMenuCommand::EnumerateChildCommands (const std::function<void (const MenuCommandPtr&)>&) const
{

}

std::vector<MenuCommandPtr> SingleMenuCommand::GetChildCommands () const
{
	return {};
}

MultiMenuCommand::MultiMenuCommand (const NE::LocString& name) :
	MenuCommand (name)
{
	
}

MultiMenuCommand::~MultiMenuCommand ()
{
	
}

void MultiMenuCommand::AddChildCommand (MenuCommandPtr command)
{
	childCommands.push_back (command);
}

bool MultiMenuCommand::IsChecked () const
{
	DBGBREAK ();
	return false;
}

bool MultiMenuCommand::HasChildCommands () const
{
	return !childCommands.empty ();
}

void MultiMenuCommand::EnumerateChildCommands (const std::function<void (const MenuCommandPtr& command)>& processor) const
{
	for (const MenuCommandPtr& command : childCommands) {
		processor (command);
	}
}

std::vector<MenuCommandPtr> MultiMenuCommand::GetChildCommands () const
{
	return childCommands;
}

bool MultiMenuCommand::WillModify () const
{
	DBGBREAK ();
	return false;
}

void MultiMenuCommand::DoModification ()
{
	DBGBREAK ();
}

MenuCommandStructure::MenuCommandStructure ()
{

}

MenuCommandStructure::~MenuCommandStructure ()
{

}

void MenuCommandStructure::AddCommand (MenuCommandPtr command)
{
	commands.push_back (command);
}

std::vector<MenuCommandPtr> MenuCommandStructure::GetCommands () const
{
	return commands;
}

bool MenuCommandStructure::IsEmpty () const
{
	return commands.empty ();
}

void MenuCommandStructure::EnumerateCommands (const std::function<void (const MenuCommandPtr& command)>& processor) const
{
	for (const MenuCommandPtr& command : commands) {
		processor (command);
	}
}

}
