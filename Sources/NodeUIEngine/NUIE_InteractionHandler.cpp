#include "NUIE_InteractionHandler.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UIItemFinder.hpp"
#include "NUIE_ConnectionInteractions.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_NodeCommonMenuCommands.hpp"
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
		Selection selection = uiManager.GetSelection ();
		if (!modifierKeys.Contains (ModifierKeyCode::Command)) {
			selection.Clear ();
		}
		std::unordered_set<UINodePtr> nodesToSelect;
		uiManager.EnumerateNodes ([&] (UINodePtr uiNode) {
			const NE::NodeId& uiNodeId = uiNode->GetId ();
			Rect nodeRect = uiNode->GetRect (uiEnvironment);
			if (modelSelectionRect.Contains (nodeRect)) {
				if (selection.ContainsNode (uiNodeId)) {
					selection.DeleteNode (uiNodeId);
				} else {
					selection.AddNode (uiNodeId);
				}
			}
			return true;
		});
		uiManager.SetSelection (selection, uiEnvironment);
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

	virtual void HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position)	override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point offset = viewBox.ViewToModel (position) - startModelPosition;

		MoveNodesCommand command (relevantNodes, offset);
		uiManager.ExecuteCommand (command, uiEnvironment);

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
		uiManager.EnumerateUIConnections (relevantNodes, [&] (UIOutputSlotConstPtr outputSlot, UIInputSlotConstPtr inputSlot) {
			UINodeConstPtr outputNode = uiManager.GetNode (outputSlot->GetOwnerNodeId ());
			UINodeConstPtr inputNode = uiManager.GetNode (inputSlot->GetOwnerNodeId ());
			temporaryConnections.push_back ({
				outputNode->GetOutputSlotConnPosition (drawingEnv, outputSlot->GetId ()),
				inputNode->GetInputSlotConnPosition (drawingEnv, inputSlot->GetId ())
			});
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

	virtual void HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position)	override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point offset = viewBox.ViewToModel (position) - startModelPosition;

		CopyMoveNodesCommand command (uiEnvironment, relevantNodes, offset);
		uiManager.ExecuteCommand (command, uiEnvironment);

		uiManager.RequestRedraw ();
	}

	virtual void HandleAbort () override
	{
		RequestRedraw ();
	}

	virtual void EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&, Direction)>& processor) const override
	{
		const ViewBox& viewBox = uiManager.GetViewBox ();
		Point diff = viewBox.ViewToModel (currentPosition) - startModelPosition;
		for (const auto& connection : temporaryConnections) {
			processor (connection.first + diff, connection.second + diff, Direction::None);
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

class UINodeInteractionCommandInterface : public UINodeCommandInterface
{
public:
	UINodeInteractionCommandInterface (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment) :
		UINodeCommandInterface (),
		uiManager (uiManager),
		uiEnvironment (uiEnvironment)
	{

	}

	virtual void RunUndoableCommand (const std::function<void ()>& func) override
	{
		CustomUndoableCommand command (func);
		uiManager.ExecuteCommand (command, uiEnvironment);
	}

private:
	NodeUIManager& uiManager;
	NodeUIEnvironment& uiEnvironment;
};

PastePositionCalculator::PastePositionCalculator () :
	lastPastePosition (),
	samePositionPasteCounter (0)
{

}

void PastePositionCalculator::Clear ()
{
	lastPastePosition.Set (0.0, 0.0);
	samePositionPasteCounter = 0;
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
		UINodeInteractionCommandInterface commandInterface (uiManager, uiEnvironment);
		Point modelPosition = uiManager.GetViewBox ().ViewToModel (position);
		return uiNode->HandleMouseClick (uiEnvironment, modifierKeys, mouseButton, modelPosition, commandInterface);
	});
}

EventHandlerResult NodeInputEventHandler::HandleMouseDoubleClick (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	return ForwardEventToNode ([&] () {
		UINodeInteractionCommandInterface commandInterface (uiManager, uiEnvironment);
		Point modelPosition = uiManager.GetViewBox ().ViewToModel (position);
		return uiNode->HandleMouseDoubleClick (uiEnvironment, modifierKeys, mouseButton, modelPosition, commandInterface);
	});
}

EventHandlerResult NodeInputEventHandler::HandleMouseWheel (NodeUIEnvironment&, const ModifierKeys&, MouseWheelRotation, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeInputEventHandler::HandleMouseSwipe (NodeUIEnvironment&, const ModifierKeys&, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult NodeInputEventHandler::HandleKeyPress (NodeUIEnvironment&, KeyCode)
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

void InteractionHandler::Clear ()
{
	multiMouseMoveHandler.Clear ();
	pastePositionCalculator.Clear ();
}

const NodeDrawingModifier* InteractionHandler::GetDrawingModifier ()
{
	return &multiMouseMoveHandler;
}

void InteractionHandler::ExecuteCommand (NodeUIEnvironment& uiEnvironment, CommandCode command)
{
	if (command == CommandCode::Escape) {
		HandleKeyPress (uiEnvironment, KeyCode::Escape);
		return;
	}

	const Selection& selection = uiManager.GetSelection ();
	const NE::NodeCollection& selectedNodes = selection.GetNodes ();
	MenuCommandPtr menuCommand = nullptr;

	switch (command) {
		case CommandCode::SelectAll:
			{
				Selection allSelection;
				uiManager.EnumerateNodes ([&] (UINodeConstPtr uiNode) {
					allSelection.AddNode (uiNode->GetId ());
					return true;
				});
				uiManager.SetSelection (allSelection, uiEnvironment);
			}
			break;
		case CommandCode::SetParameters:
			{
				if (!selectedNodes.IsEmpty ()) {
					size_t nodeCount = selectedNodes.Count ();
					UINodePtr currentNode = uiManager.GetNode (selectedNodes.Get (nodeCount - 1));
					menuCommand.reset (new SetNodeParametersMenuCommand (uiManager, uiEnvironment, currentNode, selectedNodes));
				}
			}
			break;
		case CommandCode::Delete:
			{
				menuCommand.reset (new DeleteNodesMenuCommand (uiManager, uiEnvironment, selectedNodes));
			}
			break;
		case CommandCode::Copy:
			{
				menuCommand.reset (new CopyNodesMenuCommand (uiManager, uiEnvironment, selectedNodes));
			}
			break;
		case CommandCode::Paste:
			{
				Point modelPastePosition = pastePositionCalculator.CalculatePastePosition (uiManager, uiEnvironment);
				menuCommand.reset (new PasteNodesMenuCommand (uiManager, uiEnvironment, modelPastePosition));
			}
			break;
		case CommandCode::Group:
			{
				menuCommand.reset (new CreateGroupMenuCommand (uiManager, uiEnvironment, selectedNodes));
			}
			break;
		case CommandCode::Ungroup:
			{
				menuCommand.reset (new RemoveNodesFromGroupMenuCommand (uiManager, uiEnvironment, selectedNodes));
			}
			break;
		case CommandCode::Undo:
			{
				menuCommand.reset (new UndoMenuCommand (uiManager, uiEnvironment));
			}
			break;
		case CommandCode::Redo:
			{
				menuCommand.reset (new RedoMenuCommand (uiManager, uiEnvironment));
			}
			break;
		case CommandCode::Escape:
		case CommandCode::Undefined:
			DBGBREAK ();
			break;
	}

	if (menuCommand != nullptr) {
		menuCommand->Do ();
	}
}

void InteractionHandler::HandleContextMenuRequest (NodeUIEnvironment& uiEnvironment, const Point& position)
{
	EventHandler& eventHandler = uiEnvironment.GetEventHandler ();
	MenuCommandPtr selectedCommand;
	bool found = FindItemUnderPosition (uiManager, uiEnvironment, position,
		[&] (const UIInputSlotPtr& foundInputSlot) {
			MenuCommandStructure commands = CreateInputSlotCommandStructure (uiManager, uiEnvironment, foundInputSlot);
			selectedCommand = eventHandler.OnContextMenu (EventHandler::ContextMenuType::InputSlot, position, commands);
		},
		[&] (const UIOutputSlotPtr& foundOutputSlot) {
			MenuCommandStructure commands = CreateOutputSlotCommandStructure (uiManager, uiEnvironment, foundOutputSlot);
			selectedCommand = eventHandler.OnContextMenu (EventHandler::ContextMenuType::OutputSlot, position, commands);
		},
		[&] (const UINodePtr& foundNode) {
			MenuCommandStructure commands = CreateNodeCommandStructure (uiManager, uiEnvironment, foundNode);
			selectedCommand = eventHandler.OnContextMenu (EventHandler::ContextMenuType::Node, position, commands);
		},
		[&] (const UINodeGroupPtr& foundGroup) {
			MenuCommandStructure commands = CreateNodeGroupCommandStructure (uiManager, uiEnvironment, foundGroup);
			selectedCommand = eventHandler.OnContextMenu (EventHandler::ContextMenuType::Group, position, commands);
		}
	);
	if (!found) {
		Point modelPosition = uiManager.GetViewBox ().ViewToModel (position);
		MenuCommandStructure commands = CreateEmptyAreaCommandStructure (uiManager, uiEnvironment, modelPosition);
		selectedCommand = eventHandler.OnContextMenu (EventHandler::ContextMenuType::EmptyArea, position, commands);
	}
	if (selectedCommand != nullptr) {
		selectedCommand->Do ();
	}
}

EventHandlerResult InteractionHandler::HandleMouseDragStart (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	EventHandlerResult handlerResult = EventHandlerResult::EventNotHandled;
	if (!multiMouseMoveHandler.AreOtherHandlersAllowed ()) {
		return handlerResult;
	}

	if (mouseButton == MouseButton::Left) {
		bool found = FindItemUnderPosition (uiManager, uiEnvironment, position,
			[&] (const UIInputSlotConstPtr& foundInputSlot) {
				if (modifierKeys.Contains (ModifierKeyCode::Command)) {
					if (uiManager.HasConnectedOutputSlots (foundInputSlot)) {
						std::vector<ConnectionStartOutputSlot> foundOutputSlots;
						uiManager.EnumerateConnectedUIOutputSlots (foundInputSlot, [&] (UIOutputSlotConstPtr outputSlot) {
							UINodeConstPtr outputNode = uiManager.GetNode (outputSlot->GetOwnerNodeId ());
							Point outputSlotPosition = outputNode->GetOutputSlotConnPosition (uiEnvironment, outputSlot->GetId ());
							foundOutputSlots.push_back (ConnectionStartOutputSlot (outputSlot, outputSlotPosition));
							return true;
						});
						if (DBGVERIFY (!foundOutputSlots.empty ())) {
							multiMouseMoveHandler.AddHandler (mouseButton, new NodeOutputToInputReconnectionHandler (uiManager, foundOutputSlots, foundInputSlot));
						}
					}
				} else {
					if (uiManager.CanConnectOutputSlotToInputSlot (foundInputSlot)) {
						UINodeConstPtr uiNode = uiManager.GetNode (foundInputSlot->GetOwnerNodeId ());
						Point startSlotPosition = uiNode->GetInputSlotConnPosition (uiEnvironment, foundInputSlot->GetId ());
						ConnectionStartInputSlot startSlot (foundInputSlot, startSlotPosition);
						multiMouseMoveHandler.AddHandler (mouseButton, new NodeInputToOutputConnectionHandler (uiManager, startSlot));
					}
				}
			},
			[&] (const UIOutputSlotConstPtr& foundOutputSlot) {
				if (modifierKeys.Contains (ModifierKeyCode::Command)) {
					if (uiManager.HasConnectedInputSlots (foundOutputSlot)) {
						std::vector<ConnectionStartInputSlot> foundInputSlots;
						uiManager.EnumerateConnectedUIInputSlots (foundOutputSlot, [&] (UIInputSlotConstPtr inputSlot) {
							UINodeConstPtr inputNode = uiManager.GetNode (inputSlot->GetOwnerNodeId ());
							Point inputSlotPosition = inputNode->GetInputSlotConnPosition (uiEnvironment, inputSlot->GetId ());
							foundInputSlots.push_back (ConnectionStartInputSlot (inputSlot, inputSlotPosition));
							return true;
						});
						if (DBGVERIFY (!foundInputSlots.empty ())) {
							multiMouseMoveHandler.AddHandler (mouseButton, new NodeInputToOutputReconnectionHandler (uiManager, foundInputSlots, foundOutputSlot));
						}
					}
				} else {
					UINodeConstPtr uiNode = uiManager.GetNode (foundOutputSlot->GetOwnerNodeId ());
					Point startSlotPosition = uiNode->GetOutputSlotConnPosition (uiEnvironment, foundOutputSlot->GetId ());
					ConnectionStartOutputSlot startSlot (foundOutputSlot, startSlotPosition);
					multiMouseMoveHandler.AddHandler (mouseButton, new NodeOutputToInputConnectionHandler (uiManager, startSlot));
				}
			},
			[&] (const UINodePtr& foundNode) {
				NE::NodeCollection nodesToMove = GetNodesForCommand (uiManager, foundNode);
				if (modifierKeys.Contains (ModifierKeyCode::Command)) {
					multiMouseMoveHandler.AddHandler (mouseButton, new NodeCopyMovingHandler (uiEnvironment, uiManager, nodesToMove));
				} else {
					multiMouseMoveHandler.AddHandler (mouseButton, new NodeMovingHandler (uiManager, nodesToMove));
				}
			},
			[&] (const UINodeGroupPtr& foundGroup) {
				NE::NodeCollection nodesToMove = uiManager.GetGroupNodes (foundGroup);
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
		Selection selection;
		if (foundNode != nullptr) {
			const NE::NodeId& foundNodeId = foundNode->GetId ();
			selection = uiManager.GetSelection ();
			if (modifierKeys.Contains (ModifierKeyCode::Command)) {
				if (selection.ContainsNode (foundNodeId)) {
					selection.DeleteNode (foundNodeId);
				} else {
					selection.AddNode (foundNodeId);
				}
			} else {
				selection.Clear ();
				selection.AddNode (foundNodeId);
			}
		}
		uiManager.SetSelection (selection, uiEnvironment);
		handlerResult = EventHandlerResult::EventHandled;
	} else if (mouseButton == MouseButton::Right) {
		HandleContextMenuRequest (uiEnvironment, position);
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

		SetNodeParametersMenuCommand setParameters (uiManager, uiEnvironment, foundNode, NE::NodeCollection ({ foundNode->GetId () }));
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

EventHandlerResult InteractionHandler::HandleMouseSwipe (NodeUIEnvironment&, const ModifierKeys&, const Point& offset)
{
	ViewBox viewBox = uiManager.GetViewBox ();
	viewBox.SetOffset (viewBox.GetOffset () + offset);
	uiManager.SetViewBox (viewBox);
	return EventHandlerResult::EventHandled;
}

EventHandlerResult InteractionHandler::HandleKeyPress (NodeUIEnvironment& uiEnvironment, KeyCode pressedKey)
{
	if (pressedKey == KeyCode::Escape) {
		if (multiMouseMoveHandler.HasHandler ()) {
			multiMouseMoveHandler.AbortHandlers ();
			return EventHandlerResult::EventHandled;
		} else {
			uiManager.SetSelection (EmptySelection, uiEnvironment);
		}
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

}
