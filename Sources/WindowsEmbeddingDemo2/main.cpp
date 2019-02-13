#include "NUIE_NodeEditor.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_HwndEventHandlers.hpp"
#include "WAS_HwndNodeUIEnvironment.hpp"
#include "WAS_ParameterDialog.hpp"
#include "BI_BuiltInNodes.hpp"

#include <windows.h>
#include <windowsx.h>

static WAS::HwndNodeUIEnvironment uiEnvironment (
	NE::StringSettingsPtr (new NE::BasicStringSettings (NE::GetDefaultStringSettings ())),
	NUIE::SkinParamsPtr (new NUIE::BasicSkinParams (NUIE::GetDefaultSkinParams ())),
	nullptr
);
static NUIE::NodeEditor nodeEditor (uiEnvironment);

#define MENU_FILE_NEW	1000
#define MENU_FILE_QUIT	1001
#define MENU_EDIT_UNDO	2000
#define MENU_EDIT_REDO	2001

static void CreateMenuBar (HWND hwnd)
{
	HMENU menuBar = CreateMenu ();
	
	HMENU fileMenu = CreateMenu ();
	AppendMenu (fileMenu, MF_STRING, MENU_FILE_NEW, L"New");
	AppendMenu (fileMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu (fileMenu, MF_STRING, MENU_FILE_QUIT, L"Quit");
	AppendMenu (menuBar, MF_POPUP, (UINT_PTR) fileMenu, L"File");

	HMENU editMenu = CreateMenu ();
	AppendMenu (editMenu, MF_STRING, MENU_EDIT_UNDO, L"Undo");
	AppendMenu (editMenu, MF_STRING, MENU_EDIT_REDO, L"Redo");
	AppendMenu (menuBar, MF_POPUP, (UINT_PTR) editMenu, L"Edit");

	SetMenu (hwnd, menuBar);
}

LRESULT CALLBACK ApplicationWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_CREATE:
			{
				uiEnvironment.Init (&nodeEditor, hwnd);

				uiEnvironment.RegisterNode (L"Input Nodes", L"Boolean", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::BooleanNode (L"Boolean", position, true));
				});
				uiEnvironment.RegisterNode (L"Input Nodes", L"Integer", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::IntegerUpDownNode (L"Integer", position, 0, 5));
				});
				uiEnvironment.RegisterNode (L"Input Nodes", L"Number", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", position, 0.0, 5.0));
				});
				uiEnvironment.RegisterNode (L"Input Nodes", L"Integer Range", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::IntegerRangeNode (L"Integer Range", position));
				});
				uiEnvironment.RegisterNode (L"Input Nodes", L"Number Range", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::DoubleRangeNode (L"Number Range", position));
				});
				uiEnvironment.RegisterNode (L"Arithmetic Nodes", L"Addition", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::AdditionNode (L"Addition", position));
				});
				uiEnvironment.RegisterNode (L"Arithmetic Nodes", L"Subtraction", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::SubtractionNode (L"Subtraction", position));
				});
				uiEnvironment.RegisterNode (L"Arithmetic Nodes", L"Multiplication", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::MultiplicationNode (L"Multiplication", position));
				});
				uiEnvironment.RegisterNode (L"Arithmetic Nodes", L"Division", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::DivisionNode (L"Division", position));
				});
				uiEnvironment.RegisterNode (L"Other Nodes", L"Viewer", [&] (const NUIE::Point& position) {
					return NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", position, 5));
				});

				nodeEditor.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", NUIE::Point (100, 100), 20, 10)));
				nodeEditor.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", NUIE::Point (100, 300), 20, 10)));
				nodeEditor.AddNode (NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", NUIE::Point (300, 200), 5)));
				nodeEditor.Update ();

				CreateMenuBar (hwnd);
			}
			break;
		case WM_CLOSE:
			DestroyWindow (hwnd);
			break;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				uiEnvironment.OnResize (0, 0, newWidth, newHeight);
			}
			break;
		case WM_COMMAND:
			{
				WORD commandId = LOWORD (wParam);
				switch (commandId) {
					case MENU_FILE_NEW:
						nodeEditor.New ();
						break;
					case MENU_FILE_QUIT:
						PostQuitMessage (0);
						break;
					case MENU_EDIT_UNDO:
						nodeEditor.Undo ();
						break;
					case MENU_EDIT_REDO:
						nodeEditor.Redo ();
						break;
				}
			}
			break;
		case WM_DESTROY:
			PostQuitMessage (0);
			break;
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
}

int wWinMain (HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	WNDCLASSEX windowClass;
	memset (&windowClass, 0, sizeof (WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0;
	windowClass.lpfnWndProc = ApplicationWindowProc;
	windowClass.style = CS_DBLCLKS;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"WindowsEmbeddingDemo";
	windowClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);

	if (!RegisterClassEx (&windowClass)) {
		return false;
	}

	RECT requiredRect = { 0, 0, 800, 600 };
	AdjustWindowRect (&requiredRect, WS_OVERLAPPEDWINDOW, false);

	HWND windowHandle = CreateWindowEx (
		WS_EX_WINDOWEDGE | WS_CLIPCHILDREN, windowClass.lpszClassName, L"Node Engine Embedding Demo", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, requiredRect.right - requiredRect.left, requiredRect.bottom - requiredRect.top, NULL, NULL, NULL, nullptr
	);

	if (windowHandle == NULL) {
		return 1;
	}

	ShowWindow (windowHandle, SW_SHOW);
	UpdateWindow (windowHandle);

	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0)) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	return 0;
}
