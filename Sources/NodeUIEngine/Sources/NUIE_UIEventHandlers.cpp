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

void MouseMoveHandler::OnMouseDown (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position)
{
	startPosition = position;
	prevPosition = startPosition;
	HandleMouseDown (env, pressedKeys, position);
}

void MouseMoveHandler::OnMouseMove (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position)
{
	HandleMouseMove (env, pressedKeys, position);
	prevPosition = position;
}

void MouseMoveHandler::OnMouseUp (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position)
{
	HandleMouseUp (env, pressedKeys, position);
}

void MouseMoveHandler::HandleMouseDown (NodeUIEnvironment&, const KeySet&, const Point&)
{
	
}

void MouseMoveHandler::HandleMouseMove (NodeUIEnvironment&, const KeySet&, const Point&)
{

}

void MouseMoveHandler::HandleMouseUp (NodeUIEnvironment&, const KeySet&, const Point&)
{

}

void MouseMoveHandler::EnumerateSelectionRectangles (const std::function<void (const Rect&)>&) const
{

}

void MouseMoveHandler::EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&)>&) const
{

}

MultiMouseMoveHandler::MultiMouseMoveHandler ()
{

}

bool MultiMouseMoveHandler::HasHandler (MouseButton mouseButton) const
{
	return handlers.find (mouseButton) != handlers.end ();
}

std::shared_ptr<MouseMoveHandler>& MultiMouseMoveHandler::GetHandler (MouseButton mouseButton)
{
	return handlers.find (mouseButton)->second;
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

void MultiMouseMoveHandler::EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end)>& processor) const
{
	for (const auto& it : handlers) {
		it.second->EnumerateTemporaryConnections (processor);
	}
}

}
