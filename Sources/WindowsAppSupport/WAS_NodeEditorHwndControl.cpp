#include "WAS_NodeEditorHwndControl.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

static LRESULT CALLBACK NodeEditorStaticWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	} else if (msg == WM_DESTROY) {
		SetWindowLongPtr (hwnd, GWLP_USERDATA, 0);
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
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				nodeEditor->OnResize (newWidth, newHeight);
			}
			break;
	}

	if (control->IsInputHandlingEnabled ()) {
		switch (msg) {
			case WM_LBUTTONDOWN:
				{
					SetWindowCapture (hwnd);
					int x = GET_X_LPARAM (lParam);
					int y = GET_Y_LPARAM (lParam);
					nodeEditor->OnMouseDown (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
				}
				break;
			case WM_MBUTTONDOWN:
				{
					SetWindowCapture (hwnd);
					int x = GET_X_LPARAM (lParam);
					int y = GET_Y_LPARAM (lParam);
					nodeEditor->OnMouseDown (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
				}
				break;
			case WM_RBUTTONDOWN:
				{
					SetWindowCapture (hwnd);
					int x = GET_X_LPARAM (lParam);
					int y = GET_Y_LPARAM (lParam);
					nodeEditor->OnMouseDown (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
				}
				break;
			case WM_LBUTTONUP:
				{
					ReleaseWindowCapture (hwnd);
					int x = GET_X_LPARAM (lParam);
					int y = GET_Y_LPARAM (lParam);
					nodeEditor->OnMouseUp (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
				}
				break;
			case WM_MBUTTONUP:
				{
					ReleaseWindowCapture (hwnd);
					int x = GET_X_LPARAM (lParam);
					int y = GET_Y_LPARAM (lParam);
					nodeEditor->OnMouseUp (GetModiferKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
				}
				break;
			case WM_RBUTTONUP:
				{
					ReleaseWindowCapture (hwnd);
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
			case WM_KEYDOWN:
				{
					if (!control->IsMouseOverEditorWindow ()) {
						break;
					}
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
						nodeEditor->ExecuteCommand (commandCode);
					}
				}
				break;
			case WM_CANCELMODE:
				ReleaseWindowCapture (hwnd);
				break;
		}
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
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

bool NodeEditorHwndControl::IsMouseOverEditorWindow () const
{
	POINT mousePos;
	GetCursorPos (&mousePos);

	RECT rect;
	HWND editorHwnd = (HWND) GetEditorNativeHandle ();
	if (!GetWindowRect (editorHwnd, &rect)) {
		return false;
	}

	return PtInRect (&rect, mousePos);
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

NUIE::DrawingContext& NodeEditorHwndControl::GetDrawingContext ()
{
	return *nativeContext;
}

NUIE::NodeEditor* NodeEditorHwndControl::GetNodeEditor ()
{
	return nodeEditor;
}

}
