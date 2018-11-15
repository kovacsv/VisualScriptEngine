#ifndef NUIE_UINODECOMMANDSTRUCTURE_HPP
#define NUIE_UINODECOMMANDSTRUCTURE_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_CommandStructure.hpp"
#include "NUIE_NodeUIManager.hpp"

namespace NUIE
{

class UndoableCommand : public SingleUICommand
{
public:
	UndoableCommand (const std::wstring& name, bool isChecked);
	virtual ~UndoableCommand ();

	virtual bool IsUndoable () const override;
};

class NotUndoableCommand : public SingleUICommand
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

NE::NodeCollection	GetNodesForCommand (const NodeUIManager& uiManager, const UINodePtr& uiNode);

UICommandStructure	CreateEmptyAreaCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position);
UICommandStructure	CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode);
UICommandStructure	CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot);
UICommandStructure	CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot);
UICommandStructure	CreateNodeGroupCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group);

}

#endif
