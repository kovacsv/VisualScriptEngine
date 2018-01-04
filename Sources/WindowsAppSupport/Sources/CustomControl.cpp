#include "CustomControl.hpp"
#include "Debug.hpp"

namespace UI
{

static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	}

	CustomControl* control = (CustomControl*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
	if (control == nullptr) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	UI::TranslateEventToItem (control, hwnd, msg, wParam, lParam);
	return DefWindowProc (hwnd, msg, wParam, lParam);
}

CustomControl::CustomControl ()
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

void CustomControl::OnMouseDown (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{

}

void CustomControl::OnMouseUp (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{

}

void CustomControl::OnMouseMove (HWND hwnd, UI::Keys keys, int x, int y)
{

}

void CustomControl::OnMouseWheel (HWND hwnd, UI::Keys keys, int x, int y, int delta)
{

}

void CustomControl::OnMouseDoubleClick (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{

}

void CustomControl::OnMenuCommand (HWND hwnd, int commandId)
{

}

void CustomControl::OnResize (HWND hwnd, int newWidth, int newHeight)
{

}

}
