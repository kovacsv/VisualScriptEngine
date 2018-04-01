#include "NodeUIInteractionHandler.hpp"
#include "NodeUIManager.hpp"
#include "UIItemFinder.hpp"
#include "UINodeCommandStructure.hpp"
#include "Debug.hpp"

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

	virtual void HandleMouseMove (NodeUIEnvironment&, const KeySet&, const Point& position) override
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

	virtual void HandleMouseMove (NodeUIEnvironment& env, const KeySet&, const Point& position) override
	{
		Point clientPosition = FitMousePositionToClient (env, position);
		selectionRect = Rect::FromTwoPoints (startPosition, clientPosition);
		uiManager.RequestRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point&) override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Rect modelSelectionRect = viewBox.ViewToModel (selectionRect);
		NodeCollection selectedNodes = uiManager.GetSelectedNodes ();
		if (!pressedKeys.Contains (KeyCode::Control)) {
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
	NodeMovingHandler (NodeUIManager& uiManager, UINodePtr& currentNode) :
		MouseMoveHandler (),
		uiManager (uiManager),
		currentNode (currentNode)
	{
	
	}

	virtual bool AreOtherHandlersAllowed () const override
	{
		return false;
	}

	virtual void HandleMouseMove (NodeUIEnvironment&, const KeySet&, const Point& position) override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point diff = (position - prevPosition) / viewBox.GetScale ();

		NodeCollection nodesToMove = GetNodesForCommand (uiManager, currentNode);
		nodesToMove.Enumerate ([&] (const NE::NodeId& nodeId) {
			UINodePtr uiNode = uiManager.GetUINode (nodeId);
			uiNode->SetNodePosition (uiNode->GetNodePosition () + diff);
			return true;
		});

		uiManager.RequestRedraw ();
	}

private:
	NodeUIManager&	uiManager;
	UINodePtr		currentNode;
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

	virtual void HandleMouseDown (NodeUIEnvironment&, const KeySet&, const Point& position) override
	{
		currentPosition = position;
	}

protected:
	NodeUIManager&	uiManager;
	StartSlotType	startSlot;
	EndSlotType		endSlot;
	Point			sourceNodePosition;
	Point			currentPosition;
};

class NodeOutputToInputConnectionHandler : public NodeConnectionHandler<UIOutputSlotPtr, UIInputSlotPtr>
{
public:
	NodeOutputToInputConnectionHandler (NodeUIManager& uiManager, UIOutputSlotPtr startSlot, const Point& sourceNodePosition) :
		NodeConnectionHandler<UIOutputSlotPtr, UIInputSlotPtr> (uiManager, startSlot, sourceNodePosition)
	{
	
	}

	virtual void EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end)>& processor) const override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point position = viewBox.ViewToModel (currentPosition);
		processor (sourceNodePosition, position);
	}

	virtual void HandleMouseMove (NodeUIEnvironment& env, const KeySet&, const Point& position) override
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

	virtual void HandleMouseUp (NodeUIEnvironment&, const KeySet&, const Point&) override
	{
		if (endSlot != nullptr) {
			std::vector<UIOutputSlotConstPtr> existingOutputSlots;
			uiManager.EnumerateConnectedOutputSlots (endSlot, [&] (UIOutputSlotConstPtr outputSlot) {
				existingOutputSlots.push_back (outputSlot);
			});
			uiManager.ConnectOutputSlotToInputSlot (startSlot, endSlot);
		}
		uiManager.RequestRedraw ();
	}
};

class NodeInputToOutputConnectionHandler : public NodeConnectionHandler<UIInputSlotPtr, UIOutputSlotPtr>
{
public:
	NodeInputToOutputConnectionHandler (NodeUIManager& uiManager, UIInputSlotPtr startSlot, const Point& sourceNodePosition) :
		NodeConnectionHandler<UIInputSlotPtr, UIOutputSlotPtr> (uiManager, startSlot, sourceNodePosition)
	{
	
	}

	virtual void EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end)>& processor) const override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point position = viewBox.ViewToModel (currentPosition);
		processor (position, sourceNodePosition);
	}

	virtual void HandleMouseMove (NodeUIEnvironment& env, const KeySet&, const Point& position) override
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

	virtual void HandleMouseUp (NodeUIEnvironment&, const KeySet&, const Point&) override
	{
		if (endSlot != nullptr) {
			uiManager.ConnectOutputSlotToInputSlot (endSlot, startSlot);
		}
		uiManager.RequestRedraw ();
	}
};

NodeMouseEventHandler::NodeMouseEventHandler (NodeUIManager& uiManager) :
	uiManager (uiManager)
{

}

EventHandlerResult NodeMouseEventHandler::HandleMouseDragStart (NodeUIEnvironment&, const KeySet&, MouseButton, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeMouseEventHandler::HandleMouseDragStop (NodeUIEnvironment&, const KeySet&, MouseButton, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeMouseEventHandler::HandleMouseDrag (NodeUIEnvironment&, const KeySet&, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeMouseEventHandler::HandleMouseClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position)
{
	EventHandlerResult handlerResult = EventHandlerResult::EventNotHandled;
	if (uiManager.IsPreviewMode ()) {
		return handlerResult;
	}

	const ViewBox& viewBox = uiManager.GetViewBox ();
	UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, position);
	if (foundNode != nullptr) {
		handlerResult = foundNode->HandleMouseClick (env, pressedKeys, mouseButton, viewBox.ViewToModel (position));
		if (handlerResult == EventHandlerResult::EventHandled) {
			uiManager.RequestRecalculate ();
			uiManager.InvalidateNodeDrawing (foundNode);
		}	
	}
	
	return handlerResult;
}

EventHandlerResult NodeMouseEventHandler::HandleMouseWheel (NodeUIEnvironment&, const KeySet&, MouseWheelRotation, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

NodeUIInteractionHandler::NodeUIInteractionHandler (NodeUIManager& uiManager) :
	MouseEventHandler (),
	uiManager (uiManager),
	multiMouseMoveHandler (),
	nodeMouseEventHandler (uiManager)
{

}

NodeUIInteractionHandler::~NodeUIInteractionHandler ()
{

}

const NodeDrawingExtension* NodeUIInteractionHandler::GetDrawingExtension ()
{
	return &multiMouseMoveHandler;
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseDragStart (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position)
{
	EventHandlerResult handlerResult = EventHandlerResult::EventNotHandled;
	if (!multiMouseMoveHandler.AreOtherHandlersAllowed ()) {
		return handlerResult;
	}

	if (mouseButton == MouseButton::Left) {
		bool found = FindItemUnderPosition (uiManager, env, position,
			[&] (UINodePtr& foundNode) {
				multiMouseMoveHandler.AddHandler (mouseButton, new NodeMovingHandler (uiManager, foundNode));
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
			}
		);
		if (!found) {
			multiMouseMoveHandler.AddHandler (mouseButton, new SelectionRectHandler (uiManager));
		}
	} else if (mouseButton == MouseButton::Middle) {
		multiMouseMoveHandler.AddHandler (mouseButton, new PanningHandler (uiManager));
	}

	multiMouseMoveHandler.EnumerateHandlers ([&] (const std::shared_ptr<MouseMoveHandler>& handler) {
		handler->OnMouseDown (env, pressedKeys, position);
		handlerResult = EventHandlerResult::EventHandled;
	});

	return handlerResult;
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseDragStop (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position)
{
	if (multiMouseMoveHandler.HasHandler (mouseButton)) {
		multiMouseMoveHandler.GetHandler (mouseButton)->OnMouseUp (env, pressedKeys, position);
		multiMouseMoveHandler.RemoveHandler (mouseButton);
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseDrag (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position)
{
	if (multiMouseMoveHandler.HasHandler ()) {
		multiMouseMoveHandler.EnumerateHandlers ([&] (const std::shared_ptr<MouseMoveHandler>& handler) {
			handler->OnMouseMove (env, pressedKeys, position);
		});
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position)
{
	EventHandlerResult handlerResult = nodeMouseEventHandler.HandleMouseClick (env, pressedKeys, mouseButton, position);
	if (handlerResult == EventHandlerResult::EventHandled) {
		return handlerResult;
	}

	if (mouseButton == MouseButton::Left) {
		NodeCollection selectedNodes;
		UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, position);
		if (foundNode != nullptr) {
			const NE::NodeId& foundNodeId = foundNode->GetId ();
			selectedNodes = uiManager.GetSelectedNodes ();
			if (pressedKeys.Contains (KeyCode::Control)) {
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
		CommandPtr selectedCommand;
		bool found = FindItemUnderPosition (uiManager, env, position,
			[&] (UINodePtr& foundNode) {
				CommandStructure commands = CreateNodeCommandStructure (uiManager, env, foundNode);
				selectedCommand = eventHandlers.OnContextMenu (uiManager, env, position, foundNode, commands);
			},
			[&] (UIOutputSlotPtr& foundOutputSlot) {
				CommandStructure commands = CreateOutputSlotCommandStructure (uiManager, env, foundOutputSlot);
				selectedCommand = eventHandlers.OnContextMenu (uiManager, env, position, foundOutputSlot, commands);
			},
			[&] (UIInputSlotPtr& foundInputSlot) {
				CommandStructure commands = CreateInputSlotCommandStructure (uiManager, env, foundInputSlot);
				selectedCommand = eventHandlers.OnContextMenu (uiManager, env, position, foundInputSlot, commands);
			}
		);
		if (!found) {
			Point modelPosition = uiManager.GetViewBox ().ViewToModel (position);
			CommandStructure commands = CreateEmptyAreaCommandStructure (uiManager, env, modelPosition);
			selectedCommand = eventHandlers.OnContextMenu (uiManager, env, position, commands);
		}
		if (selectedCommand != nullptr) {
			selectedCommand->Do ();
		}
		handlerResult = EventHandlerResult::EventHandled;
	}

	return handlerResult;
}

EventHandlerResult NodeUIInteractionHandler::HandleMouseWheel (NodeUIEnvironment&, const KeySet&, MouseWheelRotation rotation, const Point& position)
{
	ViewBox viewBox = uiManager.GetViewBox ();
	double scaleRatio = (rotation == MouseWheelRotation::Forward ? 1.1 : 0.9);
	viewBox.SetScale (viewBox.GetScale () * scaleRatio, position);
	uiManager.SetViewBox (viewBox);
	return EventHandlerResult::EventHandled;
}

}
