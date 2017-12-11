#include "WindowsAppUtilities.hpp"
#include "MouseEventHandler.hpp"

#include <windowsx.h>
#include <unordered_map>

namespace UI
{

SetCaptureHandler::SetCaptureHandler () :
	counter (0)
{
	
}

void SetCaptureHandler::HandleMouseDown (HWND hwnd)
{
	if (counter == 0) {
		SetCapture (hwnd);
	}
	counter += 1;
}

void SetCaptureHandler::HandleMouseUp ()
{
	counter -= 1;
	if (counter == 0) {
		ReleaseCapture ();
	}
}

NUIE::CommandPtr SelectCommandFromContextMenu (HWND hwnd, const NUIE::Point& position, const NUIE::CommandStructure& commands)
{
	if (commands.IsEmpty ()) {
		return nullptr;
	}

	POINT mousePos;
	mousePos.x = (int) position.GetX ();
	mousePos.y = (int) position.GetY ();
	ClientToScreen (hwnd, &mousePos);
	HMENU hPopupMenu = CreatePopupMenu ();
	HMENU hCurrentMenu = hPopupMenu;

	std::unordered_map<size_t, NUIE::CommandPtr> commandTable;

	size_t currentCommandId = 1000;
	std::function<void (const NUIE::CommandPtr&)> addCommand = [&] (const NUIE::CommandPtr& command) {
		if (command->HasChildCommands ()) {
			HMENU subMenu = CreatePopupMenu ();
			AppendMenu (hCurrentMenu, MF_POPUP, (UINT_PTR) subMenu, command->GetName ().c_str ());
			HMENU hOldMenu = hCurrentMenu;
			hCurrentMenu = subMenu;
			command->EnumerateChildCommands (addCommand);
			hCurrentMenu = hOldMenu;
		} else {
			UINT flags = MF_STRING;
			if (command->IsChecked ()) {
				flags |= MF_CHECKED;
			}
			InsertMenu (hCurrentMenu, 0, flags, currentCommandId, command->GetName ().c_str ());
			commandTable.insert ({ currentCommandId, command });
			currentCommandId += 1;
		}
	};
	commands.EnumerateCommands (addCommand);

	int selectedItem = TrackPopupMenu (hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, mousePos.x, mousePos.y, 0, hwnd, NULL);
	DestroyMenu (hPopupMenu);
	if (selectedItem == 0) {
		return nullptr;
	}

	return commandTable[selectedItem];
}

static NUIE::KeySet GetKeysFromEvent (WPARAM wParam)
{
	std::unordered_set<NUIE::KeyCode> keys;
	if (wParam & MK_CONTROL) {
		keys.insert (NUIE::KeyCode::Control);
	}
	if (wParam & MK_SHIFT) {
		keys.insert (NUIE::KeyCode::Shift);
	}
	return NUIE::KeySet (keys);
}

bool ForwardInputEventToNodeEditor (NUIE::NodeEditor& nodeEditor, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static SetCaptureHandler setCaptureHandler;
	
	switch (msg) {
		case WM_LBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDown (GetKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			return true;
		case WM_MBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDown (GetKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			return true;
		case WM_RBUTTONDOWN:
			{
				setCaptureHandler.HandleMouseDown (hwnd);
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDown (GetKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			return true;
		case WM_LBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseUp (GetKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			return true;
		case WM_MBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseUp (GetKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			return true;
		case WM_RBUTTONUP:
			{
				setCaptureHandler.HandleMouseUp ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseUp (GetKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			return true;
		case WM_MOUSEMOVE:
			{
				SetFocus (hwnd); // before Windows 10 only the focused window catches the mouse wheel message
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseMove (GetKeysFromEvent (wParam), x, y);
			}
			return true;
		case WM_MOUSEWHEEL:
			{
				POINT mousePos;
				mousePos.x = GET_X_LPARAM (lParam);
				mousePos.y = GET_Y_LPARAM (lParam);
				ScreenToClient (hwnd, &mousePos);
				int delta = GET_WHEEL_DELTA_WPARAM (wParam);
				NUIE::MouseWheelRotation rotation = delta > 0 ? NUIE::MouseWheelRotation::Forward : NUIE::MouseWheelRotation::Backward;
				nodeEditor.OnMouseWheel (GetKeysFromEvent (wParam), rotation, mousePos.x, mousePos.y);
			}
			return true;
		case WM_LBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDoubleClick (GetKeysFromEvent (wParam), NUIE::MouseButton::Left, x, y);
			}
			return true;
		case WM_MBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDoubleClick (GetKeysFromEvent (wParam), NUIE::MouseButton::Middle, x, y);
			}
			return true;
		case WM_RBUTTONDBLCLK:
			{
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				nodeEditor.OnMouseDoubleClick (GetKeysFromEvent (wParam), NUIE::MouseButton::Right, x, y);
			}
			return true;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				nodeEditor.OnResize (newWidth, newHeight);
			}
			return true;
	}
	return false;
}

}
