#include "CustomControl.hpp"
#include "WindowsAppUtilities.hpp"
#include "Debug.hpp"

#include <windowsx.h>

namespace UI
{

static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static SetCaptureHandler setCaptureHandler;

	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	}

	CustomControl* control = (CustomControl*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
	if (control == nullptr) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	switch (msg) {
		case WM_CREATE:
			control->OnCreate (hwnd);
			break;
		case WM_PAINT:
			control->OnPaint (hwnd);
			break;
		case WM_LBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseDown (hwnd, GetKeysFromEvent (wParam), MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseDown (hwnd, GetKeysFromEvent (wParam), MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseDown (hwnd, GetKeysFromEvent (wParam), MouseButton::Right, x, y);
			}
			break;
		case WM_LBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseUp (hwnd, GetKeysFromEvent (wParam), MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseUp (hwnd, GetKeysFromEvent (wParam), MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseUp (hwnd, GetKeysFromEvent (wParam), MouseButton::Right, x, y);
			}
			break;
		case WM_MOUSEMOVE:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseMove (hwnd, GetKeysFromEvent (wParam), x, y);
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
				control->OnMouseWheel (hwnd, GetKeysFromEvent (wParam), mousePos.x, mousePos.y, delta);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseDoubleClick (hwnd, GetKeysFromEvent (wParam), MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseDoubleClick (hwnd, GetKeysFromEvent (wParam), MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->OnMouseDoubleClick (hwnd, GetKeysFromEvent (wParam), MouseButton::Right, x, y);
			}
			break;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				control->OnResize (hwnd, newWidth, newHeight);
			}
			break;
	}
	return DefWindowProc (hwnd, msg, wParam, lParam);
}

CustomControl::CustomControl () :
	Item ()
{

}

CustomControl::~CustomControl ()
{

}

bool CustomControl::Init (HWND parentHandle, int x, int y, int width, int height)
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
	windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"CustomControl";
	
	RegisterClassEx (&windowClass);
	windowHandle = CreateWindowEx (
		0, windowClass.lpszClassName, L"", WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, parentHandle, NULL, NULL, this
	);

	if (windowHandle == NULL) {
		return false;
	}

	return true;
}

void CustomControl::OnCreate (HWND hwnd)
{

}

void CustomControl::OnPaint (HWND hwnd)
{

}

void CustomControl::OnMouseDown (HWND hwnd, Keys keys, MouseButton button, int x, int y)
{

}

void CustomControl::OnMouseUp (HWND hwnd, Keys keys, MouseButton button, int x, int y)
{

}

void CustomControl::OnMouseMove (HWND hwnd, Keys keys, int x, int y)
{

}

void CustomControl::OnMouseWheel (HWND hwnd, Keys keys, int x, int y, int delta)
{

}

void CustomControl::OnMouseDoubleClick (HWND hwnd, Keys keys, MouseButton button, int x, int y)
{

}

void CustomControl::OnResize (HWND hwnd, int newWidth, int newHeight)
{

}

}
