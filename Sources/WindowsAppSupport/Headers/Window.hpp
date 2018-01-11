#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "UIItem.hpp"

#include <windows.h>
#include <string>

namespace UI
{

class Window : public Item
{
public:
	Window ();
	virtual ~Window ();

	bool			Open (const std::wstring& windowTitle, int x, int y, int width, int height);
	void			Close ();

	virtual void	OnCreate (HWND hwnd);
	virtual void	OnPaint (HWND hwnd);
	virtual void	OnMouseDown (HWND hwnd, Keys keys, MouseButton button, int x, int y);
	virtual void	OnMouseUp (HWND hwnd, Keys keys, MouseButton button, int x, int y);
	virtual void	OnMouseMove (HWND hwnd, Keys keys, int x, int y);
	virtual void	OnMouseWheel (HWND hwnd, Keys keys, int x, int y, int delta);
	virtual void	OnMouseDoubleClick (HWND hwnd, Keys keys, MouseButton button, int x, int y);
	virtual void	OnResize (HWND hwnd, int newWidth, int newHeight);

	virtual void	OnClose (HWND hwnd);
	virtual void	OnDestroy (HWND hwnd);
	virtual void	OnMenuCommand (HWND hwnd, int commandId);
};

}

#endif
