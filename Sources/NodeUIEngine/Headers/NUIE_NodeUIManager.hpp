#ifndef NODEUIMANAGER_HPP
#define NODEUIMANAGER_HPP

#include "NE_NodeManager.hpp"
#include "NUIE_UINode.hpp"
#include "NUIE_UINodeGroup.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeCollection.hpp"
#include "NUIE_CopyPasteHandler.hpp"
#include "NUIE_ViewBox.hpp"

#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace NUIE
{

class NodeDrawingModfier;

class NodeUIManager
{
public:
	NodeUIManager ();
	virtual ~NodeUIManager ();

	UINodePtr				AddNode (const UINodePtr& uiNode, NE::EvaluationEnv& env);
	bool					DeleteNode (const UINodePtr& uiNode, NE::EvaluationEnv& env);
	bool					DeleteNode (const NE::NodeId& nodeId, NE::EvaluationEnv& env);

	const NodeCollection&	GetSelectedNodes () const;
	void					SetSelectedNodes (const NodeCollection& newSelectedNodes);

	bool					IsOutputSlotConnectedToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;
	bool					CanConnectMoreOutputSlotToInputSlot (const UIInputSlotConstPtr& inputSlot) const;
	bool					CanConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;
	bool					ConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);
	bool					DisconnectOutputSlotFromInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);
	bool					DisconnectAllInputSlotsFromOutputSlot (const UIOutputSlotConstPtr& outputSlot);
	bool					DisconnectAllOutputSlotsFromInputSlot (const UIInputSlotConstPtr& inputSlot);

	bool					HasConnectedInputSlots (const UIOutputSlotConstPtr& outputSlot) const;
	bool					HasConnectedOutputSlots (const UIInputSlotConstPtr& inputSlot) const;
	size_t					GetConnectedInputSlotCount (const UIOutputSlotConstPtr& outputSlot) const;
	size_t					GetConnectedOutputSlotCount (const UIInputSlotConstPtr& inputSlot) const;

	void					EnumerateConnections (const std::function<void (const NE::NodeId&, const NE::SlotId&, const NE::NodeId&, const NE::SlotId&)>& processor) const;
	void					EnumerateConnectedInputSlots (const NE::OutputSlotConstPtr& outputSlot, const std::function<void (const NE::InputSlotConstPtr&)>& processor) const;
	void					EnumerateConnectedOutputSlots (const NE::InputSlotConstPtr& inputSlot, const std::function<void (const NE::OutputSlotConstPtr&)>& processor) const;
	void					EnumerateConnectedInputSlots (const UIOutputSlotConstPtr& outputSlot, const std::function<void (UIInputSlotConstPtr)>& processor) const;
	void					EnumerateConnectedOutputSlots (const UIInputSlotConstPtr& inputSlot, const std::function<void (UIOutputSlotConstPtr)>& processor) const;

	UINodePtr				GetUINode (const NE::NodeId& nodeId);
	UINodeConstPtr			GetUINode (const NE::NodeId& nodeId) const;

	void					EnumerateUINodes (const std::function<bool (const UINodePtr&)>& processor);
	void					EnumerateUINodes (const std::function<bool (const UINodeConstPtr&)>& processor) const;

	void					RequestRecalculate ();
	void					RequestRedraw ();

	void					InvalidateAllNodesDrawing ();
	void					InvalidateNodeValue (const NE::NodeId& nodeId);
	void					InvalidateNodeValue (const UINodePtr& uiNode);
	void					InvalidateNodeDrawing (const NE::NodeId& nodeId);
	void					InvalidateNodeDrawing (const UINodePtr& uiNode);
	void					InvalidateNodeGroupDrawing (const NE::NodeId& nodeId);
	void					InvalidateNodeGroupDrawing (const UINodePtr& uiNode);

	void					Update (NodeUICalculationEnvironment& env);
	void					Draw (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawingModifier);
	void					ResizeContext (NodeUIDrawingEnvironment& env, int newWidth, int newHeight);

	const ViewBox&			GetViewBox () const;
	void					SetViewBox (const ViewBox& newViewBox);
	bool					IsPreviewMode () const;

	void					Clear ();
	bool					Load (NE::InputStream& inputStream);
	bool					Save (NE::OutputStream& outputStream) const;

	bool					CanPaste () const;
	bool					Copy (const NodeCollection& nodeCollection);
	bool					Paste ();

	void					EnumerateUINodeGroups (const std::function<bool (const UINodeGroupPtr&)>& processor) const;
	bool					RemoveNodesFromGroup (const NodeCollection& nodeCollection);
	bool					CreateUINodeGroup (const std::wstring& name, const NodeCollection& nodeCollection);
	void					DeleteUINodeGroup (const UINodeGroupPtr& group);

private:
	class Status
	{
	public:
		Status ();

		void	Reset ();

		void	RequestRecalculate ();
		void	RequestRedraw ();

		bool	NeedToRecalculate () const;
		bool	NeedToRedraw () const;

	private:
		bool	needToRecalculate;
		bool	needToRedraw;
	};

	NE::NodeManager		nodeManager;
	UINodeGroupList		nodeGroups;
	NodeCollection		selectedNodes;
	CopyPasteHandler	copyPasteHandler;
	ViewBox				viewBox;
	Status				status;
};

}

#endif
