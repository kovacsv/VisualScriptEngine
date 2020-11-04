#include "NUIE_UIEventHandlers.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

MouseMoveHandler::MouseMoveHandler ()
{

}

MouseMoveHandler::~MouseMoveHandler ()
{

}

bool MouseMoveHandler::AreOtherHandlersAllowed () const
{
	return true;
}

void MouseMoveHandler::OnMouseDown (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position)
{
	startPosition = position;
	currentPosition = position;
	prevPosition = position;
	HandleMouseDown (env, modifierKeys, position);
}

void MouseMoveHandler::OnMouseMove (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position)
{
	currentPosition = position;
	HandleMouseMove (env, modifierKeys, position);
	prevPosition = position;
}

void MouseMoveHandler::OnMouseUp (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position)
{
	currentPosition = position;
	HandleMouseUp (env, modifierKeys, position);
}

void MouseMoveHandler::OnAbort ()
{
	HandleAbort ();
}

void MouseMoveHandler::HandleMouseDown (NodeUIEnvironment&, const ModifierKeys&, const Point&)
{
	
}

void MouseMoveHandler::HandleMouseMove (NodeUIEnvironment&, const ModifierKeys&, const Point&)
{

}

void MouseMoveHandler::HandleMouseUp (NodeUIEnvironment&, const ModifierKeys&, const Point&)
{

}

void MouseMoveHandler::HandleAbort ()
{

}

void MouseMoveHandler::EnumerateSelectionRectangles (const std::function<void (const Rect&)>&) const
{

}

void MouseMoveHandler::EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&, Direction)>&) const
{

}

void MouseMoveHandler::EnumerateDuplicatedNodes (const std::function<void (const NE::NodeId&, const Point&)>&) const
{

}

bool MouseMoveHandler::NeedToDrawConnection (const NE::NodeId&, const NE::SlotId&, const NE::NodeId&, const NE::SlotId&) const
{
	return true;
}

Point MouseMoveHandler::GetNodeOffset (const NE::NodeId&) const
{
	return Point (0.0, 0.0);
}

MultiMouseMoveHandler::MultiMouseMoveHandler () :
	handlers ()
{

}

void MultiMouseMoveHandler::Clear ()
{
	handlers.clear ();
}

bool MultiMouseMoveHandler::HasHandler (MouseButton mouseButton) const
{
	return handlers.find (mouseButton) != handlers.end ();
}

std::shared_ptr<MouseMoveHandler>& MultiMouseMoveHandler::GetHandler (MouseButton mouseButton)
{
	return handlers.find (mouseButton)->second;
}

void MultiMouseMoveHandler::AbortHandlers ()
{
	for (const auto& it : handlers) {
		it.second->OnAbort ();
	}
	handlers.clear ();
}

bool MultiMouseMoveHandler::AreOtherHandlersAllowed () const
{
	for (const auto& it : handlers) {
		if (!it.second->AreOtherHandlersAllowed ()) {
			return false;
		}
	}
	return true;
}

void MultiMouseMoveHandler::AddHandler (MouseButton mouseButton, MouseMoveHandler* newHandler)
{
	if (DBGERROR (handlers.find (mouseButton) != handlers.end ())) {
		return;
	}
	handlers.insert ({ mouseButton, std::shared_ptr<MouseMoveHandler> (newHandler) });
}

void MultiMouseMoveHandler::RemoveHandler (MouseButton mouseButton)
{
	if (DBGERROR (handlers.find (mouseButton) == handlers.end ())) {
		return;
	}
	handlers.erase (mouseButton);
}

void MultiMouseMoveHandler::EnumerateHandlers (const std::function<void (std::shared_ptr<MouseMoveHandler>&)>& processor)
{
	for (auto& it : handlers) {
		processor (it.second);
	}
}

bool MultiMouseMoveHandler::HasHandler () const
{
	return !handlers.empty ();
}

void MultiMouseMoveHandler::EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const
{
	for (const auto& it : handlers) {
		it.second->EnumerateSelectionRectangles (processor);
	}
}

void MultiMouseMoveHandler::EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction)>& processor) const
{
	for (const auto& it : handlers) {
		it.second->EnumerateTemporaryConnections (processor);
	}
}

void MultiMouseMoveHandler::EnumerateDuplicatedNodes (const std::function<void (const NE::NodeId&, const Point&)>& processor) const
{
	for (const auto& it : handlers) {
		it.second->EnumerateDuplicatedNodes (processor);
	}
}

bool MultiMouseMoveHandler::NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const
{
	for (const auto& it : handlers) {
		if (!it.second->NeedToDrawConnection (outputNodeId, outputSlotId, inputNodeId, inputSlotId)) {
			return false;
		}
	}
	return true;
}

Point MultiMouseMoveHandler::GetNodeOffset (const NE::NodeId& nodeId) const
{
	Point offset (0.0, 0.0);
	for (const auto& it : handlers) {
		offset = offset + it.second->GetNodeOffset (nodeId);
	}
	return offset;
}

}
