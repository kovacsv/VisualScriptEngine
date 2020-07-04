#include "WAS_NodeEditorHwndControl.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

static LRESULT CALLBACK NodeEditorStaticWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static SetCaptureHandler setCaptureHandler;

	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	} else if (msg == WM_DESTROY) {
		SetWindowLongPtr (hwnd, GWLP_USERDATA, NULL);
	}

	NodeEditorHwndControl* control = (NodeEditorHwndControl*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
	if (control == nullptr) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	NUIE::NodeEditor* nodeEditor = control->GetNodeEditor ();
	if (DBGERROR (nodeEditor == nullptr)) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	switch (msg) {
		case WM_PAINT:
			control->Draw ();
			break;
		case WM_ERASEBKGND:
			return 0;
		case WM_LBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseDown (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseDown (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseDown (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			break;
		case WM_LBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseUp (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseUp (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseUp (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			break;
		case WM_MOUSEMOVE:
			{
				SetFocus (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseMove (GetModiferKeysFromEvent (wParam), x, y);
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
				nodeEditor->OnMouseWheel (GetModiferKeysFromEvent (wParam), rotation, mousePos.x, mousePos.y);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseDoubleClick (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			break;
		case WM_MBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseDoubleClick (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			break;
		case WM_RBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor->OnMouseDoubleClick (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			break;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				nodeEditor->OnResize (newWidth, newHeight);
			}
			break;
		case WM_KEYDOWN:
			{
				NUIE::Key pressedKey (NUIE::KeyCode::Undefined);
				bool isControlPressed = (GetKeyState (VK_CONTROL) < 0);
				if (isControlPressed) {
					switch (wParam) {
						case 'A':
							pressedKey.SetKeyCode (NUIE::KeyCode::SelectAll);
							break;
						case 'C':
							pressedKey.SetKeyCode (NUIE::KeyCode::Copy);
							break;
						case 'V':
							pressedKey.SetKeyCode (NUIE::KeyCode::Paste);
							break;
						case 'G':
							pressedKey.SetKeyCode (NUIE::KeyCode::Group);
							break;
						case 'Z':
							pressedKey.SetKeyCode (NUIE::KeyCode::Undo);
							break;
						case 'Y':
							pressedKey.SetKeyCode (NUIE::KeyCode::Redo);
							break;
					}
				} else {
					switch (wParam) {
						case VK_ESCAPE:
							pressedKey.SetKeyCode (NUIE::KeyCode::Escape);
							break;
						case VK_DELETE:
							pressedKey.SetKeyCode (NUIE::KeyCode::Delete);
							break;
					}
				}
				if (pressedKey.IsValid ()) {
					nodeEditor->OnKeyPress (pressedKey);
				}
			}
			break;
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
}

NodeEditorHwndControl::NodeEditorHwndControl () :
	NodeEditorHwndControl (NUIE::NativeDrawingContextPtr (new BitmapContextGdi ()))
{

}

NodeEditorHwndControl::NodeEditorHwndControl (const NUIE::NativeDrawingContextPtr& nativeContext) :
	NativeNodeEditorControl (),
	nodeEditor (nullptr),
	nativeContext (nativeContext),
	control ()
{

}

NodeEditorHwndControl::~NodeEditorHwndControl ()
{

}

bool NodeEditorHwndControl::Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height)
{
	nodeEditor = nodeEditorPtr;
	DBGASSERT (nodeEditor != nullptr);

	bool controlInited = control.Init ((HWND) nativeParentHandle, NodeEditorStaticWindowProc, L"NodeEditorHwndControl", this);
	if (DBGERROR (!controlInited)) {
		return false;
	}

	HWND hwnd = control.GetWindowHandle ();
	if (DBGERROR (hwnd == NULL)) {
		return false;
	}

	nativeContext->Init (hwnd);
	MoveWindow (hwnd, x, y, width, height, TRUE);

	return true;
}

void* NodeEditorHwndControl::GetEditorNativeHandle () const
{
	return control.GetWindowHandle ();
}

bool NodeEditorHwndControl::IsEditorFocused () const
{
	HWND focusedHwnd = GetFocus ();
	HWND editorHwnd = (HWND) GetEditorNativeHandle ();
	return focusedHwnd == editorHwnd;
}

void NodeEditorHwndControl::Resize (int x, int y, int width, int height)
{
	HWND hwnd = control.GetWindowHandle ();
	if (hwnd == NULL) {
		return;
	}
	MoveWindow (hwnd, x, y, width, height, TRUE);
	if (nodeEditor != nullptr) {
		nodeEditor->OnResize (width, height);
	}
}

void NodeEditorHwndControl::Invalidate ()
{
	HWND hwnd = control.GetWindowHandle ();
	if (hwnd == NULL) {
		return;
	}
	InvalidateRect (hwnd, NULL, FALSE);
}

NUIE::DrawingContext& NodeEditorHwndControl::GetDrawingContext ()
{
	return *nativeContext;
}

NUIE::NodeEditor* NodeEditorHwndControl::GetNodeEditor ()
{
	return nodeEditor;
}

void NodeEditorHwndControl::Draw ()
{
	HWND hwnd = control.GetWindowHandle ();
	if (hwnd == NULL) {
		return;
	}
	if (nodeEditor != nullptr) {
		nodeEditor->Draw ();
	}
	nativeContext->BlitToWindow (hwnd);
}

}
