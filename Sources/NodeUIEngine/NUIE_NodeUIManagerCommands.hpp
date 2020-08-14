#ifndef NUIE_NODEUIMANAGERCOMMANDS_HPP
#define NUIE_NODEUIMANAGERCOMMANDS_HPP

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_ParameterInterface.hpp"
#include "NUIE_ClipboardHandler.hpp"

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
	Point						offset;
};

class MoveNodesWithOffsetsCommand : public UndoableCommand
{
public:
	MoveNodesWithOffsetsCommand (const NE::NodeCollection& nodes, const std::vector<Point>& offsets);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const NE::NodeCollection&	nodes;
	const std::vector<Point>&	offsets;
};

class CopyMoveNodesCommand : public UndoableCommand
{
public:
	CopyMoveNodesCommand (const NE::NodeCollection& nodes, const Point& offset);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const NE::NodeCollection&	nodes;
	Point						offset;
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

class ReconnectInputSlotCommand : public UndoableCommand
{
public:
	ReconnectInputSlotCommand (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& oldInputSlot, const UIInputSlotConstPtr& newInputSlot);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UIOutputSlotConstPtr&		outputSlot;
	const UIInputSlotConstPtr&		oldInputSlot;
	const UIInputSlotConstPtr&		newInputSlot;
};

class ReconnectOutputSlotCommand : public UndoableCommand
{
public:
	ReconnectOutputSlotCommand (const UIOutputSlotConstPtr& oldOutputSlot, const UIOutputSlotConstPtr& newOutputSlot, const UIInputSlotConstPtr& inputSlot);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UIOutputSlotConstPtr&		oldOutputSlot;
	const UIOutputSlotConstPtr&		newOutputSlot;
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
	CopyNodesCommand (const NE::NodeCollection& nodes, ClipboardHandler& clipboard);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const NE::NodeCollection&	nodes;
	ClipboardHandler&			clipboard;
};

class PasteNodesCommand : public UndoableCommand
{
public:
	PasteNodesCommand (const Point& position, ClipboardHandler& clipboard);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const Point&		position;
	ClipboardHandler&	clipboard;
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

class AddNodesToGroupCommand : public UndoableCommand
{
public:
	AddNodesToGroupCommand (const UINodeGroupPtr& uiGroup, const NE::NodeCollection& nodes);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const UINodeGroupPtr& uiGroup;
	const NE::NodeCollection& nodes;
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

class ApplyParametersCommand : public UndoableCommand
{
public:
	ApplyParametersCommand (const ParameterInterfacePtr& paramInterface, NE::EvaluationEnv& evaluationEnv);

	virtual void Do (NodeUIManager& uiManager) override;

private:
	const ParameterInterfacePtr&	paramInterface;
	NE::EvaluationEnv&				evaluationEnv;
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
