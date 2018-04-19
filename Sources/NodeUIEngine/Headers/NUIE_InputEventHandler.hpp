#ifndef INPUTEVENTHANDLER_HPP
#define INPUTEVENTHANDLER_HPP

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

enum class KeyCode
{
	Control		= 1,
	Shift		= 2
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
	struct hash<NUIE::KeyCode>
	{
		size_t operator() (const NUIE::KeyCode& keyCode) const noexcept
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
	ModifierKeys (const std::unordered_set<KeyCode>& keys);
	~ModifierKeys ();

	bool Contains (KeyCode keyCode) const;

private:
	std::unordered_set<KeyCode> keys;
};

extern ModifierKeys EmptyModifierKeys;

class Key
{
public:
	enum class SpecialKey
	{
		Undefined,
		Delete
	};

	Key ();
	Key (wchar_t unicodeKey);
	Key (SpecialKey specialKeyCode);

	bool		IsValid () const;
	bool		IsSpecialKey () const;
	wchar_t		GetUnicodeKey () const;
	SpecialKey	GetSpecialKeyCode () const;

private:
	bool		isValid;
	wchar_t		unicodeKey;
	SpecialKey	specialKeyCode;
};

extern Key InvalidKey;

enum class EventHandlerResult
{
	EventHandled,
	EventNotHandled
};

class InputEventHandler
{
public:
	virtual EventHandlerResult	HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) = 0;
	virtual EventHandlerResult	HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position) = 0;
	virtual EventHandlerResult	HandleKeyPress (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Key& pressedKey) = 0;
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
