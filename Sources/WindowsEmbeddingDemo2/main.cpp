#include "NUIE_NodeEditor.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_HwndEventHandlers.hpp"
#include "WAS_HwndNodeUIEnvironment.hpp"
#include "WAS_ParameterDialog.hpp"
#include "BI_BuiltInNodes.hpp"

#include <windows.h>
#include <windowsx.h>

class MyEventHandlers : public WAS::HwndEventHandlers
{
public:
	MyEventHandlers () :
		WAS::HwndEventHandlers ()
	{
		size_t inputNodes = nodeTree.AddGroup (L"Input Nodes");
		nodeTree.AddItem (inputNodes, L"Boolean", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::BooleanNode (L"Boolean", position, true));
		});
		nodeTree.AddItem (inputNodes, L"Integer", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerUpDownNode (L"Integer", position, 0, 5));
		});
		nodeTree.AddItem (inputNodes, L"Number", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", position, 0.0, 5.0));
		});
		nodeTree.AddItem (inputNodes, L"Integer Increment", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerIncrementedNode (L"Integer Increment", position));
		});
		nodeTree.AddItem (inputNodes, L"Number Increment", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleIncrementedNode (L"Number Increment", position));
		});
		nodeTree.AddItem (inputNodes, L"Number Distribution", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleDistributedNode (L"Number Distribution", position));
		});
		nodeTree.AddItem (inputNodes, L"List Builder", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::ListBuilderNode (L"List Builder", position));
		});
		size_t arithmeticNodes = nodeTree.AddGroup (L"Arithmetic Nodes");
		nodeTree.AddItem (arithmeticNodes, L"Addition", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::AdditionNode (L"Addition", position));
		});
		nodeTree.AddItem (arithmeticNodes, L"Subtraction", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::SubtractionNode (L"Subtraction", position));
		});
		nodeTree.AddItem (arithmeticNodes, L"Multiplication", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiplicationNode (L"Multiplication", position));
		});
		nodeTree.AddItem (arithmeticNodes, L"Division", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DivisionNode (L"Division", position));
		});
		size_t otherNodes = nodeTree.AddGroup (L"Other Nodes");
		nodeTree.AddItem (otherNodes, L"Viewer", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", position, 5));
		});
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override
	{
		NUIE::MenuCommandStructure finalCommands = commands;
		AddNodeTreeAsCommands (nodeTree, uiManager, uiEnvironment, position, finalCommands);
		return WAS::SelectCommandFromContextMenu (control->GetEditorHandle (), position, finalCommands);
	}

private:
	WAS::NodeTree nodeTree;
};

static std::shared_ptr<WAS::NodeEditorHwndControl> nodeEditorControl (new WAS::NodeEditorHwndControl ());
static WAS::HwndNodeUIEnvironment uiEnvironment (
	nodeEditorControl,
	NE::StringConverterPtr (new NE::BasicStringConverter (NE::GetDefaultStringConverter ())),
	NUIE::SkinParamsPtr (new NUIE::BasicSkinParams (NUIE::GetDefaultSkinParams ())),
	WAS::HwndEventHandlersPtr (new MyEventHandlers ()),
	nullptr
);
static NUIE::NodeEditor nodeEditor (uiEnvironment);

LRESULT CALLBACK ApplicationWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_CREATE:
			{
				uiEnvironment.Init (&nodeEditor, hwnd);
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
