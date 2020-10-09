#include "NUIE_NodeEditor.hpp"
#include "NUIE_NodeTree.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_HwndEventHandler.hpp"
#include "WAS_ParameterDialog.hpp"
#include "WAS_NodeTree.hpp"
#include "BI_BuiltInNodes.hpp"

#include <windows.h>
#include <windowsx.h>

static void InitNodeTree (NUIE::NodeTree& nodeTree)
{
	size_t inputNodes = nodeTree.AddGroup (L"Input Nodes");
	nodeTree.AddItem (inputNodes, L"Boolean", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::BooleanNode (NE::LocString (L"Boolean"), position, true));
	});
	nodeTree.AddItem (inputNodes, L"Integer", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 5));
	});
	nodeTree.AddItem (inputNodes, L"Number", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 5.0));
	});
	nodeTree.AddItem (inputNodes, L"Integer Increment", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position));
	});
	nodeTree.AddItem (inputNodes, L"Number Increment", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position));
	});
	nodeTree.AddItem (inputNodes, L"Number Distribution", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), position));
	});
	nodeTree.AddItem (inputNodes, L"List Builder", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::ListBuilderNode (NE::LocString (L"List Builder"), position));
	});
	size_t arithmeticNodes = nodeTree.AddGroup (L"Arithmetic Nodes");
	nodeTree.AddItem (arithmeticNodes, L"Addition", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position));
	});
	nodeTree.AddItem (arithmeticNodes, L"Subtraction", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position));
	});
	nodeTree.AddItem (arithmeticNodes, L"Multiplication", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position));
	});
	nodeTree.AddItem (arithmeticNodes, L"Division", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position));
	});
	size_t otherNodes = nodeTree.AddGroup (L"Other Nodes");
	nodeTree.AddItem (otherNodes, L"Viewer", [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), position, 5));
	});
}

class MyEventHandler : public WAS::HwndEventHandler
{
public:
	MyEventHandler () :
		WAS::HwndEventHandler (),
		nodeEditor (nullptr),
		nodeTree ()
	{
		InitNodeTree (nodeTree);
	}

	void SetNodeEditor (NUIE::NodeEditor* nodeEditorPtr)
	{
		nodeEditor = nodeEditorPtr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (NUIE::EventHandler::ContextMenuType type, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override
	{
		if (type == NUIE::EventHandler::ContextMenuType::EmptyArea) {
			NUIE::MenuCommandStructure finalCommands = commands;
			NUIE::AddNodeTreeToMenuStructure (nodeTree, position, nodeEditor, finalCommands);
			return WAS::SelectCommandFromContextMenu (hwnd, position, finalCommands);
		} else {
			return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
		}
	}

private:
	NUIE::NodeEditor*	nodeEditor;
	NUIE::NodeTree		nodeTree;
};

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
		stringConverter (NE::BasicStringConverter (WAS::GetStringSettingsFromSystem ())),
		skinParams (NUIE::GetDefaultSkinParams ()),
		eventHandler (),
		clipboardHandler (),
		evaluationEnv (nullptr),
		drawingContext (new WAS::BitmapContextGdi ()),
		nodeEditor (nullptr),
		editorHandle (nullptr)
	{

	}

	void Init (NUIE::NodeEditor* nodeEditorPtr, HWND editorWindowHandle)
	{
		nodeEditor = nodeEditorPtr;
		editorHandle = editorWindowHandle;

		drawingContext->Init (editorHandle);
		eventHandler.Init (editorHandle);
		eventHandler.SetNodeEditor (nodeEditor);
	}

	void OnResize (int width, int height)
	{
		drawingContext->Resize (width, height);
	}

	void OnPaint ()
	{
		nodeEditor->Draw ();
		drawingContext->BlitToWindow (editorHandle);
	}

	virtual const NE::StringConverter& GetStringConverter () override
	{
		return stringConverter;
	}

	virtual const NUIE::SkinParams& GetSkinParams () override
	{
		return skinParams;
	}

	virtual NUIE::DrawingContext& GetDrawingContext () override
	{
		return *drawingContext;
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
		InvalidateRect (editorHandle, NULL, FALSE);
	}

	virtual NUIE::EventHandler& GetEventHandler () override
	{
		return eventHandler;
	}

	virtual NUIE::ClipboardHandler& GetClipboardHandler () override
	{
		return clipboardHandler;
	}

	virtual void OnSelectionChanged (const NUIE::Selection&) override
	{

	}

private:
	NE::BasicStringConverter			stringConverter;
	NUIE::BasicSkinParams				skinParams;
	MyEventHandler						eventHandler;
	NUIE::MemoryClipboardHandler		clipboardHandler;
	NE::EvaluationEnv					evaluationEnv;
	NUIE::NativeDrawingContextPtr		drawingContext;

	NUIE::NodeEditor*					nodeEditor;
	HWND								editorHandle;
};

static MyNodeUIEnvironment uiEnvironment;
static NUIE::NodeEditor nodeEditor (uiEnvironment);

LRESULT CALLBACK ApplicationWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_CREATE:
			{
				uiEnvironment.Init (&nodeEditor, hwnd);
			}
			break;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				uiEnvironment.OnResize (newWidth, newHeight);
			}
			break;
		case WM_PAINT:
			uiEnvironment.OnPaint ();
			break;
		case WM_CLOSE:
			DestroyWindow (hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage (0);
			break;
		case WM_ERASEBKGND:
			return 0;
		case WM_LBUTTONDOWN:
			{
				WAS::SetWindowCapture (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDown (WAS::GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				WAS::SetWindowCapture (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDown (WAS::GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				WAS::SetWindowCapture (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDown (WAS::GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			break;
		case WM_LBUTTONUP:
			{
				WAS::ReleaseWindowCapture (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseUp (WAS::GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONUP:
			{
				WAS::ReleaseWindowCapture (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseUp (WAS::GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONUP:
			{
				WAS::ReleaseWindowCapture (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseUp (WAS::GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			break;
		case WM_MOUSEMOVE:
			{
				SetFocus (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseMove (WAS::GetModiferKeysFromEvent (wParam), x, y);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				POINT mousePos;
				mousePos.x = GET_X_LPARAM (lParam);
				mousePos.y = GET_Y_LPARAM (lParam);
				ScreenToClient (hwnd, &mousePos);
				int delta = GET_WHEEL_DELTA_WPARAM (wParam);
				NUIE::MouseWheelRotation rotation = delta > 0 ? NUIE::MouseWheelRotation::Forward : NUIE::MouseWheelRotation::Backward;
				nodeEditor.OnMouseWheel (WAS::GetModiferKeysFromEvent (wParam), rotation, mousePos.x, mousePos.y);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDoubleClick (WAS::GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDoubleClick (WAS::GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDoubleClick (WAS::GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			break;
		case WM_KEYDOWN:
			{
				NUIE::CommandCode commandCode = NUIE::CommandCode::Undefined;
				bool isControlPressed = (GetKeyState (VK_CONTROL) < 0);
				bool isShiftPressed = (GetKeyState (VK_SHIFT) < 0);
				if (isControlPressed) {
					switch (wParam) {
						case 'A':
							commandCode = NUIE::CommandCode::SelectAll;
							break;
						case 'C':
							commandCode = NUIE::CommandCode::Copy;
							break;
						case 'V':
							commandCode = NUIE::CommandCode::Paste;
							break;
						case 'G':
							if (isShiftPressed) {
								commandCode = NUIE::CommandCode::Ungroup;
							} else {
								commandCode = NUIE::CommandCode::Group;
							}
							break;
						case 'Z':
							if (isShiftPressed) {
								commandCode = NUIE::CommandCode::Redo;
							} else {
								commandCode = NUIE::CommandCode::Undo;
							}
							break;
					}
				} else {
					switch (wParam) {
						case VK_ESCAPE:
							commandCode = NUIE::CommandCode::Escape;
							break;
						case VK_DELETE:
						case VK_BACK:
							commandCode = NUIE::CommandCode::Delete;
							break;
					}
				}
				if (commandCode != NUIE::CommandCode::Undefined) {
					nodeEditor.ExecuteCommand (commandCode);
				}
			}
			break;
		case WM_CANCELMODE:
			WAS::ReleaseWindowCapture (hwnd);
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
	windowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"WindowsEmbeddingDemo";
	windowClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);

	if (!RegisterClassEx (&windowClass)) {
		return false;
	}

	RECT requiredRect = { 0, 0, 900, 500 };
	AdjustWindowRect (&requiredRect, WS_OVERLAPPEDWINDOW, false);

	HWND windowHandle = CreateWindowEx (
		WS_EX_WINDOWEDGE | WS_CLIPCHILDREN, windowClass.lpszClassName, L"VisualScriptEngine Embedding Demo", WS_OVERLAPPEDWINDOW,
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
