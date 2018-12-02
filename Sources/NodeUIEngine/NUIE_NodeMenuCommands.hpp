#ifndef NUIE_NODEMENUCOMMANDS_HPP
#define NUIE_NODEMENUCOMMANDS_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_MenuCommands.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeParameters.hpp"

namespace NUIE
{

class UndoableCommand : public SingleMenuCommand
{
public:
	UndoableCommand (const std::wstring& name, bool isChecked);
	virtual ~UndoableCommand ();

	virtual bool IsUndoable () const override;
};

class NotUndoableCommand : public SingleMenuCommand
{
public:
	NotUndoableCommand (const std::wstring& name, bool isChecked);
	virtual ~NotUndoableCommand ();

	virtual bool IsUndoable () const override;
};

class DeleteNodesCommand : public UndoableCommand
{
public:
	DeleteNodesCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes);
	virtual ~DeleteNodesCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NE::NodeCollection	relevantNodes;
};

class CopyNodesCommand : public NotUndoableCommand
{
public:
	CopyNodesCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes);
	virtual ~CopyNodesCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NE::NodeCollection	relevantNodes;
};

class PasteNodesCommand : public UndoableCommand
{
public:
	PasteNodesCommand (NodeUIManager& uiManager, const Point& position);
	virtual ~PasteNodesCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	Point				position;
};

class UndoCommand : public NotUndoableCommand
{
public:
	UndoCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment);
	virtual ~UndoCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
};

class RedoCommand : public NotUndoableCommand
{
public:
	RedoCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment);
	virtual ~RedoCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
};

class SetParametersCommand : public NotUndoableCommand
{
public:
	SetParametersCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& currentNode, const NE::NodeCollection& relevantNodes);
	virtual ~SetParametersCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodePtr			currentNode;
	NE::NodeCollection	relevantNodes;
	NodeParameterList	relevantParameters;
};

NE::NodeCollection	GetNodesForCommand (const NodeUIManager& uiManager, const UINodePtr& uiNode);

MenuCommandStructure	CreateEmptyAreaCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position);
MenuCommandStructure	CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode);
MenuCommandStructure	CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot);
MenuCommandStructure	CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot);
MenuCommandStructure	CreateNodeGroupCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group);

}

#endif
