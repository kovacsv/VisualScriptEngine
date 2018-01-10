#ifndef CUSTOMCONTROL_HPP
#define CUSTOMCONTROL_HPP

#include "UIItem.hpp"

#include <windows.h>

namespace UI
{

class CustomControl : public EventBasedItem
{
public:
	CustomControl ();
	virtual ~CustomControl ();

	bool Init (HWND parentHandle, int x, int y, int width, int height);

private:
	virtual void	OnCreate (HWND hwnd) override;
	virtual void	OnPaint (HWND hwnd) override;
	virtual void	OnMouseDown (HWND hwnd, Keys keys, MouseButton button, int x, int y) override;
	virtual void	OnMouseUp (HWND hwnd, Keys keys, MouseButton button, int x, int y) override;
	virtual void	OnMouseMove (HWND hwnd, Keys keys, int x, int y) override;
	virtual void	OnMouseWheel (HWND hwnd, Keys keys, int x, int y, int delta) override;
	virtual void	OnMouseDoubleClick (HWND hwnd, Keys keys, MouseButton button, int x, int y) override;
	virtual void	OnResize (HWND hwnd, int newWidth, int newHeight) override;
};

}

#endif
