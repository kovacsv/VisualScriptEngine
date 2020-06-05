#include "NUIE_MenuCommands.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

MenuCommand::MenuCommand (const NE::String& name) :
	name (name)
{

}

MenuCommand::~MenuCommand ()
{

}

std::wstring MenuCommand::GetName () const
{
	return name.GetLocalized ();
}

SingleMenuCommand::SingleMenuCommand (const NE::String& name, bool isChecked) :
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

GroupMenuCommand::GroupMenuCommand (const NE::String& name) :
	MenuCommand (name)
{
	
}

GroupMenuCommand::~GroupMenuCommand ()
{
	
}

void GroupMenuCommand::AddChildCommand (MenuCommandPtr command)
{
	childCommands.push_back (command);
}

bool GroupMenuCommand::IsChecked () const
{
	DBGBREAK ();
	return false;
}

bool GroupMenuCommand::HasChildCommands () const
{
	return !childCommands.empty ();
}

void GroupMenuCommand::EnumerateChildCommands (const std::function<void (const MenuCommandPtr& command)>& processor) const
{
	for (const MenuCommandPtr& command : childCommands) {
		processor (command);
	}
}

void GroupMenuCommand::Do ()
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
