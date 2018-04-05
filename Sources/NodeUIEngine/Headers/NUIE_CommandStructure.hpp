#ifndef COMMANDSTRUCTURE_HPP
#define COMMANDSTRUCTURE_HPP

#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <memory>

namespace NUIE
{

class Command;
typedef std::shared_ptr<Command> CommandPtr;

class GroupCommand;
typedef std::shared_ptr<GroupCommand> GroupCommandPtr;

class SingleCommand;
typedef std::shared_ptr<SingleCommand> SingleCommandPtr;

class MultiCommand;
typedef std::shared_ptr<MultiCommand> MultiCommandPtr;

class CommandId
{
public:
	CommandId (const std::string& id);
	~CommandId ();

	bool	operator== (const CommandId& rhs) const;
	bool	operator!= (const CommandId& rhs) const;

private:
	std::string id;
};

class Command
{
public:
	Command (const std::wstring& name);
	virtual ~Command ();

	const std::wstring&		GetName () const;

	virtual bool			IsChecked () const = 0;
	virtual bool			HasChildCommands () const = 0;
	virtual void			EnumerateChildCommands (const std::function<void (const CommandPtr& command)>& processor) const = 0;

	virtual void			Do () = 0;

protected:
	std::wstring			name;
};

class GroupCommand : public Command
{
public:
	GroupCommand (const std::wstring& name);
	virtual ~GroupCommand ();

	void			AddChildCommand (CommandPtr command);

	virtual bool	IsChecked () const override;
	virtual bool	HasChildCommands () const override;
	virtual void	EnumerateChildCommands (const std::function<void (const CommandPtr& command)>& processor) const override;
	virtual void	Do () override;

private:
	std::vector<CommandPtr> childCommands;
};

class SingleCommand : public Command
{
public:
	SingleCommand (const std::wstring& name, bool isChecked);
	virtual ~SingleCommand ();

	virtual bool	IsChecked () const override;
	virtual bool	HasChildCommands () const override;
	virtual void	EnumerateChildCommands (const std::function<void (const CommandPtr&)>& processor) const override;

private:
	bool isChecked;
};

class CommandStructure
{
public:
	void	AddCommand (CommandPtr command);
	bool	IsEmpty () const;
	void	EnumerateCommands (const std::function<void (const CommandPtr&)>& processor) const;

private:
	std::vector<CommandPtr>	commands;
};

}

#endif
