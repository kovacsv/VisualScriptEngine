#ifndef NUIE_NODEUIMANAGERCOMMANDS_HPP
#define NUIE_NODEUIMANAGERCOMMANDS_HPP

#include "NUIE_NodeUIManager.hpp"

namespace NUIE
{

class UndoableCommand : public NodeUIManagerCommand
{
public:
	UndoableCommand ();
	virtual ~UndoableCommand ();

	virtual bool IsUndoable () const override;
};

class NotUndoableCommand : public NodeUIManagerCommand
{
public:
	NotUndoableCommand ();
	virtual ~NotUndoableCommand ();

	virtual bool IsUndoable () const override;
};

class AddNodeCommand : public UndoableCommand
{
public:
	AddNodeCommand (const UINodePtr& uiNode, NE::EvaluationEnv& evaluationEnv);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UINodePtr&	uiNode;
	NE::EvaluationEnv&	evaluationEnv;
};

class DeleteNodesCommand : public UndoableCommand
{
public:
	DeleteNodesCommand (const NE::NodeCollection& nodes, NE::EvaluationEnv& evaluationEnv);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const NE::NodeCollection&	nodes;
	NE::EvaluationEnv&			evaluationEnv;
};

class MoveNodesCommand : public UndoableCommand
{
public:
	MoveNodesCommand (const NE::NodeCollection& nodes, const Point& offset);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const NE::NodeCollection&	nodes;
	const Point&				offset;
};

class ConnectSlotsCommand : public UndoableCommand
{
public:
	ConnectSlotsCommand (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UIOutputSlotConstPtr&		outputSlot;
	const UIInputSlotConstPtr&		inputSlot;
};

class DisconnectSlotsCommand : public UndoableCommand
{
public:
	DisconnectSlotsCommand (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UIOutputSlotConstPtr&		outputSlot;
	const UIInputSlotConstPtr&		inputSlot;
};

class DisconnectAllInputSlotsCommand : public UndoableCommand
{
public:
	DisconnectAllInputSlotsCommand (const UIOutputSlotConstPtr& outputSlot);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UIOutputSlotConstPtr& outputSlot;
};

class DisconnectAllOutputSlotsCommand : public UndoableCommand
{
public:
	DisconnectAllOutputSlotsCommand (const UIInputSlotConstPtr& inputSlot);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UIInputSlotConstPtr& inputSlot;
};

class CopyNodesCommand : public NotUndoableCommand
{
public:
	CopyNodesCommand (const NE::NodeCollection& nodes);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const NE::NodeCollection& nodes;
};

class PasteNodesCommand : public UndoableCommand
{
public:
	PasteNodesCommand (const Point& position);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const Point& position;
};

class AddGroupCommand : public UndoableCommand
{
public:
	AddGroupCommand (const UINodeGroupPtr& uiGroup, const NE::NodeCollection& nodes);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UINodeGroupPtr& uiGroup;
	const NE::NodeCollection& nodes;
};

class DeleteGroupCommand : public UndoableCommand
{
public:
	DeleteGroupCommand (const UINodeGroupPtr& uiGroup);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UINodeGroupPtr& uiGroup;
};

class RemoveNodesFromGroupCommand : public UndoableCommand
{
public:
	RemoveNodesFromGroupCommand (const NE::NodeCollection& nodes);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const NE::NodeCollection& nodes;
};

class UndoCommand : public NotUndoableCommand
{
public:
	UndoCommand (NE::EvaluationEnv& evaluationEnv);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	NE::EvaluationEnv& evaluationEnv;
};

class RedoCommand : public NotUndoableCommand
{
public:
	RedoCommand (NE::EvaluationEnv& evaluationEnv);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	NE::EvaluationEnv& evaluationEnv;
};

class CustomUndoableCommand : public UndoableCommand
{
public:
	CustomUndoableCommand (const std::function<void ()>& func);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	std::function<void ()> func;
};

}

#endif
