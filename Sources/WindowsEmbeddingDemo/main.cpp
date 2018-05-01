#include "NUIE_NodeEditor.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_NodeEditorHwndControl.hpp"

#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"

#include <windows.h>
#include <windowsx.h>

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

	virtual NUIE::CommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& uiEnvironment,
		const NUIE::Point& position,
		const NUIE::CommandStructure& commands) override
	{
		return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::CommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& env,
		const NUIE::Point& position,
		const NUIE::UINodePtr& uiNode,
		const NUIE::CommandStructure& commands) override
	{
		return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::CommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& env,
		const NUIE::Point& position,
		const NE::OutputSlotPtr& outputSlot,
		const NUIE::CommandStructure& commands) override
	{
		return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::CommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& env,
		const NUIE::Point& position,
		const NE::InputSlotPtr& inputSlot,
		const NUIE::CommandStructure& commands) override
	{
		return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::CommandPtr OnContextMenu (
		NUIE::NodeUIManager& uiManager,
		NUIE::NodeUIEnvironment& env,
		const NUIE::Point& position,
		const NUIE::UINodeGroupPtr& group,
		const NUIE::CommandStructure& commands) override
	{
		return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual bool OnParameterSettings (NUIE::ParameterInterfacePtr) override
	{
		// TODO
		return false;
	}

private:
	HWND hwnd;
};

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
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

	virtual NUIE::DrawingContext& GetDrawingContext () override
	{
		return nodeEditorControl.GetDrawingContext ();
	}

	virtual NUIE::SkinParams& GetSkinParams () override
	{
		return skinParams;
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

private:
	NUIE::DefaultSkinParams			skinParams;
	MyEventHandlers					eventHandlers;
	NE::EvaluationEnv				evaluationEnv;
	WAS::NodeEditorHwndControl		nodeEditorControl;
};

static MyNodeUIEnvironment uiEnvironment;
static NUIE::NodeEditor nodeEditor (uiEnvironment);

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
