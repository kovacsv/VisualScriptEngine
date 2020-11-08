#include "NUIE_ConnectionInteractions.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_UIItemFinder.hpp"

#include <algorithm>

namespace NUIE
{

NodeOutputToInputConnectionHandler::NodeOutputToInputConnectionHandler (NodeUIManager& uiManager, const UIOutputSlotConstPtr& startSlot, const Point& startSlotPosition) :
	NodeConnectionHandler<UIOutputSlotConstPtr, UIInputSlotConstPtr> (uiManager, startSlot, startSlotPosition)
{
	
}

void NodeOutputToInputConnectionHandler::EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	Point position = viewBox.ViewToModel (currentPosition);
	processor (startSlotPosition, position, Direction::Forward);
}

void NodeOutputToInputConnectionHandler::HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position)
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	currentPosition = position;
	endSlot = FindInputSlotUnderPosition (uiManager, uiEnvironment, currentPosition);
	if (endSlot != nullptr) {
		if (CanConnectToInputSlot (startSlot, endSlot)) {
			UINodePtr uiNode = uiManager.GetNode (endSlot->GetOwnerNodeId ());
			currentPosition = viewBox.ModelToView (uiNode->GetInputSlotConnPosition (uiEnvironment, endSlot->GetId ()));
		} else {
			endSlot = nullptr;
		}
	}
	uiManager.RequestRedraw ();
}

void NodeOutputToInputConnectionHandler::HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&)
{
	if (endSlot != nullptr) {
		ConnectSlotsCommand command (startSlot, endSlot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	} else {
		uiManager.RequestRedraw ();
	}
}

bool NodeOutputToInputConnectionHandler::CanConnectToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
{
	return uiManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}


NodeOutputToInputReconnectionHandler::NodeOutputToInputReconnectionHandler (NodeUIManager& uiManager, const UIOutputSlotConstPtr& startSlot, const UIInputSlotConstPtr& originalEndSlot, const Point& startSlotPosition) :
	NodeOutputToInputConnectionHandler (uiManager, startSlot, startSlotPosition),
	originalEndSlot (originalEndSlot)
{

}

bool NodeOutputToInputReconnectionHandler::NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const
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

void NodeOutputToInputReconnectionHandler::HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&)
{
	if (endSlot != nullptr && endSlot != originalEndSlot) {
		ReconnectInputSlotCommand command (startSlot, originalEndSlot, endSlot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	} else if (endSlot == nullptr) {
		DisconnectSlotsCommand disconnectCommand (startSlot, originalEndSlot);
		uiManager.ExecuteCommand (disconnectCommand, uiEnvironment);
	} else {
		uiManager.RequestRedraw ();
	}
}

bool NodeOutputToInputReconnectionHandler::CanConnectToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
{
	if (inputSlot == originalEndSlot) {
		return true;
	}
	if (uiManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot)) {
		return true;
	}
	return false;
}

NodeInputToOutputConnectionHandler::NodeInputToOutputConnectionHandler (NodeUIManager& uiManager, const UIInputSlotConstPtr& startSlot, const Point& startSlotPosition) :
	NodeConnectionHandler<UIInputSlotConstPtr, UIOutputSlotConstPtr> (uiManager, startSlot, startSlotPosition)
{

}

void NodeInputToOutputConnectionHandler::EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	Point position = viewBox.ViewToModel (currentPosition);
	processor (position, startSlotPosition, Direction::Backward);
}

void NodeInputToOutputConnectionHandler::HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position)
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	currentPosition = position;
	endSlot = FindOutputSlotUnderPosition (uiManager, uiEnvironment, currentPosition);
	if (endSlot != nullptr) {
		if (CanConnectToOutputSlot (endSlot, startSlot)) {
			UINodePtr uiNode = uiManager.GetNode (endSlot->GetOwnerNodeId ());
			currentPosition = viewBox.ModelToView (uiNode->GetOutputSlotConnPosition (uiEnvironment, endSlot->GetId ()));
		} else {
			endSlot = nullptr;
		}
	}
	uiManager.RequestRedraw ();
}

void NodeInputToOutputConnectionHandler::HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&)
{
	if (endSlot != nullptr) {
		ConnectSlotsCommand command (endSlot, startSlot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	} else {
		uiManager.RequestRedraw ();
	}
}

bool NodeInputToOutputConnectionHandler::CanConnectToOutputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
{
	return uiManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}

NodeInputToOutputReconnectionHandler::NodeInputToOutputReconnectionHandler (NodeUIManager& uiManager, const UIInputSlotConstPtr& startSlot, const UIOutputSlotConstPtr& originalEndSlot, const Point& startSlotPosition) :
	NodeInputToOutputConnectionHandler (uiManager, startSlot, startSlotPosition),
	originalEndSlot (originalEndSlot)
{

}

bool NodeInputToOutputReconnectionHandler::NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const
{
	if (outputNodeId == originalEndSlot->GetOwnerNodeId () &&
		inputNodeId == startSlot->GetOwnerNodeId () &&
		outputSlotId == originalEndSlot->GetId () &&
		inputSlotId == startSlot->GetId ()) {
		return false;
	}
	return true;
}

void NodeInputToOutputReconnectionHandler::HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&)
{
	if (endSlot != nullptr && endSlot != originalEndSlot) {
		ReconnectOutputSlotCommand command (originalEndSlot, endSlot, startSlot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	} else if (endSlot == nullptr) {
		DisconnectSlotsCommand disconnectCommand (originalEndSlot, startSlot);
		uiManager.ExecuteCommand (disconnectCommand, uiEnvironment);
	} else {
		uiManager.RequestRedraw ();
	}
}

bool NodeInputToOutputReconnectionHandler::CanConnectToOutputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const
{
	if (outputSlot == originalEndSlot) {
		return true;
	}
	if (uiManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot)) {
		return true;
	}
	return false;
}

}
