#ifndef NUIE_INPUTEVENTHANDLER_HPP
#define NUIE_INPUTEVENTHANDLER_HPP

#include "NUIE_HashUtils.hpp"
#include "NUIE_Geometry.hpp"
#include "NUIE_NodeUIEnvironment.hpp"

#include <unordered_map>
#include <unordered_set>

namespace NUIE
{

enum class MouseButton
{
	Left,
	Middle,
	Right
};

enum class MouseWheelRotation
{
	Forward,
	Backward
};

enum class ModifierKeyCode
{
	Command		= 1,
	Shift		= 2
};

enum class KeyCode
{
	Escape
};

class ModifierKeys
{
public:
	ModifierKeys ();
	ModifierKeys (const std::initializer_list<ModifierKeyCode>& keyCodes);
	~ModifierKeys ();

	void	Insert (ModifierKeyCode keyCode);
	bool	Contains (ModifierKeyCode keyCode) const;

private:
	std::unordered_set<ModifierKeyCode, EnumHash> keys;
};

extern const ModifierKeys EmptyModifierKeys;

enum class EventHandlerResult
{
	EventHandled,
	EventNotHandled
};

class InputEventHandler
{
public:
	InputEventHandler ();
	virtual ~InputEventHandler ();

	virtual EventHandlerResult	HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseDoubleClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseSwipe (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& offset) = 0;
	virtual EventHandlerResult	HandleKeyPress (NodeUIEnvironment& env, KeyCode pressedKey) = 0;
};

class MouseEventTranslator
{
public:
	MouseEventTranslator (InputEventHandler& handler);
	virtual ~MouseEventTranslator ();

	void	OnMouseDown (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position);
	void	OnMouseUp (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position);
	void	OnMouseMove (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position);

private:
	InputEventHandler&									handler;
	std::unordered_map<MouseButton, Point, EnumHash>	downMouseButtons;
	std::unordered_set<MouseButton, EnumHash>			movingMouseButtons;
};

}

#endif
