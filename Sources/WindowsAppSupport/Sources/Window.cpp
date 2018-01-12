#include "Window.hpp"

#include <windowsx.h>

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

	switch (msg) {
		case WM_CREATE:
			window->SetWindowHandle (hwnd);
			window->OnCreate (hwnd);
			break;
		case WM_PAINT:
			window->OnPaint (hwnd);
			break;
		case WM_LBUTTONDOWN:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseDown (hwnd, GetKeysFromEvent (wParam), MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseDown (hwnd, GetKeysFromEvent (wParam), MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseDown (hwnd, GetKeysFromEvent (wParam), MouseButton::Right, x, y);
			}
			break;
		case WM_LBUTTONUP:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseUp (hwnd, GetKeysFromEvent (wParam), MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONUP:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseUp (hwnd, GetKeysFromEvent (wParam), MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONUP:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseUp (hwnd, GetKeysFromEvent (wParam), MouseButton::Right, x, y);
			}
			break;
		case WM_MOUSEMOVE:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseMove (hwnd, GetKeysFromEvent (wParam), x, y);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				// TODO: before Windows 10 only the focused control catches the mouse wheel message
				POINT mousePos;
				mousePos.x = GET_X_LPARAM (lParam);
				mousePos.y = GET_Y_LPARAM (lParam);
				ScreenToClient (hwnd, &mousePos);
				int delta = GET_WHEEL_DELTA_WPARAM (wParam);
				window->OnMouseWheel (hwnd, GetKeysFromEvent (wParam), mousePos.x, mousePos.y, delta);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseDoubleClick (hwnd, GetKeysFromEvent (wParam), MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseDoubleClick (hwnd, GetKeysFromEvent (wParam), MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				window->OnMouseDoubleClick (hwnd, GetKeysFromEvent (wParam), MouseButton::Right, x, y);
			}
			break;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				window->OnResize (hwnd, newWidth, newHeight);
			}
			break;
		case WM_COMMAND:
			{
				if (HIWORD (wParam) == 0) {
					int commandId = LOWORD (wParam);
					window->OnMenuCommand (hwnd, commandId);
					break;
				}
			}
			break;
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

	HWND hwnd = CreateWindowEx (
		WS_EX_WINDOWEDGE, windowClass.lpszClassName, windowTitle.c_str (), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		x, y, requiredRect.right - requiredRect.left, requiredRect.bottom - requiredRect.top, NULL, NULL, NULL, this
	);

	if (hwnd == NULL) {
		return false;
	}

	return true;
}

void Window::Close ()
{
	SendMessage (GetWindowHandle (), WM_CLOSE, NULL, NULL);
}

void Window::OnClose (HWND hwnd)
{
	DestroyWindow (hwnd);
}

void Window::OnDestroy (HWND hwnd)
{
	PostQuitMessage (0);
}

void Window::OnMenuCommand (HWND hwnd, int commandId)
{

}

void Window::OnCreate (HWND hwnd)
{

}

void Window::OnPaint (HWND hwnd)
{

}

void Window::OnMouseDown (HWND hwnd, Keys keys, MouseButton button, int x, int y)
{

}

void Window::OnMouseUp (HWND hwnd, Keys keys, MouseButton button, int x, int y)
{

}

void Window::OnMouseMove (HWND hwnd, Keys keys, int x, int y)
{

}

void Window::OnMouseWheel (HWND hwnd, Keys keys, int x, int y, int delta)
{

}

void Window::OnMouseDoubleClick (HWND hwnd, Keys keys, MouseButton button, int x, int y)
{

}

void Window::OnResize (HWND hwnd, int newWidth, int newHeight)
{

}

}
