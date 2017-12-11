#include "UIItem.hpp"
#include "WindowsAppUtilities.hpp"

#include <windowsx.h>

namespace UI
{

Keys::Keys () :
	types (0)
{

}

Keys::Keys (int types) :
	types (types)
{

}

Keys::~Keys ()
{

}

bool Keys::IsKeyPressed (int keyTypes) const
{
	return (types & keyTypes) != 0;
}

Item::Item () :
	windowHandle (NULL)
{

}

Item::~Item ()
{

}

void Item::MoveResize (int x, int y, int width, int height)
{
	MoveWindow (windowHandle, x, y, width, height, TRUE);
}

static Keys GetKeysFromEvent (WPARAM wParam)
{
	int flags = 0;
	if (wParam & MK_CONTROL) {
		flags |= Keys::ControlKey;
	}
	if (wParam & MK_SHIFT) {
		flags |= Keys::ShiftKey;
	}
	return Keys (flags);
}

void TranslateEventToItem (Item* item, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static SetCaptureHandler setCaptureHandler;

	switch (msg) {
		case WM_CREATE:
			item->OnCreate (hwnd);
			break;
		case WM_PAINT:
			item->OnPaint (hwnd);
			break;
		case WM_LBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseDown (hwnd, GetKeysFromEvent (wParam), MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseDown (hwnd, GetKeysFromEvent (wParam), MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseDown (hwnd, GetKeysFromEvent (wParam), MouseButton::Right, x, y);
			}
			break;
		case WM_LBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseUp (hwnd, GetKeysFromEvent (wParam), MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseUp (hwnd, GetKeysFromEvent (wParam), MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseUp (hwnd, GetKeysFromEvent (wParam), MouseButton::Right, x, y);
			}
			break;
		case WM_MOUSEMOVE:
			{
				SetFocus (hwnd); // before Windows 10 only the focused window catches the mouse wheel message
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseMove (hwnd, GetKeysFromEvent (wParam), x, y);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				POINT mousePos;
				mousePos.x = GET_X_LPARAM (lParam);
				mousePos.y = GET_Y_LPARAM (lParam);
				ScreenToClient (hwnd, &mousePos);
				int delta = GET_WHEEL_DELTA_WPARAM (wParam);
				item->OnMouseWheel (hwnd, GetKeysFromEvent (wParam), mousePos.x, mousePos.y, delta);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseDoubleClick (hwnd, GetKeysFromEvent (wParam), MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseDoubleClick (hwnd, GetKeysFromEvent (wParam), MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				item->OnMouseDoubleClick (hwnd, GetKeysFromEvent (wParam), MouseButton::Right, x, y);
			}
			break;
		case WM_COMMAND:
			{
				if (HIWORD (wParam) == 0) {
					int commandId = LOWORD (wParam);
					item->OnMenuCommand (hwnd, commandId);
					break;
				}
			}
			break;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				item->OnResize (hwnd, newWidth, newHeight);
			}
			break;
		case WM_CLOSE:
			DestroyWindow (hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage (0);
			break;
	}
}

}
