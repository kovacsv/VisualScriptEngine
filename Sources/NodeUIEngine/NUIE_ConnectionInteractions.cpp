#include "NUIE_ConnectionInteractions.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_UIItemFinder.hpp"

#include <algorithm>

namespace NUIE
{

class InteractionUIOutputSlotList : public UIOutputSlotList
{
public:
	InteractionUIOutputSlotList (const std::vector<ConnectionStartOutputSlot>& startSlots) :
		UIOutputSlotList (),
		startSlots (startSlots)
	{

	}

	virtual size_t GetSize () const override
	{
		return startSlots.size ();
	}

	virtual void Enumerate (const std::function<bool (NE::OutputSlotConstPtr)>& processor) const override
	{
		for (const ConnectionStartOutputSlot& startSlot : startSlots) {
			processor (startSlot.slot);
		}
	}

private:
	const std::vector<ConnectionStartOutputSlot>& startSlots;
};

class InteractionUIInputSlotList : public UIInputSlotList
{
public:
	InteractionUIInputSlotList (const std::vector<ConnectionStartInputSlot>& startSlots) :
		UIInputSlotList (),
		startSlots (startSlots)
	{

	}

	virtual size_t GetSize () const override
	{
		return startSlots.size ();
	}

	virtual void Enumerate (const std::function<bool (NE::InputSlotConstPtr)>& processor) const override
	{
		for (const ConnectionStartInputSlot& startSlot : startSlots) {
			processor (startSlot.slot);
		}
	}

private:
	const std::vector<ConnectionStartInputSlot>& startSlots;
};

ConnectionStartOutputSlot::ConnectionStartOutputSlot (const UIOutputSlotConstPtr& slot, const Point& position) :
	slot (slot),
	position (position)
{

}

ConnectionStartInputSlot::ConnectionStartInputSlot (const UIInputSlotConstPtr& slot, const Point& position) :
	slot (slot),
	position (position)
{

}

NodeOutputToInputConnectionHandlerBase::NodeOutputToInputConnectionHandlerBase (NodeUIManager& uiManager) :
	MouseMoveHandler (),
	uiManager (uiManager),
	endSlot (nullptr)
{

}

void NodeOutputToInputConnectionHandlerBase::HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position)
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	currentPosition = position;
	endSlot = FindInputSlotUnderPosition (uiManager, uiEnvironment, currentPosition);
	if (endSlot != nullptr) {
		if (CanConnectToInputSlot (endSlot)) {
			UINodePtr uiNode = uiManager.GetNode (endSlot->GetOwnerNodeId ());
			currentPosition = viewBox.ModelToView (uiNode->GetInputSlotConnPosition (uiEnvironment, endSlot->GetId ()));
		} else {
			endSlot = nullptr;
		}
	}
	uiManager.RequestRedraw ();
}

void NodeOutputToInputConnectionHandlerBase::HandleAbort ()
{
	uiManager.RequestRedraw ();
}

NodeOutputToInputConnectionHandler::NodeOutputToInputConnectionHandler (NodeUIManager& uiManager, const ConnectionStartOutputSlot& startSlot) :
	NodeOutputToInputConnectionHandlerBase (uiManager),
	startSlot (startSlot)
{
	
}

void NodeOutputToInputConnectionHandler::EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	Point position = viewBox.ViewToModel (currentPosition);
	processor (startSlot.position, position, Direction::Forward);
}

void NodeOutputToInputConnectionHandler::HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&)
{
	if (endSlot != nullptr) {
		ConnectSlotsCommand command (startSlot.slot, endSlot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	} else {
		uiManager.RequestRedraw ();
	}
}

bool NodeOutputToInputConnectionHandler::CanConnectToInputSlot (const UIInputSlotConstPtr& inputSlot) const
{
	return uiManager.CanConnectOutputSlotToInputSlot (startSlot.slot, inputSlot);
}

NodeOutputToInputReconnectionHandler::NodeOutputToInputReconnectionHandler (NodeUIManager& uiManager, const std::vector<ConnectionStartOutputSlot>& startSlots, const UIInputSlotConstPtr& originalEndSlot) :
	NodeOutputToInputConnectionHandlerBase (uiManager),
	startSlots (startSlots),
	originalEndSlot (originalEndSlot)
{

}

void NodeOutputToInputReconnectionHandler::EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	Point position = viewBox.ViewToModel (currentPosition);
	for (const ConnectionStartOutputSlot& startSlot : startSlots) {
		processor (startSlot.position, position, Direction::Forward);
	}
}

bool NodeOutputToInputReconnectionHandler::NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const
{
	if (inputNodeId == originalEndSlot->GetOwnerNodeId () && inputSlotId == originalEndSlot->GetId ()) {
		for (const ConnectionStartOutputSlot& startSlot : startSlots) {
			if (outputNodeId == startSlot.slot->GetOwnerNodeId () && outputSlotId == startSlot.slot->GetId ()) {
				return false;
			}
		}
	}
	return true;
}

void NodeOutputToInputReconnectionHandler::HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&)
{
	InteractionUIOutputSlotList startOutputSlots (startSlots);
	if (endSlot != nullptr && endSlot != originalEndSlot) {
		ReconnectInputSlotCommand command (startOutputSlots, originalEndSlot, endSlot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	} else if (endSlot == nullptr) {
		DisconnectOutputSlotsCommand disconnectCommand (startOutputSlots, originalEndSlot);
		uiManager.ExecuteCommand (disconnectCommand, uiEnvironment);
	} else {
		uiManager.RequestRedraw ();
	}
}

bool NodeOutputToInputReconnectionHandler::CanConnectToInputSlot (const UIInputSlotConstPtr& inputSlot) const
{
	if (inputSlot == originalEndSlot) {
		return true;
	}
	InteractionUIOutputSlotList startOutputSlots (startSlots);
	if (uiManager.CanConnectOutputSlotsToInputSlot (startOutputSlots, inputSlot)) {
		return true;
	}
	return false;
}

NodeInputToOutputConnectionHandlerBase::NodeInputToOutputConnectionHandlerBase (NodeUIManager& uiManager) :
	MouseMoveHandler (),
	uiManager (uiManager),
	endSlot (nullptr)
{

}

void NodeInputToOutputConnectionHandlerBase::HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position)
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	currentPosition = position;
	endSlot = FindOutputSlotUnderPosition (uiManager, uiEnvironment, currentPosition);
	if (endSlot != nullptr) {
		if (CanConnectToOutputSlot (endSlot)) {
			UINodePtr uiNode = uiManager.GetNode (endSlot->GetOwnerNodeId ());
			currentPosition = viewBox.ModelToView (uiNode->GetOutputSlotConnPosition (uiEnvironment, endSlot->GetId ()));
		} else {
			endSlot = nullptr;
		}
	}
	uiManager.RequestRedraw ();
}

void NodeInputToOutputConnectionHandlerBase::HandleAbort ()
{
	uiManager.RequestRedraw ();
}

NodeInputToOutputConnectionHandler::NodeInputToOutputConnectionHandler (NodeUIManager& uiManager, const ConnectionStartInputSlot& startSlot) :
	NodeInputToOutputConnectionHandlerBase (uiManager),
	startSlot (startSlot)
{

}

void NodeInputToOutputConnectionHandler::EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	Point position = viewBox.ViewToModel (currentPosition);
	processor (position, startSlot.position, Direction::Backward);
}

void NodeInputToOutputConnectionHandler::HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&)
{
	if (endSlot != nullptr) {
		ConnectSlotsCommand command (endSlot, startSlot.slot);
		uiManager.ExecuteCommand (command, uiEnvironment);
	} else {
		uiManager.RequestRedraw ();
	}
}

bool NodeInputToOutputConnectionHandler::CanConnectToOutputSlot (const UIOutputSlotConstPtr& outputSlot) const
{
	return uiManager.CanConnectOutputSlotToInputSlot (outputSlot, startSlot.slot);
}

NodeInputToOutputReconnectionHandler::NodeInputToOutputReconnectionHandler (NodeUIManager& uiManager, const std::vector<ConnectionStartInputSlot>& startSlots, const UIOutputSlotConstPtr& originalEndSlot) :
	NodeInputToOutputConnectionHandlerBase (uiManager),
	startSlots (startSlots),
	originalEndSlot (originalEndSlot)
{

}

void NodeInputToOutputReconnectionHandler::EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	Point position = viewBox.ViewToModel (currentPosition);
	for (const ConnectionStartInputSlot& startSlot : startSlots) {
		processor (position, startSlot.position, Direction::Backward);
	}
}

bool NodeInputToOutputReconnectionHandler::NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const
{
	if (outputNodeId == originalEndSlot->GetOwnerNodeId () && outputSlotId == originalEndSlot->GetId ()) {
		for (const ConnectionStartInputSlot& startSlot : startSlots) {
			if (inputNodeId == startSlot.slot->GetOwnerNodeId () && inputSlotId == startSlot.slot->GetId ()) {
				return false;
			}
		}
	}
	return true;
}

void NodeInputToOutputReconnectionHandler::HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&)
{
	InteractionUIInputSlotList startInputSlots (startSlots);
	if (endSlot != nullptr && endSlot != originalEndSlot) {
		ReconnectOutputSlotCommand command (originalEndSlot, endSlot, startInputSlots);
		uiManager.ExecuteCommand (command, uiEnvironment);
	} else if (endSlot == nullptr) {
		DisconnectInputSlotsCommand disconnectCommand (originalEndSlot, startInputSlots);
		uiManager.ExecuteCommand (disconnectCommand, uiEnvironment);
	} else {
		uiManager.RequestRedraw ();
	}
}

bool NodeInputToOutputReconnectionHandler::CanConnectToOutputSlot (const UIOutputSlotConstPtr& outputSlot) const
{
	if (outputSlot == originalEndSlot) {
		return true;
	}
	InteractionUIInputSlotList startInputSlots (startSlots);
	if (uiManager.CanConnectOutputSlotToInputSlots (outputSlot, startInputSlots)) {
		return true;
	}
	return false;
}

}
