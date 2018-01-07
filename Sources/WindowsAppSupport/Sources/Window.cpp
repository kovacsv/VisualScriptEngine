#include "Window.hpp"

namespace UI
{

static std::wstring WindowClassName = L"UIWindowClass";

static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	}

	Window* window = (Window*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
	if (window == nullptr) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	UI::TranslateEventToItem (window, hwnd, msg, wParam, lParam);
	switch (msg) {
		case WM_CLOSE:
			window->OnClose (hwnd);
			break;
		case WM_DESTROY:
			window->OnDestroy (hwnd);
			break;
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
}

Window::Window () :
	Item ()
{
}

Window::~Window ()
{
}

bool Window::Open (const std::wstring& windowTitle, int x, int y, int width, int height)
{
	WNDCLASSEX windowClass;
	memset (&windowClass, 0, sizeof (WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0;
	windowClass.lpfnWndProc = StaticWindowProc;
	windowClass.style = CS_DBLCLKS;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = NULL;
	windowClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = WindowClassName.c_str ();
	windowClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);

	RegisterClassEx (&windowClass);

	RECT requiredRect = { x, y, x + width, y + height };
	AdjustWindowRect (&requiredRect, WS_OVERLAPPEDWINDOW, false);

	windowHandle = CreateWindowEx (
		WS_EX_WINDOWEDGE, windowClass.lpszClassName, windowTitle.c_str (), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		x, y, requiredRect.right - requiredRect.left, requiredRect.bottom - requiredRect.top, NULL, NULL, NULL, this
	);

	if (windowHandle == NULL) {
		return false;
	}

	return true;
}

void Window::Close ()
{
	SendMessage (windowHandle, WM_CLOSE, NULL, NULL);
}

void Window::OnClose (HWND hwnd)
{
	DestroyWindow (hwnd);
}

void Window::OnDestroy (HWND hwnd)
{
	PostQuitMessage (0);
}

void Window::OnCreate (HWND hwnd)
{

}

void Window::OnPaint (HWND hwnd)
{

}

void Window::OnMouseDown (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{

}

void Window::OnMouseUp (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{

}

void Window::OnMouseMove (HWND hwnd, UI::Keys keys, int x, int y)
{

}

void Window::OnMouseWheel (HWND hwnd, UI::Keys keys, int x, int y, int delta)
{

}

void Window::OnMouseDoubleClick (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{

}

void Window::OnMenuCommand (HWND hwnd, int commandId)
{

}

void Window::OnResize (HWND hwnd, int newWidth, int newHeight)
{

}

}
