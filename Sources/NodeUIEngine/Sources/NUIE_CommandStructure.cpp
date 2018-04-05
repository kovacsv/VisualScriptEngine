#include "NUIE_CommandStructure.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

CommandId::CommandId (const std::string& id) :
	id (id)
{

}

CommandId::~CommandId ()
{

}

bool CommandId::operator== (const CommandId& rhs) const
{
	return id == rhs.id;
}

bool CommandId::operator!= (const CommandId& rhs) const
{
	return !operator== (rhs);
}

Command::Command (const std::wstring& name) :
	name (name)
{

}

Command::~Command ()
{

}

const std::wstring& Command::GetName () const
{
	return name;
}

GroupCommand::GroupCommand (const std::wstring& name) :
	Command (name)
{
	
}

GroupCommand::~GroupCommand ()
{
	
}

void GroupCommand::AddChildCommand (CommandPtr command)
{
	childCommands.push_back (command);
}

bool GroupCommand::IsChecked () const
{
	return false;
}

bool GroupCommand::HasChildCommands () const
{
	return !childCommands.empty ();
}

void GroupCommand::EnumerateChildCommands (const std::function<void (const CommandPtr& command)>& processor) const
{
	for (const CommandPtr& command : childCommands) {
		processor (command);
	}
}

void GroupCommand::Do ()
{
	DBGBREAK ();
}

SingleCommand::SingleCommand (const std::wstring& name, bool isChecked) :
	Command (name),
	isChecked (isChecked)
{

}

SingleCommand::~SingleCommand ()
{

}

bool SingleCommand::IsChecked () const
{
	return isChecked;
}

bool SingleCommand::HasChildCommands () const
{
	return false;
}

void SingleCommand::EnumerateChildCommands (const std::function<void (const CommandPtr&)>&) const
{

}

void CommandStructure::AddCommand (CommandPtr command)
{
	commands.push_back (command);
}

bool CommandStructure::IsEmpty () const
{
	return commands.empty ();
}

void CommandStructure::EnumerateCommands (const std::function<void (const CommandPtr& command)>& processor) const
{
	for (const CommandPtr& command : commands) {
		processor (command);
	}
}

}
