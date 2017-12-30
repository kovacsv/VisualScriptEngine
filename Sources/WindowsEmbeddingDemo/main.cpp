#include "NodeEditor.hpp"
#include "BitmapContextGdi.hpp"
#include "WindowsAppUtilities.hpp"

#include "InputUINodes.hpp"
#include "ViewerUINodes.hpp"

#include <windows.h>

class MyEventHandlers : public NUIE::EventHandlers
{
public:
	MyEventHandlers () :
		hwnd (NULL)
	{
	
	}

	virtual void RedrawRequested () override
	{
		InvalidateRect (hwnd, NULL, FALSE);
	}

	virtual NUIE::CommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, const NUIE::CommandStructure& commands) override
	{
		return UI::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::CommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::CommandStructure& commands) override
	{
		return UI::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::CommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::OutputSlotPtr& outputSlot, const NUIE::CommandStructure& commands) override
	{
		return UI::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	virtual NUIE::CommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::InputSlotPtr& inputSlot, const NUIE::CommandStructure& commands) override
	{
		return UI::SelectCommandFromContextMenu (hwnd, position, commands);
	}

	void SetWindowHandle (HWND newHwnd)
	{
		hwnd = newHwnd;
	}

private:
	HWND hwnd;
};

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
		bitmapContext (),
		skinParams (),
		eventHandlers (),
		evaluationEnv (nullptr),
		hwnd (NULL)
	{
	
	}

	virtual NUIE::DrawingContext& GetDrawingContext () override
	{
		return bitmapContext;
	}

	virtual NUIE::SkinParams& GetSkinParams () override
	{
		return skinParams;
	}
	
	virtual NUIE::EventHandlers& GetEventHandlers () override
	{
		return eventHandlers;
	}

	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
	}

	void SetWindowHandle (HWND newHwnd)
	{
		eventHandlers.SetWindowHandle (newHwnd);
		hwnd = newHwnd;
	}

	void DrawContextToWindow ()
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint (hwnd, &ps);
		bitmapContext.DrawToHDC (hdc);
		EndPaint (hwnd, &ps);
	}

private:
	BitmapContextGdi	bitmapContext;
	NUIE::SkinParams	skinParams;
	MyEventHandlers		eventHandlers;
	NE::EvaluationEnv	evaluationEnv;
	HWND				hwnd;
};

static MyNodeUIEnvironment uiEnvironment;
static NUIE::NodeEditor nodeEditor (uiEnvironment);

LRESULT CALLBACK ApplicationWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	}

	if (UI::ForwardInputEventToNodeEditor (nodeEditor, hwnd, msg, wParam, lParam)) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	switch (msg) {
		case WM_CREATE:
			{
				uiEnvironment.SetWindowHandle (hwnd);
				NUIE::NodeUIManager& uiManager = nodeEditor.GetNodeUIManager ();
				uiManager.AddNode (NUIE::UINodePtr (new NUIE::IntegerUpDownUINode (L"Integer", NUIE::Point (100, 100), 20, 10)), uiEnvironment.GetEvaluationEnv ());
				uiManager.AddNode (NUIE::UINodePtr (new NUIE::IntegerUpDownUINode (L"Integer", NUIE::Point (100, 300), 20, 10)), uiEnvironment.GetEvaluationEnv ());
				uiManager.AddNode (NUIE::UINodePtr (new NUIE::MultiLineViewerUINode (L"Viewer", NUIE::Point (300, 200), 5)), uiEnvironment.GetEvaluationEnv ());
				nodeEditor.Update ();
			}
			break;
		case WM_PAINT:
			nodeEditor.Draw ();
			uiEnvironment.DrawContextToWindow ();
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
