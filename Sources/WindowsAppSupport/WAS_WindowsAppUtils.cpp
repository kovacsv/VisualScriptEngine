#include "WAS_WindowsAppUtils.hpp"

#include <unordered_map>

namespace WAS
{

NE::BasicStringSettings GetStringSettingsFromSystem ()
{
	NE::BasicStringSettings result = NE::GetDefaultStringSettings ();

	wchar_t decSeparator[16];
	int decSeparatorLength = GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, decSeparator, 16);
	if (DBGVERIFY (decSeparatorLength == 2)) {
		result.SetDecimalSeparator (decSeparator[0]);
	}

	wchar_t listSeparator[16];
	int listSeparatorLength = GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_SLIST, listSeparator, 16);
	if (DBGVERIFY (listSeparatorLength == 2)) {
		result.SetListSeparator (listSeparator[0]);
	}

	return result;
}

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

}
