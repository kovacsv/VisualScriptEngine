#include "WAS_WindowsAppUtils.hpp"

#include <unordered_map>

namespace WAS
{

NUIE::ModifierKeys GetModiferKeysFromEvent (WPARAM wParam)
{
	std::unordered_set<NUIE::KeyCode> keys;
	if (wParam & MK_CONTROL) {
		keys.insert (NUIE::KeyCode::Control);
	}
	if (wParam & MK_SHIFT) {
		keys.insert (NUIE::KeyCode::Shift);
	}
	return NUIE::ModifierKeys (keys);
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

}
