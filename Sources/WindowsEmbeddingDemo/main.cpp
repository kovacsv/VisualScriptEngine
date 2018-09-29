#include "NUIE_NodeEditor.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_NodeEditorHwndControl.hpp"
#include "WAS_ParameterDialog.hpp"

#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "BI_ArithmeticUINodes.hpp"
#include "BI_BuiltInCommands.hpp"

#include <windows.h>
#include <windowsx.h>

class MyCreateNodeCommand : public BI::CreateNodeCommand
{
public:
	enum class NodeType
	{
		Integer,
		Number,
		IntegerRange,
		NumberRange,
		Addition,
		Subtraction,
		Multiplication,
		Division,
		Viewer
	};

	MyCreateNodeCommand (NodeType nodeType, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position) :
		BI::CreateNodeCommand (name, uiManager, uiEnvironment, position),
		nodeType (nodeType)
	{
	}

	virtual NUIE::UINodePtr CreateNode (const NUIE::Point& modelPosition) override
	{
		switch (nodeType) {
			case NodeType::Integer:
				return NUIE::UINodePtr (new BI::IntegerUpDownNode (L"Integer", modelPosition, 0, 5));
			case NodeType::Number:
				return NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", modelPosition, 0.0, 5.0));
			case NodeType::IntegerRange:
				return NUIE::UINodePtr (new BI::IntegerRangeNode (L"Integer Range", modelPosition));
			case NodeType::NumberRange:
				return NUIE::UINodePtr (new BI::DoubleRangeNode (L"Number Range", modelPosition));
			case NodeType::Addition:
				return NUIE::UINodePtr (new BI::AdditionNode (L"Addition", modelPosition));
			case NodeType::Subtraction:
				return NUIE::UINodePtr (new BI::SubtractionNode (L"Subtraction", modelPosition));
			case NodeType::Multiplication:
				return NUIE::UINodePtr (new BI::MultiplicationNode (L"Multiplication", modelPosition));
			case NodeType::Division:
				return NUIE::UINodePtr (new BI::DivisionNode (L"Division", modelPosition));
			case NodeType::Viewer:
				return NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", modelPosition, 5));
		}
		return nullptr;
	}

private:
	NodeType nodeType;
};

class MyEventHandlers : public NUIE::EventHandlers
{
public:
	MyEventHandlers () :
		NUIE::EventHandlers (),
		hwnd (NULL)
	{
	
	}

	void Init (HWND newHwnd)
	{
		hwnd = newHwnd;
	}

	virtual NUIE::UICommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& uiEnvironment,
		const NUIE::Point& position,
		const NUIE::UICommandStructure& commands) override
	{
		NUIE::UICommandStructure finalCommands = commands;
		finalCommands.AddCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Integer, uiManager, uiEnvironment, L"Integer", position)));
		finalCommands.AddCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Number, uiManager, uiEnvironment, L"Number", position)));
		finalCommands.AddCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::IntegerRange, uiManager, uiEnvironment, L"Integer Range", position)));
		finalCommands.AddCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::NumberRange, uiManager, uiEnvironment, L"Number Range", position)));
		finalCommands.AddCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Addition, uiManager, uiEnvironment, L"Addition", position)));
		finalCommands.AddCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Subtraction, uiManager, uiEnvironment, L"Subtraction", position)));
		finalCommands.AddCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Multiplication, uiManager, uiEnvironment, L"Multiplication", position)));
		finalCommands.AddCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Division, uiManager, uiEnvironment, L"Division", position)));
		finalCommands.AddCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Viewer, uiManager, uiEnvironment, L"Viewer", position)));
		return WAS::SelectCommandFromContextMenu (hwnd, position, finalCommands);
	}

	virtual NUIE::UICommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& env,
		const NUIE::Point& position,
		const NUIE::UINodePtr& uiNode,
		const NUIE::UICommandStructure& commands) override
	{
		return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::UICommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& env,
		const NUIE::Point& position,
		const NUIE::UIOutputSlotPtr& outputSlot,
		const NUIE::UICommandStructure& commands) override
	{
		return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::UICommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& env,
		const NUIE::Point& position,
		const NUIE::UIInputSlotPtr& inputSlot,
		const NUIE::UICommandStructure& commands) override
	{
		return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::UICommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& env,
		const NUIE::Point& position,
		const NUIE::UINodeGroupPtr& group,
		const NUIE::UICommandStructure& commands) override
	{
		return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual bool OnParameterSettings (NUIE::ParameterInterfacePtr paramInterface) override
	{
		WAS::ParameterDialog paramDialog (paramInterface);
		return paramDialog.Show (hwnd, 50, 50);
	}

private:
	HWND hwnd;
};

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
		stringSettings (L'.', L',', 2),
		skinParams (),
		eventHandlers (),
		evaluationEnv (nullptr),
		nodeEditorControl ()
	{
	
	}

	void Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle, int x, int y, int width, int height)
	{
		nodeEditorControl.Init (nodeEditorPtr, parentHandle, x, y, width, height);
		eventHandlers.Init (nodeEditorControl.GetWindowHandle ());
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
	
	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
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
	NE::BasicStringSettings		stringSettings;
	NUIE::DefaultSkinParams		skinParams;
	MyEventHandlers				eventHandlers;
	NE::EvaluationEnv			evaluationEnv;
	WAS::NodeEditorHwndControl	nodeEditorControl;
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
				RECT clientRect;
				GetClientRect (hwnd, &clientRect);
				int width = clientRect.right - clientRect.left;
				int height = clientRect.bottom - clientRect.top;
				uiEnvironment.Init (&nodeEditor, hwnd, 0, 0, width, height);

				NUIE::NodeUIManager& uiManager = nodeEditor.GetNodeUIManager ();
				uiManager.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", NUIE::Point (100, 100), 20, 10)), uiEnvironment.GetEvaluationEnv ());
				uiManager.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", NUIE::Point (100, 300), 20, 10)), uiEnvironment.GetEvaluationEnv ());
				uiManager.AddNode (NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", NUIE::Point (300, 200), 5)), uiEnvironment.GetEvaluationEnv ());
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
						nodeEditor.Clear ();
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

int wWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX windowClass;
	memset (&windowClass, 0, sizeof (WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0;
	windowClass.lpfnWndProc = ApplicationWindowProc;
	windowClass.style = CS_DBLCLKS;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = NULL;
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
