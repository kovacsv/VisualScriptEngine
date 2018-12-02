#ifndef NUIE_MENUCOMMANDS_HPP
#define NUIE_MENUCOMMANDS_HPP

#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <memory>

namespace NUIE
{

class MenuCommand;
typedef std::shared_ptr<MenuCommand> MenuCommandPtr;

class SingleMenuCommand;
typedef std::shared_ptr<SingleMenuCommand> SingleMenuCommandPtr;

class GroupMenuCommand;
typedef std::shared_ptr<GroupMenuCommand> GroupMenuCommandPtr;

class MenuCommand
{
public:
	MenuCommand (const std::wstring& name);
	virtual ~MenuCommand ();

	const std::wstring&		GetName () const;

	virtual bool			IsChecked () const = 0;
	virtual bool			HasChildCommands () const = 0;
	virtual void			EnumerateChildCommands (const std::function<void (const MenuCommandPtr& command)>& processor) const = 0;

	virtual void			Do () = 0;

protected:
	std::wstring			name;
};

class SingleMenuCommand : public MenuCommand
{
public:
	SingleMenuCommand (const std::wstring& name, bool isChecked);
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
	GroupMenuCommand (const std::wstring& name);
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
