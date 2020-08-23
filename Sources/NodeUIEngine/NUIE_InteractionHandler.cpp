#include "NUIE_InteractionHandler.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UIItemFinder.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_NodeCommonMenuCommands.hpp"
#include "NE_Debug.hpp"

#include <algorithm>

namespace NUIE
{

static void EnumerateInternalConnections (NodeUIDrawingEnvironment& drawingEnv, const NodeUIManager& uiManager, const NE::NodeCollection& nodes, const std::function<void (const Point&, const Point&)>& processor)
{
	nodes.Enumerate ([&] (const NE::NodeId& nodeId) {
		UINodeConstPtr inputNode = uiManager.GetUINode (nodeId);
		inputNode->EnumerateUIInputSlots ([&] (const UIInputSlotConstPtr& inputSlot) {
			NE::SlotInfo inputSlotInfo (inputSlot->GetOwnerNodeId (), inputSlot->GetId ());
			uiManager.EnumerateConnectedUIOutputSlots (inputSlot, [&] (const UIOutputSlotConstPtr outputSlot) {
				if (nodes.Contains (outputSlot->GetOwnerNodeId ())) {
					UINodeConstPtr outputNode = uiManager.GetUINode (outputSlot->GetOwnerNodeId ());
					processor (
						outputNode->GetOutputSlotConnPosition (drawingEnv, outputSlot->GetId ()),
						inputNode->GetInputSlotConnPosition (drawingEnv, inputSlot->GetId ())
					);
				}
			});
			return true;
		});
		return true;
	});
}

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

	virtual void HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position) override
	{
		Point clientPosition = FitMousePositionToClient (uiEnvironment, position);
		selectionRect = Rect::FromTwoPoints (startPosition, clientPosition);
		uiManager.RequestRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, const Point&) override
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
			Rect nodeRect = uiNode->GetNodeRect (uiEnvironment);
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
	Point FitMousePositionToClient (NodeUIEnvironment& uiEnvironment, const Point& position)
	{
		Point clientPosition = position;
		clientPosition.SetX (std::max (clientPosition.GetX (), 1.0));
		clientPosition.SetX (std::min (clientPosition.GetX (), (double) uiEnvironment.GetDrawingContext ().GetWidth ()));
		clientPosition.SetY (std::max (clientPosition.GetY (), 1.0));
		clientPosition.SetY (std::min (clientPosition.GetY (), (double) uiEnvironment.GetDrawingContext ().GetHeight ()));
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
		RequestRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point& position)	override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point offset = viewBox.ViewToModel (position) - startModelPosition;

		MoveNodesCommand command (relevantNodes, offset);
		uiManager.ExecuteCommand (command);

		uiManager.RequestRedraw ();
	}

	virtual void HandleAbort () override
	{
		RequestRedraw ();
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
	void RequestRedraw ()
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

class NodeCopyMovingHandler : public MouseMoveHandler
{
public:
	NodeCopyMovingHandler (NodeUIDrawingEnvironment& drawingEnv, NodeUIManager& uiManager, const NE::NodeCollection& relevantNodes) :
		MouseMoveHandler (),
		uiManager (uiManager),
		relevantNodes (relevantNodes)
	{
		EnumerateInternalConnections (drawingEnv, uiManager, relevantNodes, [&] (const Point& beg, const Point& end) {
			temporaryConnections.push_back ({ beg, end });
		});
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
		RequestRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point& position)	override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point offset = viewBox.ViewToModel (position) - startModelPosition;

		CopyMoveNodesCommand command (relevantNodes, offset);
		uiManager.ExecuteCommand (command);

		uiManager.RequestRedraw ();
	}

	virtual void HandleAbort () override
	{
		RequestRedraw ();
	}

	virtual void EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&)>& processor) const override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point diff = viewBox.ViewToModel (currentPosition) - startModelPosition;
		for (const auto& connection : temporaryConnections) {
			processor (connection.first + diff, connection.second + diff);
		}
	}

	virtual void EnumerateDuplicatedNodes (const std::function<void (const NE::NodeId&, const Point&)>& processor) const override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point diff = viewBox.ViewToModel (currentPosition) - startModelPosition;
		relevantNodes.Enumerate ([&] (const NE::NodeId& nodeId) {
			processor (nodeId, diff);
			return true;
		});
	}

private:
	void RequestRedraw ()
	{
		uiManager.RequestRedraw ();
	}

	NodeUIManager&							uiManager;
	NE::NodeCollection						relevantNodes;
	Point									startModelPosition;
	std::vector<std::pair<Point, Point>>	temporaryConnections;
};

template <class StartSlotType, class EndSlotType>
class NodeConnectionHandler : public MouseMoveHandler
{
public:
	NodeConnectionHandler (NodeUIManager& uiManager, const StartSlotType& startSlot, const Point& startSlotPosition) :
		MouseMoveHandler (),
		uiManager (uiManager),
		startSlot (startSlot),
		endSlot (nullptr),
		startSlotPosition (startSlotPosition)
	{
	
	}

	virtual void HandleAbort () override
	{
		uiManager.RequestRedraw ();
	}

protected:
	NodeUIManager&	uiManager;
	StartSlotType	startSlot;
	EndSlotType		endSlot;
	Point			startSlotPosition;
};

class NodeOutputToInputConnectionHandler : public NodeConnectionHandler<UIOutputSlotConstPtr, UIInputSlotConstPtr>
{
public:
	NodeOutputToInputConnectionHandler (NodeUIManager& uiManager, const UIOutputSlotConstPtr& startSlot, const Point& startSlotPosition) :
		NodeConnectionHandler<UIOutputSlotConstPtr, UIInputSlotConstPtr> (uiManager, startSlot, startSlotPosition)
	{
	
	}

	virtual void EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end)>& processor) const override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point position = viewBox.ViewToModel (currentPosition);
		processor (startSlotPosition, position);
	}

	virtual void HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position) override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		currentPosition = position;
		endSlot = FindInputSlotUnderPosition (uiManager, uiEnvironment, currentPosition);
		if (endSlot != nullptr) {
			if (SnapToInputSlot (startSlot, endSlot)) {
				UINodePtr uiNode = uiManager.GetUINode (endSlot->GetOwnerNodeId ());
				currentPosition = viewBox.ModelToView (uiNode->GetInputSlotConnPosition (uiEnvironment, endSlot->GetId ()));
			} else {
				endSlot = nullptr;
			}
		}
		uiManager.RequestRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point&) override
	{
		if (endSlot != nullptr) {
			ConnectSlotsCommand command (startSlot, endSlot);
			uiManager.ExecuteCommand (command);
		} else {
			uiManager.RequestRedraw ();
		}
	}

	virtual bool SnapToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
	{
		return uiManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot);
	}
};

class NodeOutputToInputReconnectionHandler : public NodeOutputToInputConnectionHandler
{
public:
	NodeOutputToInputReconnectionHandler (NodeUIManager& uiManager, const UIOutputSlotConstPtr& startSlot, const UIInputSlotConstPtr& originalEndSlot, const Point& startSlotPosition) :
		NodeOutputToInputConnectionHandler (uiManager, startSlot, startSlotPosition),
		originalEndSlot (originalEndSlot)
	{

	}

	virtual bool NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const override
	{
		if (outputNodeId == startSlot->GetOwnerNodeId () &&
			inputNodeId == originalEndSlot->GetOwnerNodeId () &&
			outputSlotId == startSlot->GetId () &&
			inputSlotId == originalEndSlot->GetId ())
		{
			return false;
		}
		return true;
	}

	virtual void HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point&) override
	{
		if (endSlot != nullptr && endSlot != originalEndSlot) {
			ReconnectInputSlotCommand command (startSlot, originalEndSlot, endSlot);
			uiManager.ExecuteCommand (command);
		} else if (endSlot == nullptr) {
			DisconnectSlotsCommand disconnectCommand (startSlot, originalEndSlot);
			uiManager.ExecuteCommand (disconnectCommand);
		} else {
			uiManager.RequestRedraw ();
		}
	}

	virtual bool SnapToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const override
	{
		return uiManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot) || inputSlot == originalEndSlot;
	}

private:
	UIInputSlotConstPtr originalEndSlot;
};

class NodeInputToOutputConnectionHandler : public NodeConnectionHandler<UIInputSlotConstPtr, UIOutputSlotConstPtr>
{
public:
	NodeInputToOutputConnectionHandler (NodeUIManager& uiManager, const UIInputSlotConstPtr& startSlot, const Point& startSlotPosition) :
		NodeConnectionHandler<UIInputSlotConstPtr, UIOutputSlotConstPtr> (uiManager, startSlot, startSlotPosition)
	{
	
	}

	virtual void EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end)>& processor) const override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point position = viewBox.ViewToModel (currentPosition);
		processor (position, startSlotPosition);
	}

	virtual void HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position) override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		currentPosition = position;
		endSlot = FindOutputSlotUnderPosition (uiManager, uiEnvironment, currentPosition);
		if (endSlot != nullptr) {
			if (SnapToOutputSlot (endSlot, startSlot)) {
				UINodePtr uiNode = uiManager.GetUINode (endSlot->GetOwnerNodeId ());
				currentPosition = viewBox.ModelToView (uiNode->GetOutputSlotConnPosition (uiEnvironment, endSlot->GetId ()));
			} else {
				endSlot = nullptr;
			}
		}
		uiManager.RequestRedraw ();
	}

	virtual void HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point&) override
	{
		if (endSlot != nullptr) {
			ConnectSlotsCommand command (endSlot, startSlot);
			uiManager.ExecuteCommand (command);
		} else {
			uiManager.RequestRedraw ();
		}
	}

	virtual bool SnapToOutputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
	{
		return uiManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot);
	}
};

class NodeInputToOutputReconnectionHandler : public NodeInputToOutputConnectionHandler
{
public:
	NodeInputToOutputReconnectionHandler (NodeUIManager& uiManager, const UIInputSlotConstPtr& startSlot, const UIOutputSlotConstPtr& originalEndSlot, const Point& startSlotPosition) :
		NodeInputToOutputConnectionHandler (uiManager, startSlot, startSlotPosition),
		originalEndSlot (originalEndSlot)
	{

	}

	virtual bool NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const override
	{
		if (outputNodeId == originalEndSlot->GetOwnerNodeId () &&
			inputNodeId == startSlot->GetOwnerNodeId () &&
			outputSlotId == originalEndSlot->GetId () &&
			inputSlotId == startSlot->GetId ()) {
			return false;
		}
		return true;
	}

	virtual void HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point&) override
	{
		if (endSlot != nullptr && endSlot != originalEndSlot) {
			ReconnectOutputSlotCommand command (originalEndSlot, endSlot, startSlot);
			uiManager.ExecuteCommand (command);
		} else if (endSlot == nullptr) {
			DisconnectSlotsCommand disconnectCommand (originalEndSlot, startSlot);
			uiManager.ExecuteCommand (disconnectCommand);
		} else {
			uiManager.RequestRedraw ();
		}
	}

	virtual bool SnapToOutputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const override
	{
		return uiManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot) || outputSlot == originalEndSlot;
	}

private:
	UIOutputSlotConstPtr originalEndSlot;
};

class UINodeInteractionCommandInterface : public UINodeCommandInterface
{
public:
	UINodeInteractionCommandInterface (NodeUIManager& uiManager) :
		UINodeCommandInterface (),
		uiManager (uiManager)
	{

	}

	virtual void RunUndoableCommand (const std::function<void ()>& func) override
	{
		CustomUndoableCommand command (func);
		uiManager.ExecuteCommand (command);
	}

private:
	NodeUIManager& uiManager;
};

PastePositionCalculator::PastePositionCalculator () :
	lastPastePosition (),
	samePositionPasteCounter (0)
{

}

Point PastePositionCalculator::CalculatePastePosition (const NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment)
{
	static const Point pasteOffset (20.0, 20.0);
	const ViewBox& viewBox = uiManager.GetViewBox ();
	const DrawingContext& drawingContext = uiEnvironment.GetDrawingContext ();
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

EventHandlerResult NodeInputEventHandler::HandleMouseClick (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	return ForwardEventToNode ([&] () {
		UINodeInteractionCommandInterface commandInterface (uiManager);
		Point modelPosition = uiManager.GetViewBox ().ViewToModel (position);
		return uiNode->HandleMouseClick (uiEnvironment, modifierKeys, mouseButton, modelPosition, commandInterface);
	});
}

EventHandlerResult NodeInputEventHandler::HandleMouseDoubleClick (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	return ForwardEventToNode ([&] () {
		UINodeInteractionCommandInterface commandInterface (uiManager);
		Point modelPosition = uiManager.GetViewBox ().ViewToModel (position);
		return uiNode->HandleMouseDoubleClick (uiEnvironment, modifierKeys, mouseButton, modelPosition, commandInterface);
	});
}

EventHandlerResult NodeInputEventHandler::HandleMouseWheel (NodeUIEnvironment&, const ModifierKeys&, MouseWheelRotation, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeInputEventHandler::HandleKeyPress (NodeUIEnvironment&, const Key&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeInputEventHandler::ForwardEventToNode (const std::function<EventHandlerResult ()>& forwardEvent)
{
	EventHandlerResult handlerResult = EventHandlerResult::EventNotHandled;
	if (uiManager.IsPreviewMode ()) {
		return handlerResult;
	}

	if (DBGVERIFY (uiNode != nullptr)) {
		handlerResult = forwardEvent ();
		if (handlerResult == EventHandlerResult::EventHandled) {
			uiManager.InvalidateNodeValue (uiNode);
			uiManager.InvalidateNodeDrawing (uiNode);
		}
	}

	return handlerResult;
}

InteractionHandler::InteractionHandler (NodeUIManager& uiManager) :
	InputEventHandler (),
	uiManager (uiManager),
	multiMouseMoveHandler ()
{

}

InteractionHandler::~InteractionHandler ()
{

}

const NodeDrawingModifier* InteractionHandler::GetDrawingModifier ()
{
	return &multiMouseMoveHandler;
}

EventHandlerResult InteractionHandler::HandleMouseDragStart (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	EventHandlerResult handlerResult = EventHandlerResult::EventNotHandled;
	if (!multiMouseMoveHandler.AreOtherHandlersAllowed ()) {
		return handlerResult;
	}

	if (mouseButton == MouseButton::Left) {
		bool found = FindItemUnderPosition (uiManager, uiEnvironment, position,
			[&] (const UINodePtr& foundNode) {
				NE::NodeCollection nodesToMove = GetNodesForCommand (uiManager, foundNode);
				if (modifierKeys.Contains (ModifierKeyCode::Control)) {
					multiMouseMoveHandler.AddHandler (mouseButton, new NodeCopyMovingHandler (uiEnvironment, uiManager, nodesToMove));
				} else {
					multiMouseMoveHandler.AddHandler (mouseButton, new NodeMovingHandler (uiManager, nodesToMove));
				}
			},
			[&] (const UIOutputSlotConstPtr& foundOutputSlot) {
				if (modifierKeys.Contains (ModifierKeyCode::Control)) {
					if (uiManager.GetConnectedInputSlotCount (foundOutputSlot) == 1) {
						UIInputSlotConstPtr foundInputSlot = nullptr;
						uiManager.EnumerateConnectedUIInputSlots (foundOutputSlot, [&] (const UIInputSlotConstPtr& inputSlot) {
							foundInputSlot = inputSlot;
							return true;
						});
						if (DBGVERIFY (foundInputSlot != nullptr)) {
							UINodeConstPtr inputNode = uiManager.GetUINode (foundInputSlot->GetOwnerNodeId ());
							Point startSlotPosition = inputNode->GetInputSlotConnPosition (uiEnvironment, foundInputSlot->GetId ());
							multiMouseMoveHandler.AddHandler (mouseButton, new NodeInputToOutputReconnectionHandler (uiManager, foundInputSlot, foundOutputSlot, startSlotPosition));
						}
					}
				} else {
					UINodeConstPtr uiNode = uiManager.GetUINode (foundOutputSlot->GetOwnerNodeId ());
					Point startSlotPosition = uiNode->GetOutputSlotConnPosition (uiEnvironment, foundOutputSlot->GetId ());
					multiMouseMoveHandler.AddHandler (mouseButton, new NodeOutputToInputConnectionHandler (uiManager, foundOutputSlot, startSlotPosition));
				}
			},
			[&] (const UIInputSlotConstPtr& foundInputSlot) {
				if (modifierKeys.Contains (ModifierKeyCode::Control)) {
					if (uiManager.GetConnectedOutputSlotCount (foundInputSlot) == 1) {
						UIOutputSlotConstPtr foundOutputSlot = nullptr;
						uiManager.EnumerateConnectedUIOutputSlots (foundInputSlot, [&] (const UIOutputSlotConstPtr& outputSlot) {
							foundOutputSlot = outputSlot;
							return true;
						});
						if (DBGVERIFY (foundOutputSlot != nullptr)) {
							UINodeConstPtr outputNode = uiManager.GetUINode (foundOutputSlot->GetOwnerNodeId ());
							Point startSlotPosition = outputNode->GetOutputSlotConnPosition (uiEnvironment, foundOutputSlot->GetId ());
							multiMouseMoveHandler.AddHandler (mouseButton, new NodeOutputToInputReconnectionHandler (uiManager, foundOutputSlot, foundInputSlot, startSlotPosition));
						}
					}
				} else {
					if (uiManager.CanConnectMoreOutputSlotToInputSlot (foundInputSlot)) {
						UINodeConstPtr uiNode = uiManager.GetUINode (foundInputSlot->GetOwnerNodeId ());
						Point startSlotPosition = uiNode->GetInputSlotConnPosition (uiEnvironment, foundInputSlot->GetId ());
						multiMouseMoveHandler.AddHandler (mouseButton, new NodeInputToOutputConnectionHandler (uiManager, foundInputSlot, startSlotPosition));
					}
				}
			},
			[&] (const UINodeGroupPtr& foundGroup) {
				NE::NodeCollection nodesToMove = uiManager.GetUIGroupNodes (foundGroup);
				multiMouseMoveHandler.AddHandler (mouseButton, new NodeMovingHandler (uiManager, nodesToMove));
			}
		);
		if (!found) {
			multiMouseMoveHandler.AddHandler (mouseButton, new SelectionRectHandler (uiManager));
		}
	} else if (mouseButton == MouseButton::Middle || mouseButton == MouseButton::Right) {
		multiMouseMoveHandler.AddHandler (mouseButton, new PanningHandler (uiManager));
	}

	multiMouseMoveHandler.EnumerateHandlers ([&] (const std::shared_ptr<MouseMoveHandler>& handler) {
		handler->OnMouseDown (uiEnvironment, modifierKeys, position);
		handlerResult = EventHandlerResult::EventHandled;
	});

	return handlerResult;
}

EventHandlerResult InteractionHandler::HandleMouseDragStop (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	if (multiMouseMoveHandler.HasHandler (mouseButton)) {
		multiMouseMoveHandler.GetHandler (mouseButton)->OnMouseUp (uiEnvironment, modifierKeys, position);
		multiMouseMoveHandler.RemoveHandler (mouseButton);
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult InteractionHandler::HandleMouseDrag (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, const Point& position)
{
	if (multiMouseMoveHandler.HasHandler ()) {
		multiMouseMoveHandler.EnumerateHandlers ([&] (const std::shared_ptr<MouseMoveHandler>& handler) {
			handler->OnMouseMove (uiEnvironment, modifierKeys, position);
		});
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult InteractionHandler::HandleMouseClick (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	EventHandlerResult handlerResult = EventHandlerResult::EventNotHandled;
	if (multiMouseMoveHandler.HasHandler ()) {
		return handlerResult;
	}

	UINodePtr foundNode = FindNodeUnderPosition (uiManager, uiEnvironment, position);
	if (foundNode != nullptr) {
		NodeInputEventHandler nodeInputEventHandler (uiManager, foundNode);
		handlerResult = nodeInputEventHandler.HandleMouseClick (uiEnvironment, modifierKeys, mouseButton, position);
		if (handlerResult == EventHandlerResult::EventHandled) {
			return handlerResult;
		}
	}

	if (mouseButton == MouseButton::Left) {
		NE::NodeCollection selectedNodes;
		if (foundNode != nullptr) {
			const NE::NodeId& foundNodeId = foundNode->GetId ();
			selectedNodes = uiManager.GetSelectedNodes ();
			if (modifierKeys.Contains (ModifierKeyCode::Control)) {
				if (selectedNodes.Contains (foundNodeId)) {
					selectedNodes.Erase (foundNodeId);
				} else {
					selectedNodes.Insert (foundNodeId);
				}
			} else {
				selectedNodes.Clear ();
				selectedNodes.Insert (foundNodeId);
			}
		}
		uiManager.SetSelectedNodes (selectedNodes);
		handlerResult = EventHandlerResult::EventHandled;
	} else if (mouseButton == MouseButton::Right) {
		EventHandler& eventHandler = uiEnvironment.GetEventHandler ();
		MenuCommandPtr selectedCommand;
		bool found = FindItemUnderPosition (uiManager, uiEnvironment, position,
			[&] (const UINodePtr& foundNode) {
				MenuCommandStructure commands = CreateNodeCommandStructure (uiManager, uiEnvironment, foundNode);
				selectedCommand = eventHandler.OnContextMenu (position, foundNode, commands);
			},
			[&] (const UIOutputSlotConstPtr& foundOutputSlot) {
				MenuCommandStructure commands = CreateOutputSlotCommandStructure (uiManager, uiEnvironment, foundOutputSlot);
				selectedCommand = eventHandler.OnContextMenu (position, foundOutputSlot, commands);
			},
			[&] (const UIInputSlotConstPtr& foundInputSlot) {
				MenuCommandStructure commands = CreateInputSlotCommandStructure (uiManager, uiEnvironment, foundInputSlot);
				selectedCommand = eventHandler.OnContextMenu (position, foundInputSlot, commands);
			},
			[&] (const UINodeGroupPtr& foundGroup) {
				MenuCommandStructure commands = CreateNodeGroupCommandStructure (uiManager, uiEnvironment, foundGroup);
				selectedCommand = eventHandler.OnContextMenu (position, foundGroup, commands);
			}
		);
		if (!found) {
			Point modelPosition = uiManager.GetViewBox ().ViewToModel (position);
			MenuCommandStructure commands = CreateEmptyAreaCommandStructure (uiManager, uiEnvironment, modelPosition);
			selectedCommand = eventHandler.OnContextMenu (position, commands);
		}
		if (selectedCommand != nullptr) {
			selectedCommand->Do ();
		}
		handlerResult = EventHandlerResult::EventHandled;
	}

	return handlerResult;
}

EventHandlerResult InteractionHandler::HandleMouseDoubleClick (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	if (multiMouseMoveHandler.HasHandler ()) {
		return EventHandlerResult::EventNotHandled;
	}

	UINodePtr foundNode = FindNodeUnderPosition (uiManager, uiEnvironment, position);
	if (foundNode != nullptr) {
		NodeInputEventHandler nodeInputEventHandler (uiManager, foundNode);
		EventHandlerResult handlerResult = nodeInputEventHandler.HandleMouseDoubleClick (uiEnvironment, modifierKeys, mouseButton, position);
		if (handlerResult == EventHandlerResult::EventHandled) {
			return handlerResult;
		}

		SetParametersMenuCommand setParameters (uiManager, uiEnvironment, foundNode, NE::NodeCollection ({ foundNode->GetId () }));
		setParameters.Do ();
		return EventHandlerResult::EventHandled;
	}

	UINodeGroupPtr foundGroup = FindNodeGroupUnderPosition (uiManager, uiEnvironment, position);
	if (foundGroup != nullptr) {
		SetGroupParametersMenuCommand setParameters (uiManager, uiEnvironment, foundGroup);
		setParameters.Do ();
		return EventHandlerResult::EventHandled;
	}

	EventHandler& eventHandler = uiEnvironment.GetEventHandler ();
	eventHandler.OnDoubleClick (position, mouseButton);
	return EventHandlerResult::EventHandled;
}

EventHandlerResult InteractionHandler::HandleMouseWheel (NodeUIEnvironment&, const ModifierKeys&, MouseWheelRotation rotation, const Point& position)
{
	ViewBox viewBox = uiManager.GetViewBox ();
	double scaleRatio = (rotation == MouseWheelRotation::Forward ? 1.1 : 0.9);
	viewBox.SetScale (viewBox.GetScale () * scaleRatio, position);
	uiManager.SetViewBox (viewBox);
	return EventHandlerResult::EventHandled;
}

EventHandlerResult InteractionHandler::HandleKeyPress (NodeUIEnvironment& uiEnvironment, const Key& pressedKey)
{
	if (pressedKey.GetKeyCode () == KeyCode::Escape) {
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
	MenuCommandPtr command = nullptr;

	switch (pressedKey.GetKeyCode ()) {
		case KeyCode::Delete:
			{
				if (!selectedNodes.IsEmpty ()) {
					command.reset (new DeleteNodesMenuCommand (uiManager, uiEnvironment, selectedNodes));
				}
			}
			break;
		case KeyCode::SelectAll:
			{
				NE::NodeCollection allSelectedNodes;
				uiManager.EnumerateUINodes ([&] (const UINodeConstPtr& uiNode) {
					allSelectedNodes.Insert (uiNode->GetId ());
					return true;
				});
				uiManager.SetSelectedNodes (allSelectedNodes);
			}
			break;
		case KeyCode::Copy:
			{
				if (!selectedNodes.IsEmpty ()) {
					command.reset (new CopyNodesMenuCommand (uiManager, uiEnvironment, selectedNodes));
				}
			}
			break;
		case KeyCode::Paste:
			{
				if (uiEnvironment.GetClipboardHandler ().HasClipboardContent ()) {
					Point modelPastePosition = pastePositionCalculator.CalculatePastePosition (uiManager, uiEnvironment);
					command.reset (new PasteNodesMenuCommand (uiManager, uiEnvironment, modelPastePosition));
				}
			}
			break;
		case KeyCode::Group:
			{
				if (!selectedNodes.IsEmpty ()) {
					command.reset (new CreateGroupMenuCommand (uiManager, selectedNodes));
				}
			}
			break;
		case KeyCode::Ungroup:
			{
				if (!selectedNodes.IsEmpty ()) {
					command.reset (new RemoveNodesFromGroupMenuCommand (uiManager, selectedNodes));
				}
			}
			break;
		case KeyCode::Undo:
			{
				command.reset (new UndoMenuCommand (uiManager, uiEnvironment));
			}
			break;
		case KeyCode::Redo:
			{
				command.reset (new RedoMenuCommand (uiManager, uiEnvironment));
			}
			break;
		case KeyCode::Escape:
		case KeyCode::Undefined:
			DBGBREAK ();
			break;
	}

	if (command != nullptr) {
		command->Do ();
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

}
