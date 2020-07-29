#ifndef NUIE_INPUTEVENTHANDLER_HPP
#define NUIE_INPUTEVENTHANDLER_HPP

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
	Control		= 1,
	Shift		= 2
};

enum class KeyCode
{
	Undefined,
	Escape,
	Delete,
	SelectAll,
	Copy,
	Paste,
	Group,
	Undo,
	Redo
};

}

namespace std
{
	template <>
	struct hash<NUIE::MouseButton>
	{
		size_t operator() (const NUIE::MouseButton& button) const noexcept
		{
			return (size_t) button;
		}
	};

	template <>
	struct hash<NUIE::ModifierKeyCode>
	{
		size_t operator() (const NUIE::ModifierKeyCode& keyCode) const noexcept
		{
			return (size_t) keyCode;
		}
	};
}

namespace NUIE
{

class ModifierKeys
{
public:
	ModifierKeys ();
	ModifierKeys (const std::unordered_set<ModifierKeyCode>& keys);
	~ModifierKeys ();

	void	Insert (ModifierKeyCode keyCode);
	bool	Contains (ModifierKeyCode keyCode) const;

private:
	std::unordered_set<ModifierKeyCode> keys;
};

extern const ModifierKeys EmptyModifierKeys;

class Key
{
public:
	Key ();
	Key (KeyCode keyCode);

	bool		IsValid () const;

	KeyCode		GetKeyCode () const;
	void		SetKeyCode (KeyCode newKeyCode);

private:
	KeyCode		keyCode;
};

extern const Key InvalidKey;

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
	virtual EventHandlerResult	HandleKeyPress (NodeUIEnvironment& env, const Key& pressedKey) = 0;
};

class MouseEventTranslator
{
public:
	MouseEventTranslator (InputEventHandler& handler);
	virtual ~MouseEventTranslator ();

	void	OnMouseDown (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position);
	void	OnMouseUp (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position);
	void	OnMouseMove (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position);
	void	OnMouseWheel (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position);
	void	OnMouseDoubleClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position);

private:
	InputEventHandler&						handler;
	std::unordered_map<MouseButton, Point>	downMouseButtons;
	std::unordered_set<MouseButton>			movingMouseButtons;
};

}

#endif
