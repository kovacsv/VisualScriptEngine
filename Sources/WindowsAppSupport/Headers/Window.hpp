#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "UIItem.hpp"

#include <windows.h>
#include <string>

namespace UI
{

class Window : public EventBasedItem
{
public:
	Window ();
	virtual ~Window ();

	bool			Open (const std::wstring& windowTitle, int x, int y, int width, int height);
	void			Close ();

	virtual void	OnClose (HWND hwnd);
	virtual void	OnDestroy (HWND hwnd);
	virtual void	OnMenuCommand (HWND hwnd, int commandId);

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
