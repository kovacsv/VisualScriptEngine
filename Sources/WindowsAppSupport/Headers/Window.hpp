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

	bool			Open (const std::wstring& windowTitle, int width, int height);
	void			Close ();

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
	
	virtual void	OnIdle (HWND hwnd);
};


}

#endif
