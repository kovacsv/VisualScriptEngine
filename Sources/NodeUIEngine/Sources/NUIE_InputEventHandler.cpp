#include "NUIE_InputEventHandler.hpp"

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

Key::Key (int keyCode) :
	keyCode (keyCode),
	specialKeyCode (SpecialKey::Undefined)
{

}

Key::Key (SpecialKey specialKeyCode) :
	keyCode (-1),
	specialKeyCode (specialKeyCode)
{

}

bool Key::IsSpecialKey () const
{
	return specialKeyCode != SpecialKey::Undefined;
}

int Key::GetKeyCode () const
{
	DBGASSERT (keyCode != -1);
	return keyCode;
}

Key::SpecialKey Key::GetSpecialKeyCode () const
{
	DBGASSERT (specialKeyCode != SpecialKey::Undefined);
	return specialKeyCode;
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
