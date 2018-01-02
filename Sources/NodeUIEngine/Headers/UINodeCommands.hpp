#ifndef UINODECOMMANDS_HPP
#define UINODECOMMANDS_HPP

#include "UINode.hpp"
#include "CommandStructure.hpp"

namespace NUIE
{

class NodeUIManager;

template <typename CommandType>
class NodeGroupCommand
{
public:
	NodeGroupCommand (const std::wstring& name);
	virtual ~NodeGroupCommand ();

	const std::wstring&		GetName () const;
	void					AddChildCommand (const CommandType& command);
	void					EnumerateChildCommands (const std::function<void (const CommandType&)>& processor);

private:
	std::wstring				name;
	std::vector<CommandType>	childCommands;
};

template <typename CommandType>
NodeGroupCommand<CommandType>::NodeGroupCommand (const std::wstring& name) :
	name (name)
{

}

template <typename CommandType>
NodeGroupCommand<CommandType>::~NodeGroupCommand ()
{
}

template <typename CommandType>
const std::wstring& NodeGroupCommand<CommandType>::GetName () const
{
	return name;
}

template <typename CommandType>
void NodeGroupCommand<CommandType>::AddChildCommand (const CommandType& command)
{
	childCommands.push_back (command);
}

template <typename CommandType>
void NodeGroupCommand<CommandType>::EnumerateChildCommands (const std::function<void(const CommandType&)>& processor)
{
	for (const CommandType& command : childCommands) {
		processor (command);
	}
}

class NodeCommandBase
{
public:
	NodeCommandBase (const std::wstring& name, bool isChecked);
	virtual ~NodeCommandBase ();

	const std::wstring&		GetName () const;
	bool					IsChecked () const;

private:
	std::wstring	name;
	bool			isChecked;
};

class NodeCommand : public NodeCommandBase
{
public:
	NodeCommand (const std::wstring& name, bool isChecked);
	virtual ~NodeCommand ();

	virtual bool	IsApplicableTo (const UINodePtr& uiNode) = 0;
	virtual void	Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UINodePtr& uiNode) = 0;
};

typedef std::shared_ptr<NodeCommand> NodeCommandPtr;
typedef std::shared_ptr<NodeGroupCommand<NodeCommandPtr>> NodeGroupCommandPtr;

class InputSlotCommand : public NodeCommandBase
{
public:
	InputSlotCommand (const std::wstring& name, bool isChecked);
	virtual ~InputSlotCommand ();

	virtual void	Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UIInputSlotPtr& inputSlot) = 0;
};

typedef std::shared_ptr<InputSlotCommand> InputSlotCommandPtr;
typedef std::shared_ptr<NodeGroupCommand<InputSlotCommandPtr>> InputSlotGroupCommandPtr;

class OutputSlotCommand : public NodeCommandBase
{
public:
	OutputSlotCommand (const std::wstring& name, bool isChecked);
	virtual ~OutputSlotCommand ();

	virtual void	Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UIOutputSlotPtr& outputSlot) = 0;
};

typedef std::shared_ptr<OutputSlotCommand> OutputSlotCommandPtr;
typedef std::shared_ptr<NodeGroupCommand<OutputSlotCommandPtr>> OutputSlotGroupCommandPtr;

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
	virtual void EnumerateConnectedNodes (const std::function<void (UINodeConstPtr, UIOutputSlotConstPtr)>& processor) const = 0;
};

class OutputSlotCommandRegistrator
{
public:
	OutputSlotCommandRegistrator ();
	virtual ~OutputSlotCommandRegistrator ();

	virtual void RegisterSlotCommand (OutputSlotCommandPtr slotCommand) = 0;
	virtual void RegisterSlotGroupCommand (OutputSlotGroupCommandPtr nodeGroupCommand) = 0;
	virtual void EnumerateConnectedNodes (const std::function<void (UINodeConstPtr, UIInputSlotConstPtr)>& processor) const = 0;
};

CommandStructure CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode);
CommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot);
CommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot);

}

#endif
