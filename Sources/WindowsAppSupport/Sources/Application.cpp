#include "Application.hpp"
#include <gdiplusbase.h>
#include <assert.h>

class GdiplusInitializer
{
public:
	GdiplusInitializer ()
	{
		Gdiplus::GdiplusStartupInput input;
		Gdiplus::GdiplusStartupOutput output;
		Gdiplus::Status status = Gdiplus::GdiplusStartup (&gdiPlusToken, &input, &output);
		assert (status == Gdiplus::Ok);
	}

	~GdiplusInitializer ()
	{
		Gdiplus::GdiplusShutdown (gdiPlusToken);
	}

private:
	ULONG_PTR	gdiPlusToken;
};

static GdiplusInitializer gdiplusInitializer;

LRESULT CALLBACK Application::StaticWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	}

	Application* app = (Application*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
	if (app == nullptr) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	UI::TranslateEventToItem (app, hwnd, msg, wParam, lParam);
	switch (msg) {
		case WM_COMMAND:
			{
				int commandId  = LOWORD (wParam);
			}
			break;
		case WM_CLOSE:
			DestroyWindow (hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage (0);
			break;
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
}

ApplicationInitData::ApplicationInitData (const std::wstring& applicationClassName, const std::wstring& applicationWindowTitle, int width, int height) :
	applicationClassName (applicationClassName),
	applicationWindowTitle (applicationWindowTitle),
	width (width),
	height (height)
{

}

Application::Application ()
{
}

Application::~Application ()
{
}

bool Application::Init (const ApplicationInitData& initData)
{
	WNDCLASSEX windowClass;
	memset (&windowClass, 0, sizeof (WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0;
	windowClass.lpfnWndProc = Application::StaticWindowProc;
	windowClass.style = CS_DBLCLKS;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = NULL;
	windowClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = initData.applicationClassName.c_str ();
	windowClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);

	if (!RegisterClassEx (&windowClass)) {
		return false;
	}

	RECT requiredRect = { 0, 0, initData.width, initData.height };
	AdjustWindowRect (&requiredRect, WS_OVERLAPPEDWINDOW, false);

	windowHandle = CreateWindowEx (
		WS_EX_WINDOWEDGE | WS_CLIPCHILDREN, windowClass.lpszClassName, initData.applicationWindowTitle.c_str (), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, requiredRect.right - requiredRect.left, requiredRect.bottom - requiredRect.top, NULL, NULL, NULL, this
	);

	if (windowHandle == NULL) {
		return false;
	}

	ShowWindow (windowHandle, SW_SHOW);
	UpdateWindow (windowHandle);

	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0)) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
		OnIdle (windowHandle);
	}
	return true;
}

void Application::Shut ()
{
	PostQuitMessage (0);
}

void Application::OnCreate (HWND hwnd)
{

}

void Application::OnPaint (HWND hwnd)
{

}

void Application::OnMouseDown (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{

}

void Application::OnMouseUp (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{

}

void Application::OnMouseMove (HWND hwnd, UI::Keys keys, int x, int y)
{

}

void Application::OnMouseWheel (HWND hwnd, UI::Keys keys, int x, int y, int delta)
{

}

void Application::OnMouseDoubleClick (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{

}

void Application::OnMenuCommand (HWND hwnd, int commandId)
{

}

void Application::OnResize (HWND hwnd, int newWidth, int newHeight)
{

}

void Application::OnIdle (HWND hwnd)
{

}
