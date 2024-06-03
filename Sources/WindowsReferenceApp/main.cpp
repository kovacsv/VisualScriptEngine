#include "NE_Debug.hpp"
#include "NUIE_VersionInfo.hpp"
#include "Application.hpp"
#include "ResourceIds.hpp"
#include "CommandIds.hpp"

#ifdef _MSC_VER
#pragma comment (lib, "gdiplus.lib")
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "windowscodecs.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

LRESULT CALLBACK ApplicationWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	} else if (msg == WM_DESTROY) {
		SetWindowLongPtr (hwnd, GWLP_USERDATA, 0);
		PostQuitMessage (0);
	}

	Application* application = (Application*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
	if (application == nullptr) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	switch (msg) {
		case WM_CREATE:
			{
				application->Init (hwnd);
			}
			break;
		case WM_CLOSE:
			{
				if (application->Close (hwnd)) {
					DestroyWindow (hwnd);
				}
				return 0;
			}
		case WM_ERASEBKGND:
			{
				return 0;
			}
		case WM_GETMINMAXINFO:
			{
				LPMINMAXINFO minMaxInfo = (LPMINMAXINFO) lParam;
				minMaxInfo->ptMinTrackSize.x = 600;
				minMaxInfo->ptMinTrackSize.y = 400;
			}
			break;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				
				application->OnResize (0, 0, newWidth, newHeight);
			}
			break;
		case WM_CLIPBOARDUPDATE:
			{
				application->OnClipboardChanged ();
			}
			break;
		case WM_COMMAND:
			{
				WORD command = LOWORD (wParam);
				switch (command) {
					case FILE_NEW:
						application->New (hwnd);
						break;
					case OPEN_FILE:
						application->Open (hwnd);
						break;
					case FILE_SAVE:
						application->Save (hwnd);
						break;
					case EDIT_UNDO:
						application->ExecuteCommand (NUIE::CommandCode::Undo);
						break;
					case EDIT_REDO:
						application->ExecuteCommand (NUIE::CommandCode::Redo);
						break;
					case EDIT_SETTINGS:
						application->ExecuteCommand (NUIE::CommandCode::SetParameters);
						break;
					case EDIT_COPY:
						application->ExecuteCommand (NUIE::CommandCode::Copy);
						break;
					case EDIT_PASTE:
						application->ExecuteCommand (NUIE::CommandCode::Paste);
						break;
					case EDIT_DELETE:
						application->ExecuteCommand (NUIE::CommandCode::Delete);
						break;
					case EDIT_GROUP:
						application->ExecuteCommand (NUIE::CommandCode::Group);
						break;
					case EDIT_UNGROUP:
						application->ExecuteCommand (NUIE::CommandCode::Ungroup);
						break;
					case FILE_QUIT:
						SendMessage (hwnd, WM_CLOSE, 0, 0);
						break;
				}
			}
			break;
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
}

#ifdef _MSC_VER
int wWinMain (HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
#else
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE, LPSTR, int)
#endif
{
	EnableLeakDetection ();
	InitCommonControls ();

	Application application;

	WNDCLASSEX windowClass;
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));
	windowClass.cbSize = sizeof (WNDCLASSEX);
	windowClass.style = CS_DBLCLKS;
	windowClass.lpfnWndProc = ApplicationWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE (APPLICATION_ICON));
	windowClass.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE (APPLICATION_ICON));
	windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"VisualScriptEngineDemo";

	if (!RegisterClassEx (&windowClass)) {
		return 1;
	}

	RECT requiredRect = { 0, 0, 1200, 800 };
	AdjustWindowRect (&requiredRect, WS_OVERLAPPEDWINDOW, false);

	std::wstring appName = L"Visual Script Engine Demo";
	std::wstring appTitle = appName + L" " + VSE_VERSION_STRING;
	HWND windowHandle = CreateWindowEx (
		WS_EX_WINDOWEDGE | WS_CLIPCHILDREN, windowClass.lpszClassName, appTitle.c_str (), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, requiredRect.right - requiredRect.left, requiredRect.bottom - requiredRect.top, NULL, NULL, NULL, &application
	);

	if (windowHandle == NULL) {
		return 1;
	}

	AddClipboardFormatListener (windowHandle);
	ShowWindow (windowHandle, SW_SHOW);
	UpdateWindow (windowHandle);

	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0)) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	RemoveClipboardFormatListener (windowHandle);
	return 0;
}
