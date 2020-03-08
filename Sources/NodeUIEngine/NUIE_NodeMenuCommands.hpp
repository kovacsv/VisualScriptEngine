#ifndef NUIE_NODEMENUCOMMANDS_HPP
#define NUIE_NODEMENUCOMMANDS_HPP

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

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NE::NodeCollection	relevantNodes;
};

class CopyNodesMenuCommand : public SingleMenuCommand
{
public:
	CopyNodesMenuCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes);
	virtual ~CopyNodesMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NE::NodeCollection	relevantNodes;
};

class PasteNodesMenuCommand : public SingleMenuCommand
{
public:
	PasteNodesMenuCommand (NodeUIManager& uiManager, const Point& position);
	virtual ~PasteNodesMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	Point				position;
};

class UndoMenuCommand : public SingleMenuCommand
{
public:
	UndoMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment);
	virtual ~UndoMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
};

class RedoMenuCommand : public SingleMenuCommand
{
public:
	RedoMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment);
	virtual ~RedoMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
};

class SetParametersCommand : public SingleMenuCommand
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

class SetGroupParametersCommand : public SingleMenuCommand
{
public:
	SetGroupParametersCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group);
	virtual ~SetGroupParametersCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodeGroupPtr		group;
};

NE::NodeCollection		GetNodesForCommand (const NodeUIManager& uiManager, const UINodePtr& uiNode);

MenuCommandStructure	CreateEmptyAreaCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position);
MenuCommandStructure	CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode);
MenuCommandStructure	CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotConstPtr& outputSlot);
MenuCommandStructure	CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotConstPtr& inputSlot);
MenuCommandStructure	CreateNodeGroupCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group);

}

#endif
