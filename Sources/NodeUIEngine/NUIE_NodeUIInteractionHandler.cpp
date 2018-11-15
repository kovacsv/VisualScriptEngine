#include "NUIE_NodeUIInteractionHandler.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UIItemFinder.hpp"
#include "NUIE_UINodeCommandStructure.hpp"
#include "NE_Debug.hpp"

#include <algorithm>

namespace NUIE
{

class PanningHandler : public MouseMoveHandler
{
public:
	PanningHandler (NodeUIManager& uiManager) :
		MouseMoveHandler (),
		uiManager (uiManager)
	{
	
	}

	virtual bool AreOtherHandlersAllowed () const override
	{
		return false;
	}

	virtual void HandleMouseMove (NodeUIEnvironment&, const ModifierKeys&, const Point& position) override
	{
		Point diff = (position - prevPosition);
		ViewBox viewBox = uiManager.GetViewBox ();
		viewBox.SetOffset (viewBox.GetOffset () + diff);
		uiManager.SetViewBox (viewBox);
	}

private:
	NodeUIManager& uiManager;
};

class SelectionRectHandler : public MouseMoveHandler
{
public:
	SelectionRectHandler (NodeUIManager& uiManager) :
		MouseMoveHandler (),
		uiManager (uiManager)
	{
	
	}

	virtual bool AreOtherHandlersAllowed () const override
	{
		return false;
	}

	virtual void HandleMouseMove (NodeUIEnvironment& env, const ModifierKeys&, const Point& position) override
	{
		Point clientPosition = FitMousePositionToClient (env, position);
		selectionRect = Rect::FromTwoPoints (startPosition, clientPosition);
		uiManager.RequestRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point&) override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Rect modelSelectionRect = viewBox.ViewToModel (selectionRect);
		NE::NodeCollection selectedNodes = uiManager.GetSelectedNodes ();
		if (!modifierKeys.Contains (ModifierKeyCode::Control)) {
			selectedNodes.Clear ();
		}
		std::unordered_set<UINodePtr> nodesToSelect;
		uiManager.EnumerateUINodes ([&] (const UINodePtr& uiNode) {
			const NE::NodeId& uiNodeId = uiNode->GetId ();
			Rect nodeRect = uiNode->GetNodeRect (env);
			if (modelSelectionRect.Contains (nodeRect)) {
				if (selectedNodes.Contains (uiNodeId)) {
					selectedNodes.Erase (uiNodeId);
				} else {
					selectedNodes.Insert (uiNodeId);
				}
			}
			return true;
		});
		uiManager.SetSelectedNodes (selectedNodes);
	}

	virtual void HandleAbort () override
	{
		uiManager.RequestRedraw ();
	}

	virtual void EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const override
	{
		processor (selectionRect);
	}

private:
	Point FitMousePositionToClient (NodeUIEnvironment& env, const Point& position)
	{
		Point clientPosition = position;
		clientPosition.SetX (std::max (clientPosition.GetX (), 1.0));
		clientPosition.SetX (std::min (clientPosition.GetX (), env.GetDrawingContext ().GetWidth ()));
		clientPosition.SetY (std::max (clientPosition.GetY (), 1.0));
		clientPosition.SetY (std::min (clientPosition.GetY (), env.GetDrawingContext ().GetHeight ()));
		return clientPosition;
	}

	NodeUIManager&	uiManager;
	Rect			selectionRect;
};

class NodeMovingHandler : public MouseMoveHandler
{
public:
	NodeMovingHandler (NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
		MouseMoveHandler (),
		uiManager (uiManager),
		relevantNodes (relevantNodes)
	{
	
	}

	virtual bool AreOtherHandlersAllowed () const override
	{
		return false;
	}

	virtual void HandleMouseDown (NodeUIEnvironment&, const ModifierKeys&, const Point& position) override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		startModelPosition = viewBox.ViewToModel (position);
	}

	virtual void HandleMouseMove (NodeUIEnvironment&, const ModifierKeys&, const Point&) override
	{
		RequestNodeRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point& position)	override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point diff = viewBox.ViewToModel (position) - startModelPosition;

		uiManager.SaveUndoState ();
		relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			UINodePtr uiNode = uiManager.GetUINode (nodeId);
			uiNode->SetNodePosition (uiNode->GetNodePosition () + diff);
			uiManager.InvalidateNodeGroupDrawing (uiNode);
			return true;
		});

		uiManager.RequestRedraw ();	
	}

	virtual void HandleAbort () override
	{
		RequestNodeRedraw ();
	}

	virtual Point GetNodeOffset (const NE::NodeId& nodeId) const override
	{
		if (relevantNodes.Contains (nodeId)) {
			const ViewBox& viewBox = uiManager.GetViewBox ();
			Point diff = viewBox.ViewToModel (currentPosition) - startModelPosition;
			return diff;
		}
		return Point (0.0, 0.0);
	}

private:
	void RequestNodeRedraw ()
	{
		relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			uiManager.InvalidateNodeGroupDrawing (nodeId);
			return true;
		});
		uiManager.RequestRedraw ();
	}

	NodeUIManager&		uiManager;
	NE::NodeCollection	relevantNodes;
	Point				startModelPosition;
};

template <class StartSlotType, class EndSlotType>
class NodeConnectionHandler : public MouseMoveHandler
{
public:
	NodeConnectionHandler (NodeUIManager& uiManager, StartSlotType startSlot, const Point& sourceNodePosition) :
		MouseMoveHandler (),
		uiManager (uiManager),
		startSlot (startSlot),
		endSlot (nullptr),
		sourceNodePosition (sourceNodePosition)
	{
	
	}

	virtual void HandleMouseDown (NodeUIEnvironment&, const ModifierKeys&, const Point& position) override
	{
		currentPosition = position;
	}

	virtual void HandleAbort () override
	{
		uiManager.RequestRedraw ();
	}

protected:
	NodeUIManager&	uiManager;
	StartSlotType	startSlot;
	EndSlotType		endSlot;
	Point			sourceNodePosition;
	Point			currentPosition;
};

class NodeOutputToInputConnectionHandler : public NodeConnectionHandler<UIOutputSlotConstPtr, UIInputSlotConstPtr>
{
public:
	NodeOutputToInputConnectionHandler (NodeUIManager& uiManager, UIOutputSlotConstPtr startSlot, const Point& sourceNodePosition) :
		NodeConnectionHandler<UIOutputSlotConstPtr, UIInputSlotConstPtr> (uiManager, startSlot, sourceNodePosition)
	{
	
	}

	virtual void EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end)>& processor) const override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point position = viewBox.ViewToModel (currentPosition);
		processor (sourceNodePosition, position);
	}

	virtual void HandleMouseMove (NodeUIEnvironment& env, const ModifierKeys&, const Point& position) override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		currentPosition = position;
		endSlot = FindInputSlotUnderPosition (uiManager, env, currentPosition);
		if (endSlot != nullptr) {
			if (uiManager.CanConnectOutputSlotToInputSlot (startSlot, endSlot)) {
				UINodePtr uiNode = uiManager.GetUINode (endSlot->GetOwnerNodeId ());
				currentPosition = viewBox.ModelToView (uiNode->GetInputSlotConnPosition (env, endSlot->GetId ()));
			} else {
				endSlot = nullptr;
			}
		}
		uiManager.RequestRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point&) override
	{
		if (endSlot != nullptr) {
			uiManager.SaveUndoState ();
			uiManager.ConnectOutputSlotToInputSlot (startSlot, endSlot);
		}
		uiManager.RequestRedraw ();
	}
};

class NodeInputToOutputConnectionHandler : public NodeConnectionHandler<UIInputSlotConstPtr, UIOutputSlotConstPtr>
{
public:
	NodeInputToOutputConnectionHandler (NodeUIManager& uiManager, UIInputSlotConstPtr startSlot, const Point& sourceNodePosition) :
		NodeConnectionHandler<UIInputSlotConstPtr, UIOutputSlotConstPtr> (uiManager, startSlot, sourceNodePosition)
	{
	
	}

	virtual void EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end)>& processor) const override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point position = viewBox.ViewToModel (currentPosition);
		processor (position, sourceNodePosition);
	}

	virtual void HandleMouseMove (NodeUIEnvironment& env, const ModifierKeys&, const Point& position) override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		currentPosition = position;
		endSlot = FindOutputSlotUnderPosition (uiManager, env, currentPosition);
		if (endSlot != nullptr) {
			if (uiManager.CanConnectOutputSlotToInputSlot (endSlot, startSlot)) {
				UINodePtr uiNode = uiManager.GetUINode (endSlot->GetOwnerNodeId ());
				currentPosition = viewBox.ModelToView (uiNode->GetOutputSlotConnPosition (env, endSlot->GetId ()));
			} else {
				endSlot = nullptr;
			}
		}
		uiManager.RequestRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point&) override
	{
		if (endSlot != nullptr) {
			uiManager.SaveUndoState ();
			uiManager.ConnectOutputSlotToInputSlot (endSlot, startSlot);
		}
		uiManager.RequestRedraw ();
	}
};

class NodeUIEventHandlerNotifications : public EventHandlerNotifications
{
public:
	NodeUIEventHandlerNotifications (NodeUIManager& uiManager) :
		uiManager (uiManager)
	{

	}

	virtual void BeforeModification () override
	{
		uiManager.SaveUndoState ();
	}

private:
	NodeUIManager& uiManager;
};

PastePositionCalculator::PastePositionCalculator () :
	lastPastePosition (),
	samePositionPasteCounter (0)
{

}

Point PastePositionCalculator::CalculatePastePosition (NodeUIManager& uiManager, NodeUIEnvironment& env)
{
	static Point pasteOffset (20.0, 20.0);
	const ViewBox& viewBox = uiManager.GetViewBox ();
	const DrawingContext& drawingContext = env.GetDrawingContext ();
	Size screenSize (drawingContext.GetWidth (), drawingContext.GetHeight ());
	Rect screenRect = Rect::FromPositionAndSize (Point (0, 0), screenSize);
	Point modelPastePosition = viewBox.ViewToModel (screenRect.GetCenter ());
	if (modelPastePosition == lastPastePosition) {
		modelPastePosition = modelPastePosition + pasteOffset * samePositionPasteCounter;
		samePositionPasteCounter += 1;
	} else {
		lastPastePosition = modelPastePosition;
		samePositionPasteCounter = 1;
	}
	return modelPastePosition;
}

NodeInputEventHandler::NodeInputEventHandler (NodeUIManager& uiManager, UINodePtr& uiNode) :
	uiManager (uiManager),
	uiNode (uiNode)
{

}

EventHandlerResult NodeInputEventHandler::HandleMouseDragStart (NodeUIEnvironment&, const ModifierKeys&, MouseButton, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeInputEventHandler::HandleMouseDragStop (NodeUIEnvironment&, const ModifierKeys&, MouseButton, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeInputEventHandler::HandleMouseDrag (NodeUIEnvironment&, const ModifierKeys&, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeInputEventHandler::HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	EventHandlerResult handlerResult = EventHandlerResult::EventNotHandled;
	if (uiManager.IsPreviewMode ()) {
		return handlerResult;
	}

	if (DBGVERIFY (uiNode != nullptr)) {
		Point modelPosition = uiManager.GetViewBox ().ViewToModel (position);
		NodeUIEventHandlerNotifications notifications (uiManager);
		handlerResult = uiNode->HandleMouseClick (env, modifierKeys, mouseButton, modelPosition, notifications);
		if (handlerResult == EventHandlerResult::EventHandled) {
			uiManager.RequestRecalculate ();
			uiManager.InvalidateNodeDrawing (uiNode);
		}
	}
	
	return handlerResult;
}

EventHandlerResult NodeInputEventHandler::HandleMouseWheel (NodeUIEnvironment&, const ModifierKeys&, MouseWheelRotation, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeInputEventHandler::HandleKeyPress (NodeUIEnvironment&, const Key&)
{
	return EventHandlerResult::EventNotHandled;
}

NodeUIInteractionHandler::NodeUIInteractionHandler (NodeUIManager& uiManager) :
	InputEventHandler (),
	uiManager (uiManager),
	multiMouseMoveHandler ()
{

}

NodeUIInteractionHandler::~NodeUIInteractionHandler ()
{

}

const NodeDrawingModifier* NodeUIInteractionHandler::GetDrawingModifier ()
{
	return &multiMouseMoveHandler;
}

void NodeUIInteractionHandler::ExecuteCommand (UICommandPtr& command)
{
	if (command->IsUndoable ()) {
		uiManager.SaveUndoState ();
	}
	command->Do ();
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	EventHandlerResult handlerResult = EventHandlerResult::EventNotHandled;
	if (!multiMouseMoveHandler.AreOtherHandlersAllowed ()) {
		return handlerResult;
	}

	if (mouseButton == MouseButton::Left) {
		bool found = FindItemUnderPosition (uiManager, env, position,
			[&] (UINodePtr& foundNode) {
				NE::NodeCollection nodesToMove = GetNodesForCommand (uiManager, foundNode);
				multiMouseMoveHandler.AddHandler (mouseButton, new NodeMovingHandler (uiManager, nodesToMove));
			},
			[&] (UIOutputSlotPtr& foundOutputSlot) {
				UINodePtr uiNode = uiManager.GetUINode (foundOutputSlot->GetOwnerNodeId ());
				Point startNodePosition = uiNode->GetOutputSlotConnPosition (env, foundOutputSlot->GetId ());
				multiMouseMoveHandler.AddHandler (mouseButton, new NodeOutputToInputConnectionHandler (uiManager, foundOutputSlot, startNodePosition));
			},
			[&] (UIInputSlotPtr& foundInputSlot) {
				if (uiManager.CanConnectMoreOutputSlotToInputSlot (foundInputSlot)) {
					UINodePtr uiNode = uiManager.GetUINode (foundInputSlot->GetOwnerNodeId ());
					Point startNodePosition = uiNode->GetInputSlotConnPosition (env, foundInputSlot->GetId ());
					multiMouseMoveHandler.AddHandler (mouseButton, new NodeInputToOutputConnectionHandler (uiManager, foundInputSlot, startNodePosition));
				}
			},
			[&] (UINodeGroupPtr& foundGroup) {
				NE::NodeCollection nodesToMove = foundGroup->GetNodes ();
				multiMouseMoveHandler.AddHandler (mouseButton, new NodeMovingHandler (uiManager, nodesToMove));
			}
		);
		if (!found) {
			multiMouseMoveHandler.AddHandler (mouseButton, new SelectionRectHandler (uiManager));
		}
	} else if (mouseButton == MouseButton::Right) {
		multiMouseMoveHandler.AddHandler (mouseButton, new PanningHandler (uiManager));
	}

	multiMouseMoveHandler.EnumerateHandlers ([&] (const std::shared_ptr<MouseMoveHandler>& handler) {
		handler->OnMouseDown (env, modifierKeys, position);
		handlerResult = EventHandlerResult::EventHandled;
	});

	return handlerResult;
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	if (multiMouseMoveHandler.HasHandler (mouseButton)) {
		multiMouseMoveHandler.GetHandler (mouseButton)->OnMouseUp (env, modifierKeys, position);
		multiMouseMoveHandler.RemoveHandler (mouseButton);
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position)
{
	if (multiMouseMoveHandler.HasHandler ()) {
		multiMouseMoveHandler.EnumerateHandlers ([&] (const std::shared_ptr<MouseMoveHandler>& handler) {
			handler->OnMouseMove (env, modifierKeys, position);
		});
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	EventHandlerResult handlerResult = EventHandlerResult::EventNotHandled;
	if (multiMouseMoveHandler.HasHandler ()) {
		return handlerResult;
	}

	if (mouseButton == MouseButton::Left) {
		NE::NodeCollection selectedNodes;
		UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, position);
		if (foundNode != nullptr) {
			NodeInputEventHandler nodeInputEventHandler (uiManager, foundNode);
			handlerResult = nodeInputEventHandler.HandleMouseClick (env, modifierKeys, mouseButton, position);
			if (handlerResult == EventHandlerResult::EventHandled) {
				return handlerResult;
			}

			const NE::NodeId& foundNodeId = foundNode->GetId ();
			selectedNodes = uiManager.GetSelectedNodes ();
			if (modifierKeys.Contains (ModifierKeyCode::Control)) {
				if (selectedNodes.Contains (foundNodeId)) {
					selectedNodes.Erase (foundNodeId);
				} else {
					selectedNodes.Insert (foundNodeId);
				}
			} else {
				bool onlyThisNodeSelected = (selectedNodes.Count () == 1 && selectedNodes.Contains (foundNodeId));
				selectedNodes.Clear ();
				if (!onlyThisNodeSelected) {
					selectedNodes.Insert (foundNodeId);
				}
			}
		}
		uiManager.SetSelectedNodes (selectedNodes);
		handlerResult = EventHandlerResult::EventHandled;
	} else if (mouseButton == MouseButton::Right) {
		EventHandlers& eventHandlers = env.GetEventHandlers ();
		UICommandPtr selectedCommand;
		bool found = FindItemUnderPosition (uiManager, env, position,
			[&] (UINodePtr& foundNode) {
				UICommandStructure commands = CreateNodeCommandStructure (uiManager, env, foundNode);
				selectedCommand = eventHandlers.OnContextMenu (uiManager, env, position, foundNode, commands);
			},
			[&] (UIOutputSlotPtr& foundOutputSlot) {
				UICommandStructure commands = CreateOutputSlotCommandStructure (uiManager, env, foundOutputSlot);
				selectedCommand = eventHandlers.OnContextMenu (uiManager, env, position, foundOutputSlot, commands);
			},
			[&] (UIInputSlotPtr& foundInputSlot) {
				UICommandStructure commands = CreateInputSlotCommandStructure (uiManager, env, foundInputSlot);
				selectedCommand = eventHandlers.OnContextMenu (uiManager, env, position, foundInputSlot, commands);
			},
			[&] (UINodeGroupPtr& foundGroup) {
				UICommandStructure commands = CreateNodeGroupCommandStructure (uiManager, env, foundGroup);
				selectedCommand = eventHandlers.OnContextMenu (uiManager, env, position, foundGroup, commands);
			}
		);
		if (!found) {
			Point modelPosition = uiManager.GetViewBox ().ViewToModel (position);
			UICommandStructure commands = CreateEmptyAreaCommandStructure (uiManager, env, modelPosition);
			selectedCommand = eventHandlers.OnContextMenu (uiManager, env, position, commands);
		}
		if (selectedCommand != nullptr) {
			ExecuteCommand (selectedCommand);
		}
		handlerResult = EventHandlerResult::EventHandled;
	}

	return handlerResult;
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseWheel (NodeUIEnvironment&, const ModifierKeys&, MouseWheelRotation rotation, const Point& position)
{
	ViewBox viewBox = uiManager.GetViewBox ();
	double scaleRatio = (rotation == MouseWheelRotation::Forward ? 1.1 : 0.9);
	viewBox.SetScale (viewBox.GetScale () * scaleRatio, position);
	uiManager.SetViewBox (viewBox);
	return EventHandlerResult::EventHandled;
}

EventHandlerResult NodeUIInteractionHandler::HandleKeyPress (NodeUIEnvironment& env, const Key& pressedKey)
{
	if (pressedKey.GetKeyCode () == PressedKeyCode::Escape) {
		if (multiMouseMoveHandler.HasHandler ()) {
			multiMouseMoveHandler.AbortHandlers ();
			return EventHandlerResult::EventHandled;
		} else {
			NE::NodeCollection emptySelectedNodes;
			uiManager.SetSelectedNodes (emptySelectedNodes);
		}
		return EventHandlerResult::EventHandled;
	}

	const NE::NodeCollection& selectedNodes = uiManager.GetSelectedNodes ();
	UICommandPtr command = nullptr;

	switch (pressedKey.GetKeyCode ()) {
		case PressedKeyCode::Delete:
			{
				command.reset (new DeleteNodesCommand (uiManager, env, selectedNodes));
			}
			break;
		case PressedKeyCode::Copy:
			{
				command.reset (new CopyNodesCommand (uiManager, selectedNodes));
			}
			break;
		case PressedKeyCode::Paste:
			{
				Point modelPastePosition = pastePositionCalculator.CalculatePastePosition (uiManager, env);
				command.reset (new PasteNodesCommand (uiManager, modelPastePosition));
			}
			break;
		case PressedKeyCode::Undo:
			{
				command.reset (new UndoCommand (uiManager, env));
			}
			break;
		case PressedKeyCode::Redo:
			{
				command.reset (new RedoCommand (uiManager, env));
			}
			break;
		case PressedKeyCode::Escape:
		case PressedKeyCode::Undefined:
			DBGBREAK ();
			break;
	}

	if (command != nullptr) {
		ExecuteCommand (command);
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

}
