#include "UIItem.hpp"
#include "Window.hpp"
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

HWND Item::GetWindowHandle () const
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
