#ifndef NUIE_NODEUIMANAGER_HPP
#define NUIE_NODEUIMANAGER_HPP

#include "NE_NodeManager.hpp"
#include "NE_NodeCollection.hpp"
#include "NUIE_UINode.hpp"
#include "NUIE_UINodeGroup.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_UINodeInvalidator.hpp"
#include "NUIE_UndoHandler.hpp"
#include "NUIE_Selection.hpp"
#include "NUIE_ViewBox.hpp"

#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace NUIE
{

class NodeUIManager;
class NodeDrawingModifier;

class NodeUIManagerCommand
{
public:
	NodeUIManagerCommand ();
	virtual ~NodeUIManagerCommand ();

	virtual bool IsUndoable () const = 0;
	virtual void Do (NodeUIManager& uiManager) = 0;
};

using NodeUIManagerCommandPtr = std::shared_ptr<NodeUIManagerCommand>;

class NodeUIManagerNodeInvalidator : public UINodeInvalidator
{
public:
	NodeUIManagerNodeInvalidator (NodeUIManager& uiManager, UINodePtr& uiNode);
	virtual ~NodeUIManagerNodeInvalidator ();

	virtual void	InvalidateValue () override;
	virtual void	InvalidateDrawing () override;

	virtual void	RequestRecalculate () override;
	virtual void	RequestRedraw () override;

private:
	NodeUIManager& uiManager;
	UINodePtr& uiNode;
};

class NodeUIManagerNodeRectGetter : public NodeRectGetter
{
public:
	NodeUIManagerNodeRectGetter (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& drawingEnv);
	
	virtual Rect GetNodeRect (const NE::NodeId& nodeId) const override;

private:
	const NodeUIManager& uiManager;
	NodeUIDrawingEnvironment& drawingEnv;
};

class UINodeFilter
{
public:
	UINodeFilter ();
	virtual ~UINodeFilter ();

	virtual bool IsMatch (const UINodeConstPtr& uiNode) const = 0;
};

class UIOutputSlotList : public NE::OutputSlotList
{
public:
	UIOutputSlotList ();
	virtual ~UIOutputSlotList ();
};

class UIInputSlotList : public NE::InputSlotList
{
public:
	UIInputSlotList ();
	virtual ~UIInputSlotList ();
};

class NodeUIManager
{
	SERIALIZABLE;

public:
	enum class UpdateMode
	{
		Automatic,
		Manual
	};

	NodeUIManager (NodeUIEnvironment& uiEnvironment);
	NodeUIManager (const NodeUIManager& src) = delete;
	NodeUIManager (NodeUIManager&& src) = delete;
	~NodeUIManager ();

	NodeUIManager&					operator= (const NodeUIManager& rhs) = delete;
	NodeUIManager&					operator= (NodeUIManager&& rhs) = delete;

	UINodePtr						AddNode (const UINodePtr& uiNode);
	bool							DeleteNode (const UINodePtr& uiNode, NE::EvaluationEnv& evalEnv, NodeUIInteractionEnvironment& interactionEnv);
	bool							DeleteNode (const NE::NodeId& nodeId, NE::EvaluationEnv& evalEnv, NodeUIInteractionEnvironment& interactionEnv);

	const Selection&				GetSelection () const;
	void							SetSelection (const Selection& newSelection, NodeUIInteractionEnvironment& interactionEnv);

	bool							IsOutputSlotConnectedToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;

	bool							CanConnectOutputSlotToInputSlot (const UIInputSlotConstPtr& inputSlot) const;
	bool							CanConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;
	bool							CanConnectOutputSlotsToInputSlot (const UIOutputSlotList& outputSlots, const UIInputSlotConstPtr& inputSlot) const;
	bool							CanConnectOutputSlotToInputSlots (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotList& inputSlots) const;

	bool							ConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);
	bool							ConnectOutputSlotsToInputSlot (const UIOutputSlotList& outputSlots, const UIInputSlotConstPtr& inputSlot);
	bool							ConnectOutputSlotToInputSlots (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotList& inputSlots);

	bool							DisconnectOutputSlotFromInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);
	bool							DisconnectOutputSlotsFromInputSlot (const UIOutputSlotList& outputSlots, const UIInputSlotConstPtr& inputSlot);
	bool							DisconnectOutputSlotFromInputSlots (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotList& inputSlots);
	bool							DisconnectAllInputSlotsFromOutputSlot (const UIOutputSlotConstPtr& outputSlot);
	bool							DisconnectAllOutputSlotsFromInputSlot (const UIInputSlotConstPtr& inputSlot);

	bool							HasConnectedInputSlots (const UIOutputSlotConstPtr& outputSlot) const;
	bool							HasConnectedOutputSlots (const UIInputSlotConstPtr& inputSlot) const;
	size_t							GetConnectedInputSlotCount (const UIOutputSlotConstPtr& outputSlot) const;
	size_t							GetConnectedOutputSlotCount (const UIInputSlotConstPtr& inputSlot) const;

	void							EnumerateConnectedUIInputSlots (const UIOutputSlotConstPtr& outputSlot, const std::function<void (UIInputSlotConstPtr)>& processor) const;
	void							EnumerateConnectedUIOutputSlots (const UIInputSlotConstPtr& inputSlot, const std::function<void (UIOutputSlotConstPtr)>& processor) const;
	void							EnumerateUIConnections (const std::function<void (UIOutputSlotConstPtr, UIInputSlotConstPtr)>& processor) const;
	void							EnumerateUIConnections (const NE::NodeCollection& nodes, const std::function<void (UIOutputSlotConstPtr, UIInputSlotConstPtr)>& processor) const;

	bool							ContainsNode (const NE::NodeId& nodeId) const;
	std::vector<UINodeConstPtr>		FindNodes (const UINodeFilter& nodeFilter) const;
	UINodePtr						GetNode (const NE::NodeId& nodeId);
	UINodeConstPtr					GetNode (const NE::NodeId& nodeId) const;

	void							EnumerateNodes (const std::function<bool (UINodePtr)>& processor);
	void							EnumerateNodes (const std::function<bool (UINodeConstPtr)>& processor) const;

	void							RequestRecalculateAndRedraw ();
	void							RequestRecalculate ();
	void							RequestRedraw ();

	void							InvalidateAllDrawings ();
	void							InvalidateAllNodesDrawing ();
	void							InvalidateAllNodeGroupsDrawing ();
	void							InvalidateNodeValue (const NE::NodeId& nodeId);
	void							InvalidateNodeValue (const UINodePtr& uiNode);
	void							InvalidateNodeDrawing (const NE::NodeId& nodeId);
	void							InvalidateNodeDrawing (const UINodePtr& uiNode);
	void							InvalidateNodeGroupDrawing (const NE::NodeId& nodeId);
	void							InvalidateNodeGroupDrawing (const UINodePtr& uiNode);

	void							Update (NodeUICalculationEnvironment& calcEnv);
	void							ManualUpdate (NodeUICalculationEnvironment& calcEnv);
	void							Draw (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawingModifier);
	void							ResizeContext (NodeUIDrawingEnvironment& drawingEnv, int newWidth, int newHeight);

	bool							GetBoundingRect (NodeUIDrawingEnvironment& drawingEnv, Rect& rect) const;
	void							AlignToWindow (NodeUIDrawingEnvironment& drawingEnv);
	void							CenterToWindow (NodeUIDrawingEnvironment& drawingEnv);
	void							FitToWindow (NodeUIDrawingEnvironment& drawingEnv);

	const ViewBox&					GetViewBox () const;
	void							SetViewBox (const ViewBox& newViewBox);
	bool							IsPreviewMode () const;

	UpdateMode						GetUpdateMode () const;
	void							SetUpdateMode (UpdateMode newUpdateMode);

	void							New (NodeUIEnvironment& uiEnvironment);
	bool							Open (NodeUIEnvironment& uiEnvironment, NE::InputStream& inputStream);
	bool							Save (NE::OutputStream& outputStream);
	bool							NeedToSave () const;

	bool							Copy (const NE::NodeCollection& nodeCollection, NE::NodeManager& result) const;
	NE::NodeCollection				Paste (const NE::NodeManager& source);
	NE::NodeCollection				Duplicate (const NE::NodeCollection& nodeCollection);

	bool							CanUndo () const;
	bool							CanRedo () const;
	void							Undo (NE::EvaluationEnv& evalEnv, NodeUIInteractionEnvironment& interactionEnv);
	void							Redo (NE::EvaluationEnv& evalEnv, NodeUIInteractionEnvironment& interactionEnv);

	UINodeGroupPtr					AddNodeGroup (const UINodeGroupPtr& group);
	void							DeleteNodeGroup (const UINodeGroupPtr& group);
	void							AddNodesToGroup (const UINodeGroupPtr& group, const NE::NodeCollection& nodeCollection);
	bool							RemoveNodesFromGroup (const NE::NodeCollection& nodeCollection);

	const NE::NodeCollection&		GetGroupNodes (const UINodeGroupConstPtr& group) const;
	UINodeGroupConstPtr				GetNodeGroup (const NE::NodeId& nodeId) const;

	void							EnumerateNodeGroups (const std::function<bool (UINodeGroupConstPtr)>& processor) const;
	void							EnumerateNodeGroups (const std::function<bool (UINodeGroupPtr)>& processor);

	void							ExecuteCommand (NodeUIManagerCommand& command, NodeUIInteractionEnvironment& interactionEnv);
	void							ExecuteCommand (NodeUIManagerCommandPtr& command, NodeUIInteractionEnvironment& interactionEnv);

private:
	class Status
	{
	public:
		Status ();

		void	Reset ();

		void	RequestRecalculate ();
		void	ResetRecalculate ();
		bool	NeedToRecalculate () const;

		void	RequestRedraw ();
		void	ResetRedraw ();
		bool	NeedToRedraw () const;

		void	RequestSave ();
		void	ResetSave ();
		bool	NeedToSave () const;

	private:
		bool	needToRecalculate;
		bool	needToRedraw;
		bool	needToSave;
	};

	enum class InternalUpdateMode
	{
		Normal,
		Manual
	};

	void				Clear (NodeUIEnvironment& uiEnvironment);
	void				InvalidateDrawingsForInvalidatedNodes ();
	void				UpdateInternal (NodeUICalculationEnvironment& calcEnv, InternalUpdateMode mode);
	void				HandleSelectionChanged (Selection::ChangeResult changeResult, NodeUIInteractionEnvironment& interactionEnv);
	void				HandleUndoStateChanged (UndoHandler::ChangeResult changeResult, NodeUIInteractionEnvironment& interactionEnv);

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	NE::NodeManager		nodeManager;
	UndoHandler			undoHandler;
	Selection			selection;
	ViewBox				viewBox;
	Status				status;
};

}

#endif
