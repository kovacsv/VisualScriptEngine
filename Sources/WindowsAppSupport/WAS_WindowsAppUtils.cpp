#include "WAS_WindowsAppUtils.hpp"

#include <unordered_map>

namespace WAS
{

void SetWindowCapture (HWND hwnd)
{
	if (GetCapture () != hwnd) {
		SetCapture (hwnd);
	}
}

void ReleaseWindowCapture (HWND hwnd)
{
	if (GetCapture () == hwnd) {
		ReleaseCapture ();
	}
}

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
	NUIE::ModifierKeys keys;
	if (wParam & MK_CONTROL) {
		keys.Insert (NUIE::ModifierKeyCode::Control);
	}
	if (wParam & MK_SHIFT) {
		keys.Insert (NUIE::ModifierKeyCode::Shift);
	}
	return keys;
}

static void AddCommandToMenu (const NUIE::MenuCommandPtr& command, std::unordered_map<size_t, NUIE::MenuCommandPtr>& commandTable, HMENU& hCurrentMenu, size_t& currentCommandId)
{
	if (command->HasChildCommands ()) {
		HMENU subMenu = CreatePopupMenu ();
		AppendMenu (hCurrentMenu, MF_POPUP, (UINT_PTR) subMenu, command->GetName ().c_str ());
		HMENU hOldMenu = hCurrentMenu;
		hCurrentMenu = subMenu;
		command->EnumerateChildCommands ([&] (const NUIE::MenuCommandPtr& childCommand) {
			AddCommandToMenu (childCommand, commandTable, hCurrentMenu, currentCommandId);
		});
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

	std::unordered_map<size_t, NUIE::MenuCommandPtr> commandTable;
	HMENU hCurrentMenu = hPopupMenu;
	size_t currentCommandId = 1000;
	commands.EnumerateCommands ([&] (const NUIE::MenuCommandPtr& command) {
		AddCommandToMenu (command, commandTable, hCurrentMenu, currentCommandId);
	});

	int selectedItem = TrackPopupMenu (hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, mousePos.x, mousePos.y, 0, hwnd, NULL);
	DestroyMenu (hPopupMenu);
	if (selectedItem == 0) {
		return nullptr;
	}

	return commandTable[selectedItem];
}

}
