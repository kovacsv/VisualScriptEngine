#include "NUIE_InputEventHandler.hpp"
#include "NUIE_SkinParams.hpp"

namespace NUIE
{

const ModifierKeys EmptyModifierKeys;
static const double MouseMoveMinOffset = 2.0;

ModifierKeys::ModifierKeys () :
	keys ()
{

}

ModifierKeys::ModifierKeys (const std::initializer_list<ModifierKeyCode>& keyCodes) :
	keys (keyCodes)
{

}

ModifierKeys::~ModifierKeys ()
{

}

void ModifierKeys::Insert (ModifierKeyCode keyCode)
{
	keys.insert (keyCode);
}

bool ModifierKeys::Contains (ModifierKeyCode keyCode) const
{
	return keys.find (keyCode) != keys.end ();
}

InputEventHandler::InputEventHandler ()
{

}

InputEventHandler::~InputEventHandler ()
{

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
	std::unordered_set<MouseButton, EnumHash> downButtonsMoved;
	for (const auto& it : downMouseButtons) {
		if (Point::Distance (it.second, position) > MouseMoveMinOffset) {
			movingMouseButtons.insert (it.first);
			handler.HandleMouseDragStart (env, modifierKeys, it.first, it.second);
			downButtonsMoved.insert (it.first);
		}
	}
	for (MouseButton downButtonMoved : downButtonsMoved) {
		downMouseButtons.erase (downButtonMoved);
	}
	if (!movingMouseButtons.empty ()) {
		handler.HandleMouseDrag (env, modifierKeys, position);
	}
}

}
