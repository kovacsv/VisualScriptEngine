#include "UIItem.hpp"

#include <windowsx.h>

namespace UI
{

Rect::Rect () :
	Rect (0, 0, 0, 0)
{

}

Rect::Rect (int x, int y, int width, int height) :
	x (x),
	y (y),
	width (width),
	height (height)
{

}

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

Item::Item ()
{

}

Item::~Item ()
{

}

WindowItem::WindowItem () :
	Item (),
	windowHandle (NULL)
{

}

WindowItem::~WindowItem ()
{

}

Rect WindowItem::GetRect () const
{
	Rect result;

	RECT windowRect;
	GetWindowRect (windowHandle, &windowRect);

	POINT position;
	position.x = windowRect.left;
	position.y = windowRect.top;
	ScreenToClient (GetParent (windowHandle), &position);

	result.x = position.x;
	result.y = position.y;
	result.width = windowRect.right - windowRect.left;
	result.height = windowRect.bottom - windowRect.top;

	return result;
}

void WindowItem::SetRect (const Rect& rect)
{
	MoveWindow (windowHandle, rect.x, rect.y, rect.width, rect.height, TRUE);
}

void WindowItem::SetWindowHandle (HWND newWindowHandle)
{
	windowHandle = newWindowHandle;
}

HWND WindowItem::GetWindowHandle () const
{
	return windowHandle;
}

Keys GetKeysFromEvent (WPARAM wParam)
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

}
