#include "NUIE_NodeEditor.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"

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

	virtual bool OnParameterSettings (NUIE::NodeParameterAccessorPtr) override
	{
		// TODO
		return false;
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
	
	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
	}

	virtual void OnValuesRecalculated () override
	{
		
	}

	virtual void OnRedrawRequested () override
	{
		InvalidateRect (hwnd, NULL, FALSE);
	}

	virtual NUIE::EventHandlers& GetEventHandlers () override
	{
		return eventHandlers;
	}

	void SetWindowHandle (HWND newHwnd)
	{
		hwnd = newHwnd;
		eventHandlers.SetWindowHandle (hwnd);
		bitmapContext.Init (hwnd);
	}

	void DrawContextToWindow ()
	{
		bitmapContext.BlitToWindow (hwnd);
	}

private:
	WAS::BitmapContextGdi		bitmapContext;
	NUIE::DefaultSkinParams		skinParams;
	MyEventHandlers				eventHandlers;
	NE::EvaluationEnv			evaluationEnv;
	HWND						hwnd;
};

class SetCaptureHandler
{
public:
	SetCaptureHandler () :
		counter (0)
	{
	
	}

	void HandleMouseDown (HWND hwnd)
	{
		if (counter == 0) {
			SetCapture (hwnd);
		}
		counter += 1;
	}

	void HandleMouseUp ()
	{
		counter -= 1;
		if (counter <= 0) { // sometimes down and up are not in pair
			ReleaseCapture ();
			counter = 0;
		}
	}

private:
	int counter;
};

static MyNodeUIEnvironment uiEnvironment;
static NUIE::NodeEditor nodeEditor (uiEnvironment);

LRESULT CALLBACK ApplicationWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static SetCaptureHandler setCaptureHandler;

	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	}

	switch (msg) {
		case WM_CREATE:
			{
				uiEnvironment.SetWindowHandle (hwnd);
				NUIE::NodeUIManager& uiManager = nodeEditor.GetNodeUIManager ();
				uiManager.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", NUIE::Point (100, 100), 20, 10)), uiEnvironment.GetEvaluationEnv ());
				uiManager.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", NUIE::Point (100, 300), 20, 10)), uiEnvironment.GetEvaluationEnv ());
				uiManager.AddNode (NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", NUIE::Point (300, 200), 5)), uiEnvironment.GetEvaluationEnv ());
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
		case WM_LBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDown (WAS::GetKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDown (WAS::GetKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDown (WAS::GetKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			break;
		case WM_LBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseUp (WAS::GetKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseUp (WAS::GetKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseUp (WAS::GetKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			break;
		case WM_MOUSEMOVE:
			{
				SetFocus (hwnd); // before Windows 10 only the focused window catches the mouse wheel message
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseMove (WAS::GetKeysFromEvent (wParam), x, y);
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
				nodeEditor.OnMouseWheel (WAS::GetKeysFromEvent (wParam), rotation, mousePos.x, mousePos.y);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDoubleClick (WAS::GetKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDoubleClick (WAS::GetKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDoubleClick (WAS::GetKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			break;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				nodeEditor.OnResize (newWidth, newHeight);
			}
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
