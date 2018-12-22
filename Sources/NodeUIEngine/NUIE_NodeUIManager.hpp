#ifndef NUIE_NODEUIMANAGER_HPP
#define NUIE_NODEUIMANAGER_HPP

#include "NE_NodeManager.hpp"
#include "NE_NodeCollection.hpp"
#include "NUIE_UINode.hpp"
#include "NUIE_UINodeGroup.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_CopyPasteHandler.hpp"
#include "NUIE_UndoHandler.hpp"
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

class NodeUIManager
{
public:
	enum class UpdateMode
	{
		Automatic,
		Manual
	};

	NodeUIManager ();
	virtual ~NodeUIManager ();

	UINodePtr					AddNode (const UINodePtr& uiNode, NE::EvaluationEnv& env);
	bool						DeleteNode (const UINodePtr& uiNode, NE::EvaluationEnv& env);
	bool						DeleteNode (const NE::NodeId& nodeId, NE::EvaluationEnv& env);

	const NE::NodeCollection&	GetSelectedNodes () const;
	void						SetSelectedNodes (const NE::NodeCollection& newSelectedNodes);

	bool						IsOutputSlotConnectedToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;
	bool						CanConnectMoreOutputSlotToInputSlot (const UIInputSlotConstPtr& inputSlot) const;
	bool						CanConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;
	bool						ConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);
	bool						DisconnectOutputSlotFromInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);
	bool						DisconnectAllInputSlotsFromOutputSlot (const UIOutputSlotConstPtr& outputSlot);
	bool						DisconnectAllOutputSlotsFromInputSlot (const UIInputSlotConstPtr& inputSlot);

	bool						HasConnectedInputSlots (const UIOutputSlotConstPtr& outputSlot) const;
	bool						HasConnectedOutputSlots (const UIInputSlotConstPtr& inputSlot) const;
	size_t						GetConnectedInputSlotCount (const UIOutputSlotConstPtr& outputSlot) const;
	size_t						GetConnectedOutputSlotCount (const UIInputSlotConstPtr& inputSlot) const;

	void						EnumerateConnectedInputSlots (const NE::OutputSlotConstPtr& outputSlot, const std::function<void (const NE::InputSlotConstPtr&)>& processor) const;
	void						EnumerateConnectedOutputSlots (const NE::InputSlotConstPtr& inputSlot, const std::function<void (const NE::OutputSlotConstPtr&)>& processor) const;
	void						EnumerateConnectedInputSlots (const UIOutputSlotConstPtr& outputSlot, const std::function<void (UIInputSlotConstPtr)>& processor) const;
	void						EnumerateConnectedOutputSlots (const UIInputSlotConstPtr& inputSlot, const std::function<void (UIOutputSlotConstPtr)>& processor) const;

	UINodePtr					GetUINode (const NE::NodeId& nodeId);
	UINodeConstPtr				GetUINode (const NE::NodeId& nodeId) const;

	void						EnumerateUINodes (const std::function<bool (const UINodePtr&)>& processor);
	void						EnumerateUINodes (const std::function<bool (const UINodeConstPtr&)>& processor) const;

	void						RequestRecalculateAndRedraw ();
	void						RequestRecalculate ();
	void						RequestRedraw ();

	void						InvalidateAllDrawings ();
	void						InvalidateAllNodesDrawing ();
	void						InvalidateAllNodeGroupsDrawing ();
	void						InvalidateNodeValue (const NE::NodeId& nodeId);
	void						InvalidateNodeValue (const UINodePtr& uiNode);
	void						InvalidateNodeDrawing (const NE::NodeId& nodeId);
	void						InvalidateNodeDrawing (const UINodePtr& uiNode);
	void						InvalidateNodeGroupDrawing (const NE::NodeId& nodeId);
	void						InvalidateNodeGroupDrawing (const UINodePtr& uiNode);

	void						Update (NodeUICalculationEnvironment& env);
	void						ManualUpdate (NodeUICalculationEnvironment& env);
	void						Draw (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawingModifier);
	void						ResizeContext (NodeUIDrawingEnvironment& env, int newWidth, int newHeight);

	const ViewBox&				GetViewBox () const;
	void						SetViewBox (const ViewBox& newViewBox);
	bool						IsPreviewMode () const;

	UpdateMode					GetUpdateMode () const;
	void						SetUpdateMode (UpdateMode newUpdateMode);

	void						New ();
	bool						Open (NE::InputStream& inputStream);
	bool						Save (NE::OutputStream& outputStream) const;
	bool						NeedToSave () const;

	bool						CanPaste () const;
	bool						Copy (const NE::NodeCollection& nodeCollection);
	bool						Paste ();

	void						SaveUndoState ();
	bool						Undo (NE::EvaluationEnv& env);
	bool						Redo (NE::EvaluationEnv& env);

	bool						AddUINodeGroup (const UINodeGroupPtr& group);
	void						DeleteUINodeGroup (const UINodeGroupPtr& group);
	bool						RemoveNodesFromGroup (const NE::NodeCollection& nodeCollection);
	void						EnumerateUINodeGroups (const std::function<bool (const UINodeGroupConstPtr&)>& processor) const;
	void						EnumerateUINodeGroups (const std::function<bool (const UINodeGroupPtr&)>& processor);

	void						ExecuteCommand (NodeUIManagerCommand& command);
	void						ExecuteCommand (NodeUIManagerCommandPtr& command);

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

	void				Clear ();
	void				InvalidateDrawingsForInvalidatedNodes ();
	void				UpdateInternal (NodeUICalculationEnvironment& env, InternalUpdateMode mode);

	NE::NodeManager		nodeManager;
	NE::NodeCollection	selectedNodes;
	CopyPasteHandler	copyPasteHandler;
	UndoHandler			undoHandler;
	ViewBox				viewBox;
	mutable Status		status;
};

}

#endif
