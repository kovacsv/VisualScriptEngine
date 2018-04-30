#include "NUIE_InputEventHandler.hpp"

namespace NUIE
{

ModifierKeys EmptyModifierKeys ({});
Key InvalidKey;

ModifierKeys::ModifierKeys (const std::unordered_set<ModifierKeyCode>& keys) :
	keys (keys)
{

}

ModifierKeys::~ModifierKeys ()
{

}


bool ModifierKeys::Contains (ModifierKeyCode keyCode) const
{
	return keys.find (keyCode) != keys.end ();
}

Key::Key () :
	pressedKeyCode (PressedKeyCode::Undefined)
{

}

Key::Key (PressedKeyCode pressedKeyCode) :
	pressedKeyCode (pressedKeyCode)
{

}

bool Key::IsValid () const
{
	return pressedKeyCode != PressedKeyCode::Undefined;
}

PressedKeyCode Key::GetKeyCode () const
{
	DBGASSERT (pressedKeyCode != PressedKeyCode::Undefined);
	return pressedKeyCode;
}

MouseEventTranslator::MouseEventTranslator (InputEventHandler& handler) :
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

void MouseEventTranslator::OnMouseUp (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	if (movingMouseButtons.find (mouseButton) != movingMouseButtons.end ()) {
		handler.HandleMouseDragStop (env, modifierKeys, mouseButton, position);
		movingMouseButtons.erase (mouseButton);
	}

	if (downMouseButtons.find (mouseButton) != downMouseButtons.end ()) {
		handler.HandleMouseClick (env, modifierKeys, mouseButton, position);
		downMouseButtons.erase (mouseButton);
	}
}

void MouseEventTranslator::OnMouseMove (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position)
{
	for (const auto& it : downMouseButtons) {
		handler.HandleMouseDragStart (env, modifierKeys, it.first, it.second);
		movingMouseButtons.insert (it.first);
	}
	downMouseButtons.clear ();

	if (!movingMouseButtons.empty ()) {
		handler.HandleMouseDrag (env, modifierKeys, position);
	}
}

void MouseEventTranslator::OnMouseWheel (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position)
{
	handler.HandleMouseWheel (env, modifierKeys, rotation, position);
}

void MouseEventTranslator::OnMouseDoubleClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position)
{
	// TODO: this is a hack
	handler.HandleMouseClick (env, modifierKeys, mouseButton, position);
}

}
