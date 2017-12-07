#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "UIItem.hpp"

#include <windows.h>
#include <gdiplus.h>
#include <string>

class ApplicationInitData
{
public:
	ApplicationInitData (const std::wstring& applicationClassName, const std::wstring& applicationWindowTitle, int width, int height);

	std::wstring	applicationClassName;
	std::wstring	applicationWindowTitle;
	int				width;
	int				height;
};

class Application : public UI::Item
{
public:
	Application ();
	virtual ~Application ();

	bool	Init (const ApplicationInitData& initData);
	void	Shut ();

private:
	static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

#endif
