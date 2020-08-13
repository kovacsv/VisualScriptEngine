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

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	NE::NodeCollection	relevantNodes;
};

class CopyNodesMenuCommand : public SingleMenuCommand
{
public:
	CopyNodesMenuCommand (NodeUIManager& uiManager, NodeUIInteractionEnvironment& interactionEnv, const NE::NodeCollection& relevantNodes);
	virtual ~CopyNodesMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&					uiManager;
	NodeUIInteractionEnvironment&	interactionEnv;
	NE::NodeCollection				relevantNodes;
};

class PasteNodesMenuCommand : public SingleMenuCommand
{
public:
	PasteNodesMenuCommand (NodeUIManager& uiManager, NodeUIInteractionEnvironment& interactionEnv, const Point& position);
	virtual ~PasteNodesMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&					uiManager;
	NodeUIInteractionEnvironment&	interactionEnv;
	Point							position;
};

class AlignToWindowMenuCommand : public SingleMenuCommand
{
public:
	AlignToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment);
	virtual ~AlignToWindowMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&				uiManager;
	NodeUIDrawingEnvironment&	uiEnvironment;
};

class CenterToWindowMenuCommand : public SingleMenuCommand
{
public:
	CenterToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment);
	virtual ~CenterToWindowMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&				uiManager;
	NodeUIDrawingEnvironment&	uiEnvironment;
};

class FitToWindowMenuCommand : public SingleMenuCommand
{
public:
	FitToWindowMenuCommand (NodeUIManager& uiManager, NodeUIDrawingEnvironment& uiEnvironment);
	virtual ~FitToWindowMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&				uiManager;
	NodeUIDrawingEnvironment&	uiEnvironment;
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

class SetParametersMenuCommand : public SingleMenuCommand
{
public:
	SetParametersMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& currentNode, const NE::NodeCollection& relevantNodes);
	virtual ~SetParametersMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	UINodePtr			currentNode;
	NE::NodeCollection	relevantNodes;
};

class CreateGroupMenuCommand : public SingleMenuCommand
{
public:
	CreateGroupMenuCommand (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes);
	virtual ~CreateGroupMenuCommand ();

	virtual void Do () override;

private:
	NodeUIManager&		uiManager;
	NE::NodeCollection	relevantNodes;
};

class SetGroupParametersMenuCommand : public SingleMenuCommand
{
public:
	SetGroupParametersMenuCommand (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodeGroupPtr& group);
	virtual ~SetGroupParametersMenuCommand ();

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
