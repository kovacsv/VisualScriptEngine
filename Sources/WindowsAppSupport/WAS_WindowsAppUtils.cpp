#include "WAS_WindowsAppUtils.hpp"

#include <unordered_map>

namespace WAS
{

NUIE::ModifierKeys GetModiferKeysFromEvent (WPARAM wParam)
{
	std::unordered_set<NUIE::ModifierKeyCode> keys;
	if (wParam & MK_CONTROL) {
		keys.insert (NUIE::ModifierKeyCode::Control);
	}
	if (wParam & MK_SHIFT) {
		keys.insert (NUIE::ModifierKeyCode::Shift);
	}
	return NUIE::ModifierKeys (keys);
}

NUIE::MenuCommandPtr SelectCommandFromContextMenu (HWND hwnd, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
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

	std::unordered_map<size_t, NUIE::MenuCommandPtr> commandTable;

	size_t currentCommandId = 1000;
	std::function<void (const NUIE::MenuCommandPtr&)> addCommand = [&] (const NUIE::MenuCommandPtr& command) {
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

HWND CreateCustomControl (HWND parentHandle, WNDPROC windowProc, LPCWSTR className, LPVOID lParam)
{
	WNDCLASSEX windowClass;
	memset (&windowClass, 0, sizeof (WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0;
	windowClass.lpfnWndProc = windowProc;
	windowClass.style = CS_DBLCLKS;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = NULL;
	windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = className;

	RegisterClassEx (&windowClass);
	HWND hwnd = CreateWindowEx (
		0, windowClass.lpszClassName, L"", WS_CHILD,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, parentHandle, NULL, NULL, lParam
	);

	return hwnd;
}

}
