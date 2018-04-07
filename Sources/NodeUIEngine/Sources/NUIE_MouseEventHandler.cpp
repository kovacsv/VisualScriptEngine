#include "NUIE_MouseEventHandler.hpp"

namespace NUIE
{

ModifierKeys EmptyModifierKeys ({});

ModifierKeys::ModifierKeys (const std::unordered_set<KeyCode>& keys) :
	keys (keys)
{

}

ModifierKeys::~ModifierKeys ()
{

}


bool ModifierKeys::Contains (KeyCode keyCode) const
{
	return keys.find (keyCode) != keys.end ();
}

MouseEventTranslator::MouseEventTranslator (MouseEventHandler& handler) :
	handler (handler)
{

}

MouseEventTranslator::~MouseEventTranslator ()
{

}

void MouseEventTranslator::OnMouseDown (NodeUIEnvironment&, const ModifierKeys&, MouseButton mouseButton, const Point& position)
{
	downMouseButtons.insert ({ mouseButton, position });
}

void MouseEventTranslator::OnMouseUp (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position)
{
	if (movingMouseButtons.find (mouseButton) != movingMouseButtons.end ()) {
		handler.HandleMouseDragStop (env, pressedKeys, mouseButton, position);
		movingMouseButtons.erase (mouseButton);
	}

	if (downMouseButtons.find (mouseButton) != downMouseButtons.end ()) {
		handler.HandleMouseClick (env, pressedKeys, mouseButton, position);
		downMouseButtons.erase (mouseButton);
	}
}

void MouseEventTranslator::OnMouseMove (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, const Point& position)
{
	for (const auto& it : downMouseButtons) {
		handler.HandleMouseDragStart (env, pressedKeys, it.first, it.second);
		movingMouseButtons.insert (it.first);
	}
	downMouseButtons.clear ();

	if (!movingMouseButtons.empty ()) {
		handler.HandleMouseDrag (env, pressedKeys, position);
	}
}

void MouseEventTranslator::OnMouseWheel (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseWheelRotation rotation, const Point& position)
{
	handler.HandleMouseWheel (env, pressedKeys, rotation, position);
}

void MouseEventTranslator::OnMouseDoubleClick (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position)
{
	// TODO: this is a hack
	handler.HandleMouseClick (env, pressedKeys, mouseButton, position);
}

}
