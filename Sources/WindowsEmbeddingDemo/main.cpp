#include "NUIE_NodeEditor.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_HwndEventHandlers.hpp"
#include "WAS_NodeEditorHwndControl.hpp"
#include "WAS_ParameterDialog.hpp"

#include "BI_BuiltInNodes.hpp"

#include <windows.h>
#include <windowsx.h>

#include "WAS_Direct2DContext.hpp"

#pragma comment (lib, "windowscodecs.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

static const NUIE::BasicSkinParams MySkinParams (
	/*backgroundColor*/ NUIE::Color (240, 240, 240),
	/*connectionLinePen*/ NUIE::Pen (NUIE::Color (0, 0, 0), 1.0),
	/*nodePadding*/ 5.0,
	/*nodeBorderPen*/ NUIE::Pen (NUIE::Color (0, 0, 0), 1.0),
	/*nodeHeaderTextFont*/ NUIE::Font (L"Arial", 16.0),
	/*nodeHeaderTextColor*/ NUIE::Color (255, 255, 255),
	/*nodeHeaderErrorTextColor*/ NUIE::Color (255, 255, 255),
	/*nodeHeaderBackgroundColor*/ NUIE::Color (100, 100, 100),
	/*nodeHeaderErrorBackgroundColor*/ NUIE::Color (225, 0, 0),
	/*nodeTextFont*/ NUIE::Font (L"Arial", 14.0),
	/*nodeContentTextColor*/ NUIE::Color (0, 0, 0),
	/*nodeContentBackgroundColor*/ NUIE::Color (200, 200, 200),
	/*slotTextColor*/ NUIE::Color (0, 0, 0),
	/*slotTextBackgroundColor*/ NUIE::Color (225, 225, 225),
	/*needToDrawSlotCircles*/ true,
	/*slotCircleSize*/ NUIE::Size (8.0, 8.0),
	/*selectionBlendColor*/ NUIE::BlendColor (NUIE::Color (240, 240, 240), 0.5),
	/*disabledBlendColor*/ NUIE::BlendColor (NUIE::Color (0, 138, 184), 0.2),
	/*selectionRectPen*/ NUIE::Pen (NUIE::Color (0, 138, 184), 1.0),
	/*nodeSelectionRectPen*/ NUIE::Pen (NUIE::Color (0, 138, 184), 5.0),
	/*buttonBorderPen*/ NUIE::Pen (NUIE::Color (50, 75, 100), 1.0),
	/*buttonBackgroundColor*/ NUIE::Color (150, 175, 200),
	/*textPanelTextColor*/ NUIE::Color (0, 0, 0),
	/*textPanelBackgroundColor*/ NUIE::Color (255, 255, 100),
	/*groupNameFont*/ NUIE::Font (L"Arial", 18.0),
	/*groupNameColor*/ NUIE::Color (0, 0, 0),
	/*groupBackgroundColors*/ NUIE::NamedColorSet ({
		{ L"Blue", NUIE::Color (160, 200, 240) },
		{ L"Green", NUIE::Color (160, 239, 160) },
		{ L"Red", NUIE::Color (239, 189, 160) }
		}),
	/*groupPadding*/ 10.0
);

class MyResourceImageLoader : public WAS::Direct2DImageLoaderFromResource
{
	virtual HRSRC GetImageResHandle (const NUIE::IconId& iconId) override
	{
		HRSRC resHandle = FindResource (NULL, MAKEINTRESOURCE (iconId.GetId ()), L"IMAGE");
		return resHandle;
	}
};

static MyResourceImageLoader imageLoader;

static void AddNodeTreeItem (WAS::NodeTree& nodeTree, size_t groupIndex, const std::wstring& name, int iconIndex, const WAS::CreatorFunction& creator)
{
	nodeTree.AddItem (groupIndex, name, [=] (const NUIE::Point& position) {
		NUIE::UINodePtr node = creator (position);
		BI::BasicUINodePtr basicNode = std::dynamic_pointer_cast<BI::BasicUINode> (node);
		if (basicNode != nullptr) {
			basicNode->SetIconId (NUIE::IconId (iconIndex));
		}
		return node;
	});
}

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
		stringSettings (WAS::GetStringSettingsFromSystem ()),
		skinParams (MySkinParams),
		eventHandlers (),
		evaluationEnv (nullptr),
		nodeEditorControl (NUIE::NativeDrawingContextPtr (new WAS::Direct2DContext (&imageLoader)))
	{
	
	}

	void Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle)
	{
		WAS::NodeTree nodeTree;

		size_t inputNodes = nodeTree.AddGroup (L"Input Nodes");
		AddNodeTreeItem (nodeTree, inputNodes, L"Integer", 101, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerUpDownNode (L"Integer", position, 0, 5));
		});
		AddNodeTreeItem (nodeTree, inputNodes, L"Number", 102, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", position, 0.0, 5.0));
		});
		size_t arithmeticNodes = nodeTree.AddGroup (L"Arithmetic Nodes");
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Addition", 103, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::AdditionNode (L"Addition", position));
		});
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Subtraction", 104, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::SubtractionNode (L"Subtraction", position));
		});
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Multiplication", 105, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiplicationNode (L"Multiplication", position));
		});
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Division", 106, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DivisionNode (L"Division", position));
		});
		size_t otherNodes = nodeTree.AddGroup (L"Other Nodes");
		AddNodeTreeItem (nodeTree, otherNodes, L"Viewer", 107, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::ViewerNode (L"Viewer", position));
		});

		RECT clientRect;
		GetClientRect (parentHandle, &clientRect);
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;

		nodeEditorControl.Init (nodeEditorPtr, parentHandle, 0, 0, width, height);
		nodeEditorControl.FillNodeTree (nodeTree);
		eventHandlers.Init (&nodeEditorControl);
	}

	void OnResize (int x, int y, int width, int height)
	{
		nodeEditorControl.Resize (x, y, width, height);
	}

	virtual const NE::StringSettings& GetStringSettings () override
	{
		return stringSettings;
	}

	virtual const NUIE::SkinParams& GetSkinParams () override
	{
		return skinParams;
	}

	virtual NUIE::DrawingContext& GetDrawingContext () override
	{
		return nodeEditorControl.GetDrawingContext ();
	}

	virtual double GetWindowScale () override
	{
		return 1.0;
	}

	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
	}

	virtual void OnEvaluationBegin () override
	{

	}

	virtual void OnEvaluationEnd () override
	{

	}

	virtual void OnValuesRecalculated () override
	{
		
	}

	virtual void OnRedrawRequested () override
	{
		nodeEditorControl.Invalidate ();
	}

	virtual NUIE::EventHandlers& GetEventHandlers () override
	{
		return eventHandlers;
	}

	virtual double GetMouseMoveMinOffset () override
	{
		return 2.0;
	}

private:
	NE::BasicStringSettings				stringSettings;
	NUIE::BasicSkinParams				skinParams;
	WAS::HwndEventHandlers				eventHandlers;
	NE::EvaluationEnv					evaluationEnv;
	WAS::NodeEditorNodeTreeHwndControl	nodeEditorControl;
};

static MyNodeUIEnvironment uiEnvironment;
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
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));
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
