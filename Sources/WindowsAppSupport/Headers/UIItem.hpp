#ifndef UIITEM_HPP
#define UIITEM_HPP

#include <windows.h>

namespace UI
{

enum class MouseButton
{
	Left,
	Middle,
	Right
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

	virtual void	MoveResize (int x, int y, int width, int height) = 0;
};

class WindowItem : public Item
{
public:
	WindowItem ();
	virtual ~WindowItem ();

	virtual void	MoveResize (int x, int y, int width, int height) override;

	void			SetWindowHandle (HWND newWindowHandle);
	HWND			GetWindowHandle () const;

private:
	HWND windowHandle;

};

Keys GetKeysFromEvent (WPARAM wParam);

}

#endif
