#ifndef NUIE_NODECOMMONMENUCOMMANDS_HPP
#define NUIE_NODECOMMONMENUCOMMANDS_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_MenuCommands.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeParameters.hpp"

namespace NUIE
{

class DeleteNodesMenuCommand : public SingleMenuCommand
{
public:
	DeleteNodesMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes);
	virtual ~DeleteNodesMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NE::NodeCollection	relevantNodes;
};

class CopyNodesMenuCommand : public SingleMenuCommand
{
public:
	CopyNodesMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes);
	virtual ~CopyNodesMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NE::NodeCollection	relevantNodes;
};

class PasteNodesMenuCommand : public SingleMenuCommand
{
public:
	PasteNodesMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position);
	virtual ~PasteNodesMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	Point				position;
};

class AlignToWindowMenuCommand : public SingleMenuCommand
{
public:
	AlignToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment);
	virtual ~AlignToWindowMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&				uiManager;
	NodeUIDrawingEnvironment&	uiEnvironment;
};

class CenterToWindowMenuCommand : public SingleMenuCommand
{
public:
	CenterToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment);
	virtual ~CenterToWindowMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&				uiManager;
	NodeUIDrawingEnvironment&	uiEnvironment;
};

class FitToWindowMenuCommand : public SingleMenuCommand
{
public:
	FitToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment);
	virtual ~FitToWindowMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&				uiManager;
	NodeUIDrawingEnvironment&	uiEnvironment;
};

class UndoMenuCommand : public SingleMenuCommand
{
public:
	UndoMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment);
	virtual ~UndoMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
};

class RedoMenuCommand : public SingleMenuCommand
{
public:
	RedoMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment);
	virtual ~RedoMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
};

class SetParametersMenuCommand : public SingleMenuCommand
{
public:
	SetParametersMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeConstPtr& currentNode, const NE::NodeCollection& relevantNodes);
	virtual ~SetParametersMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodeConstPtr		currentNode;
	NE::NodeCollection	relevantNodes;
};

class CreateGroupMenuCommand : public SingleMenuCommand
{
public:
	CreateGroupMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes);
	virtual ~CreateGroupMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NE::NodeCollection	relevantNodes;
};

class RemoveNodesFromGroupMenuCommand : public SingleMenuCommand
{
public:
	RemoveNodesFromGroupMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const NE::NodeCollection& relevantNodes);
	virtual ~RemoveNodesFromGroupMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NE::NodeCollection	relevantNodes;
};

class SetGroupParametersMenuCommand : public SingleMenuCommand
{
public:
	SetGroupParametersMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group);
	virtual ~SetGroupParametersMenuCommand ();

	virtual bool WillModify () const override;
	virtual void DoModification () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodeGroupPtr		group;
};

NE::NodeCollection		GetNodesForCommand (const NodeUIManager& uiManager, const UINodeConstPtr& uiNode);

MenuCommandStructure	CreateEmptyAreaCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position);
MenuCommandStructure	CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeConstPtr& uiNode);
MenuCommandStructure	CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotConstPtr& outputSlot);
MenuCommandStructure	CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotConstPtr& inputSlot);
MenuCommandStructure	CreateNodeGroupCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group);

}

#endif
