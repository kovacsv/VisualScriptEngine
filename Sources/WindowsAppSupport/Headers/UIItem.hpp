#ifndef UIITEM_HPP
#define UIITEM_HPP

#include "Geometry.hpp"

#include <windows.h>

namespace UI
{

enum class MouseButton
{
	Left,
	Middle,
	Right
};

class Rect
{
public:
	Rect ();
	Rect (int x, int y, int width, int height);

	int x;
	int y;
	int width;
	int height;
};

class Keys
{
public:
	enum Type
	{
		ControlKey	= 1,
		ShiftKey	= 2
	};

	Keys ();
	Keys (int types);
	~Keys ();
	
	bool	IsKeyPressed (int keyTypes) const;

private:
	int types;
};

class Item
{
public:
	Item ();
	virtual ~Item ();

	virtual Rect	GetRect () const = 0;
	virtual void	SetRect (const Rect& rect) = 0;
};

class WindowItem : public Item
{
public:
	WindowItem ();
	virtual ~WindowItem ();

	virtual Rect	GetRect () const override;
	virtual void	SetRect (const Rect& rect) override;

	void			SetWindowHandle (HWND newWindowHandle);
	HWND			GetWindowHandle () const;

private:
	HWND windowHandle;

};

Keys GetKeysFromEvent (WPARAM wParam);

}

#endif
