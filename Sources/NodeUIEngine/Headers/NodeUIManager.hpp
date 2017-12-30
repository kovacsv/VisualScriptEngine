#ifndef NODEUIMANAGER_HPP
#define NODEUIMANAGER_HPP

#include "NodeManager.hpp"
#include "NodeUIEnvironment.hpp"
#include "NodeUIInteractionHandler.hpp"
#include "NodeDrawingExtension.hpp"
#include "ViewBox.hpp"
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace NUIE
{

class NodeUIManager
{
public:
	class SelectedNodes
	{
	public:
		SelectedNodes ();
		SelectedNodes (const std::unordered_set<NE::NodeId>& selection);
		~SelectedNodes ();

		bool	Contains (const NE::NodeId& nodeId) const;
		size_t	Count () const;

		void	Enumerate (const std::function<void (const NE::NodeId&)>& processor) const;
		void	Insert (const NE::NodeId& nodeId);
		void	Erase (const NE::NodeId& nodeId);
		void	Clear ();

	private:
		std::unordered_set<NE::NodeId> selection;
	};

	NodeUIManager ();
	virtual ~NodeUIManager ();

	UINodePtr				AddNode (const UINodePtr& uiNode, NE::EvaluationEnv& env);
	bool					DeleteNode (const UINodePtr& uiNode, NE::EvaluationEnv& env);
	bool					DeleteNode (const NE::NodeId& nodeId, NE::EvaluationEnv& env);

	const SelectedNodes&	GetSelectedNodes () const;
	void					SetSelectedNodes (const SelectedNodes& newSelectedNodes);

	bool					IsOutputSlotConnectedToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;
	bool					CanConnectMoreOutputSlotToInputSlot (const UIInputSlotConstPtr& inputSlot) const;
	bool					CanConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;
	bool					ConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);
	bool					DisconnectOutputSlotFromInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);

	bool					HasConnectedOutputSlots (const UIInputSlotConstPtr& inputSlot) const;
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
	void					InvalidateNodeDrawing (const NE::NodeId& nodeId);
	void					InvalidateNodeDrawing (const UINodePtr& uiNode);

	void					Update (NodeUIEnvironment& env);
	void					Draw (NodeUIEnvironment& env);
	void					ResizeContext (NodeUIEnvironment& env, int newWidth, int newHeight);

	const ViewBox&			GetViewBox () const;
	void					SetViewBox (const ViewBox& newViewBox);
	bool					IsPreviewMode () const;

	void					OnMouseDown (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position);
	void					OnMouseUp (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position);
	void					OnMouseMove (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position);
	void					OnMouseWheel (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseWheelRotation rotation, const Point& position);
	void					OnMouseDoubleClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position);

	void					Clear ();
	bool					Load (NE::InputStream& inputStream);
	bool					Save (NE::OutputStream& outputStream) const;

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

	NE::NodeManager				nodeManager;
	ViewBox						viewBox;

	NodeUIInteractionHandler	interactionHandler;
	SelectedNodes				selectedNodes;
	Status						status;
};

}

#endif
