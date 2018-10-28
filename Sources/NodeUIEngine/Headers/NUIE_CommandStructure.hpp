#ifndef NUIE_COMMANDSTRUCTURE_HPP
#define NUIE_COMMANDSTRUCTURE_HPP

#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <memory>

namespace NUIE
{

class UICommand;
typedef std::shared_ptr<UICommand> UICommandPtr;

class UIGroupCommand;
typedef std::shared_ptr<UIGroupCommand> UIGroupCommandPtr;

class SingleUICommand;
typedef std::shared_ptr<SingleUICommand> SingleUICommandPtr;

class MultiUICommand;
typedef std::shared_ptr<MultiUICommand> MultiUICommandPtr;

class UICommand
{
public:
	UICommand (const std::wstring& name);
	virtual ~UICommand ();

	const std::wstring&		GetName () const;

	virtual bool			IsChecked () const = 0;
	virtual bool			HasChildCommands () const = 0;
	virtual void			EnumerateChildCommands (const std::function<void (const UICommandPtr& command)>& processor) const = 0;

	virtual bool			IsUndoable () const = 0;
	virtual void			Do () = 0;

protected:
	std::wstring			name;
};

class UIGroupCommand : public UICommand
{
public:
	UIGroupCommand (const std::wstring& name);
	virtual ~UIGroupCommand ();

	void			AddChildCommand (UICommandPtr command);

	virtual bool	IsChecked () const override;
	virtual bool	HasChildCommands () const override;
	virtual void	EnumerateChildCommands (const std::function<void (const UICommandPtr& command)>& processor) const override;

	virtual bool	IsUndoable () const override;
	virtual void	Do () override;

private:
	std::vector<UICommandPtr> childCommands;
};

class SingleUICommand : public UICommand
{
public:
	SingleUICommand (const std::wstring& name, bool isChecked);
	virtual ~SingleUICommand ();

	virtual bool	IsChecked () const override;
	virtual bool	HasChildCommands () const override;
	virtual void	EnumerateChildCommands (const std::function<void (const UICommandPtr&)>& processor) const override;

private:
	bool isChecked;
};

class UICommandStructure
{
public:
	void	AddCommand (UICommandPtr command);
	bool	IsEmpty () const;
	void	EnumerateCommands (const std::function<void (const UICommandPtr&)>& processor) const;

private:
	std::vector<UICommandPtr>	commands;
};

}

#endif
