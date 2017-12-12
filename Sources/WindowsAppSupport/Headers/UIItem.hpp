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

	void			MoveResize (int x, int y, int width, int height);
	HWND			GetWindowHandle () const;

	virtual void	OnCreate (HWND hwnd) = 0;
	virtual void	OnPaint (HWND hwnd) = 0;
	virtual void	OnMouseDown (HWND hwnd, Keys keys, MouseButton button, int x, int y) = 0;
	virtual void	OnMouseUp (HWND hwnd, Keys keys, MouseButton button, int x, int y) = 0;
	virtual void	OnMouseMove (HWND hwnd, Keys keys, int x, int y) = 0;
	virtual void	OnMouseWheel (HWND hwnd, Keys keys, int x, int y, int delta) = 0;
	virtual void	OnMouseDoubleClick (HWND hwnd, Keys keys, MouseButton button, int x, int y) = 0;
	virtual void	OnMenuCommand (HWND hwnd, int commandId) = 0;
	virtual void	OnResize (HWND hwnd, int newWidth, int newHeight) = 0;

protected:
	HWND windowHandle;
};

void TranslateEventToItem (Item* item, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

}

#endif
