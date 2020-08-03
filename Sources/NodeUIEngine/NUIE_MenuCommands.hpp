#ifndef NUIE_MENUCOMMANDS_HPP
#define NUIE_MENUCOMMANDS_HPP

#include "NE_LocString.hpp"

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

class MultiMenuCommand;
using MultiMenuCommandPtr = std::shared_ptr<MultiMenuCommand>;

class MenuCommand
{
public:
	MenuCommand (const NE::LocString& name);
	virtual ~MenuCommand ();

	virtual std::wstring					GetName () const;

	virtual bool							IsChecked () const = 0;
	virtual bool							HasChildCommands () const = 0;
	virtual std::vector<MenuCommandPtr>		GetChildCommands () const = 0;
	virtual void							EnumerateChildCommands (const std::function<void (const MenuCommandPtr& command)>& processor) const = 0;

	virtual void							Do () = 0;

private:
	NE::LocString			name;
};

class SingleMenuCommand : public MenuCommand
{
public:
	SingleMenuCommand (const NE::LocString& name, bool isChecked);
	virtual ~SingleMenuCommand ();

	virtual bool							IsChecked () const override;
	virtual bool							HasChildCommands () const override;
	virtual std::vector<MenuCommandPtr>		GetChildCommands () const override;
	virtual void							EnumerateChildCommands (const std::function<void (const MenuCommandPtr&)>& processor) const override;

private:
	bool isChecked;
};

class MultiMenuCommand : public MenuCommand
{
public:
	MultiMenuCommand (const NE::LocString& name);
	virtual ~MultiMenuCommand ();

	void									AddChildCommand (MenuCommandPtr command);

	virtual bool							IsChecked () const override;
	virtual bool							HasChildCommands () const override;
	virtual std::vector<MenuCommandPtr>		GetChildCommands () const override;
	virtual void							EnumerateChildCommands (const std::function<void (const MenuCommandPtr& command)>& processor) const override;

	virtual void							Do () override;

private:
	std::vector<MenuCommandPtr> childCommands;
};

class MenuCommandStructure
{
public:
	MenuCommandStructure ();
	~MenuCommandStructure ();

	void							AddCommand (MenuCommandPtr command);
	std::vector<MenuCommandPtr>		GetCommands () const;

	bool							IsEmpty () const;
	void							EnumerateCommands (const std::function<void (const MenuCommandPtr&)>& processor) const;

private:
	std::vector<MenuCommandPtr>	commands;
};

}

#endif
