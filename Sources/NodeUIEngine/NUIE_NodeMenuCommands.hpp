#ifndef NUIE_NODEMENUCOMMANDS_HPP
#define NUIE_NODEMENUCOMMANDS_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_UINodeInvalidator.hpp"
#include "NUIE_MenuCommands.hpp"

namespace NUIE
{

class NodeUIManager;
class NodeUIEnvironment;

template <typename CommandType>
class NodeGroupCommand
{
public:
	NodeGroupCommand (const NE::LocString& name);
	virtual ~NodeGroupCommand ();

	std::wstring			GetName () const;
	bool					HasChildCommand () const;
	void					AddChildCommand (const CommandType& command);
	void					EnumerateChildCommands (const std::function<void (const CommandType&)>& processor);

private:
	NE::LocString				name;
	std::vector<CommandType>	childCommands;
};

template <typename CommandType>
NodeGroupCommand<CommandType>::NodeGroupCommand (const NE::LocString& name) :
	name (name)
{

}

template <typename CommandType>
NodeGroupCommand<CommandType>::~NodeGroupCommand ()
{
}

template <typename CommandType>
std::wstring NodeGroupCommand<CommandType>::GetName () const
{
	return name.GetLocalized ();
}

template <typename CommandType>
bool NodeGroupCommand<CommandType>::HasChildCommand () const
{
	return !childCommands.empty ();
}

template <typename CommandType>
void NodeGroupCommand<CommandType>::AddChildCommand (const CommandType& command)
{
	childCommands.push_back (command);
}

template <typename CommandType>
void NodeGroupCommand<CommandType>::EnumerateChildCommands (const std::function<void (const CommandType&)>& processor)
{
	for (const CommandType& command : childCommands) {
		processor (command);
	}
}

class NodeCommandBase
{
public:
	NodeCommandBase (const NE::LocString& name, bool isChecked);
	virtual ~NodeCommandBase ();

	std::wstring	GetName () const;
	bool			IsChecked () const;

private:
	NE::LocString	name;
	bool			isChecked;
};

class NodeCommand : public NodeCommandBase
{
public:
	NodeCommand (const NE::LocString& name, bool isChecked);
	virtual ~NodeCommand ();

	virtual bool	IsApplicableTo (const UINodeConstPtr& uiNode) = 0;
	virtual void	Do (UINodeInvalidator& invalidator, NodeUIEnvironment& uiEnvironment, UINodePtr& uiNode) = 0;
};

using NodeCommandPtr = std::shared_ptr<NodeCommand>;
using NodeGroupCommandPtr = std::shared_ptr<NodeGroupCommand<NodeCommandPtr>>;

class InputSlotCommand : public NodeCommandBase
{
public:
	InputSlotCommand (const NE::LocString& name, bool isChecked);
	virtual ~InputSlotCommand ();

	virtual void	Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UIInputSlotPtr& inputSlot) = 0;
};

using InputSlotCommandPtr = std::shared_ptr<InputSlotCommand>;
using InputSlotGroupCommandPtr = std::shared_ptr<NodeGroupCommand<InputSlotCommandPtr>>;

class OutputSlotCommand : public NodeCommandBase
{
public:
	OutputSlotCommand (const NE::LocString& name, bool isChecked);
	virtual ~OutputSlotCommand ();

	virtual void	Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UIOutputSlotPtr& outputSlot) = 0;
};

using OutputSlotCommandPtr = std::shared_ptr<OutputSlotCommand>;
using OutputSlotGroupCommandPtr = std::shared_ptr<NodeGroupCommand<OutputSlotCommandPtr>>;

class NodeCommandRegistrator
{
public:
	NodeCommandRegistrator ();
	virtual ~NodeCommandRegistrator ();

	virtual void RegisterNodeCommand (NodeCommandPtr nodeCommand) = 0;
	virtual void RegisterNodeGroupCommand (NodeGroupCommandPtr nodeGroupCommand) = 0;
};

class InputSlotCommandRegistrator
{
public:
	InputSlotCommandRegistrator ();
	virtual ~InputSlotCommandRegistrator ();

	virtual void RegisterSlotCommand (InputSlotCommandPtr slotCommand) = 0;
	virtual void RegisterSlotGroupCommand (InputSlotGroupCommandPtr nodeGroupCommand) = 0;
};

class OutputSlotCommandRegistrator
{
public:
	OutputSlotCommandRegistrator ();
	virtual ~OutputSlotCommandRegistrator ();

	virtual void RegisterSlotCommand (OutputSlotCommandPtr slotCommand) = 0;
	virtual void RegisterSlotGroupCommand (OutputSlotGroupCommandPtr nodeGroupCommand) = 0;
};

}

#endif
