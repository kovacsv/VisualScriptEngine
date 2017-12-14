#ifndef CUSTOMCONTROL_HPP
#define CUSTOMCONTROL_HPP

#include "UIItem.hpp"

#include <windows.h>

class CustomControl : public UI::Item
{
public:
	CustomControl ();
	virtual ~CustomControl ();

	bool Init (HWND parentHandle, int x, int y, int width, int height);

private:
	virtual void	OnCreate (HWND hwnd) override;
	virtual void	OnPaint (HWND hwnd) override;
	virtual void	OnMouseDown (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y) override;
	virtual void	OnMouseUp (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y) override;
	virtual void	OnMouseMove (HWND hwnd, UI::Keys keys, int x, int y) override;
	virtual void	OnMouseWheel (HWND hwnd, UI::Keys keys, int x, int y, int delta) override;
	virtual void	OnMouseDoubleClick (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y) override;
	virtual void	OnMenuCommand (HWND hwnd, int commandId) override;
	virtual void	OnResize (HWND hwnd, int newWidth, int newHeight) override;
};


#endif
