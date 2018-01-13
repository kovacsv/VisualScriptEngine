#ifndef CUSTOMCONTROL_HPP
#define CUSTOMCONTROL_HPP

#include "UIItem.hpp"

#include <windows.h>

namespace UI
{

class CustomControl : public WindowItem
{
public:
	CustomControl ();
	virtual ~CustomControl ();

	bool			Init (HWND parentHandle, int x, int y, int width, int height);

	virtual void	OnCreate (HWND hwnd);
	virtual void	OnPaint (HWND hwnd);
	virtual void	OnMouseDown (HWND hwnd, Keys keys, MouseButton button, int x, int y);
	virtual void	OnMouseUp (HWND hwnd, Keys keys, MouseButton button, int x, int y);
	virtual void	OnMouseMove (HWND hwnd, Keys keys, int x, int y);
	virtual void	OnMouseWheel (HWND hwnd, Keys keys, int x, int y, int delta);
	virtual void	OnMouseDoubleClick (HWND hwnd, Keys keys, MouseButton button, int x, int y);
	virtual void	OnResize (HWND hwnd, int newWidth, int newHeight);
};

}

#endif
