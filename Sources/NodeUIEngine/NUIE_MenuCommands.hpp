#ifndef NUIE_MENUCOMMANDS_HPP
#define NUIE_MENUCOMMANDS_HPP

#include "NE_String.hpp"

#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <memory>

namespace NUIE
{

class MenuCommand;
using MenuCommandPtr = std::shared_ptr<MenuCommand>;

class SingleMenuCommand;
using SingleMenuCommandPtr = std::shared_ptr<SingleMenuCommand>;

// TODO: rename
class GroupMenuCommand;
using GroupMenuCommandPtr = std::shared_ptr<GroupMenuCommand>;

class MenuCommand
{
public:
	MenuCommand (const NE::String& name);
	virtual ~MenuCommand ();

	virtual std::wstring	GetName () const;

	virtual bool			IsChecked () const = 0;
	virtual bool			HasChildCommands () const = 0;
	virtual void			EnumerateChildCommands (const std::function<void (const MenuCommandPtr& command)>& processor) const = 0;

	virtual void			Do () = 0;

private:
	NE::String				name;
};

class SingleMenuCommand : public MenuCommand
{
public:
	SingleMenuCommand (const NE::String& name, bool isChecked);
	virtual ~SingleMenuCommand ();

	virtual bool	IsChecked () const override;
	virtual bool	HasChildCommands () const override;
	virtual void	EnumerateChildCommands (const std::function<void (const MenuCommandPtr&)>& processor) const override;

private:
	bool isChecked;
};

class GroupMenuCommand : public MenuCommand
{
public:
	GroupMenuCommand (const NE::String& name);
	virtual ~GroupMenuCommand ();

	void			AddChildCommand (MenuCommandPtr command);

	virtual bool	IsChecked () const override;
	virtual bool	HasChildCommands () const override;
	virtual void	EnumerateChildCommands (const std::function<void (const MenuCommandPtr& command)>& processor) const override;

	virtual void	Do () override;

private:
	std::vector<MenuCommandPtr> childCommands;
};

class MenuCommandStructure
{
public:
	MenuCommandStructure ();
	~MenuCommandStructure ();

	void	AddCommand (MenuCommandPtr command);
	bool	IsEmpty () const;
	void	EnumerateCommands (const std::function<void (const MenuCommandPtr&)>& processor) const;

private:
	std::vector<MenuCommandPtr>	commands;
};

}

#endif
