#include "NUIE_CommandStructure.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

UICommand::UICommand (const std::wstring& name) :
	name (name)
{

}

UICommand::~UICommand ()
{

}

const std::wstring& UICommand::GetName () const
{
	return name;
}

UIGroupCommand::UIGroupCommand (const std::wstring& name) :
	UICommand (name)
{
	
}

UIGroupCommand::~UIGroupCommand ()
{
	
}

void UIGroupCommand::AddChildCommand (UICommandPtr command)
{
	childCommands.push_back (command);
}

bool UIGroupCommand::IsChecked () const
{
	DBGBREAK ();
	return false;
}

bool UIGroupCommand::HasChildCommands () const
{
	return !childCommands.empty ();
}

void UIGroupCommand::EnumerateChildCommands (const std::function<void (const UICommandPtr& command)>& processor) const
{
	for (const UICommandPtr& command : childCommands) {
		processor (command);
	}
}

void UIGroupCommand::Do ()
{
	DBGBREAK ();
}

SingleUICommand::SingleUICommand (const std::wstring& name, bool isChecked) :
	UICommand (name),
	isChecked (isChecked)
{

}

SingleUICommand::~SingleUICommand ()
{

}

bool SingleUICommand::IsChecked () const
{
	return isChecked;
}

bool SingleUICommand::HasChildCommands () const
{
	return false;
}

void SingleUICommand::EnumerateChildCommands (const std::function<void (const UICommandPtr&)>&) const
{

}

void UICommandStructure::AddCommand (UICommandPtr command)
{
	commands.push_back (command);
}

bool UICommandStructure::IsEmpty () const
{
	return commands.empty ();
}

void UICommandStructure::EnumerateCommands (const std::function<void (const UICommandPtr& command)>& processor) const
{
	for (const UICommandPtr& command : commands) {
		processor (command);
	}
}

}
