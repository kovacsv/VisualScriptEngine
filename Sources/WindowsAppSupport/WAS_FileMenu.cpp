#include "WAS_FileMenu.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

FileMenu::FileMenu () :
	menuBar (NULL),
	commandToPopupMenu ()
{
	menuBar = CreateMenu ();
}

HMENU FileMenu::GetMenuBar ()
{
	return menuBar;
}

HMENU FileMenu::AddPopupMenu (const std::wstring& name)
{
	HMENU menu = CreateMenu ();
	AppendMenu (menuBar, MF_POPUP, (UINT_PTR) menu, name.c_str ());
	return menu;
}

void FileMenu::AddPopupMenuItem (HMENU popupMenu, UINT commandId, const std::wstring& name)
{
	if (DBGERROR (commandToPopupMenu.find (commandId) != commandToPopupMenu.end ())) {
		return;
	}
	AppendMenu (popupMenu, MF_STRING, commandId, name.c_str ());
	commandToPopupMenu.insert ({ commandId, popupMenu });
}

void FileMenu::AddPopupMenuSeparator (HMENU popupMenu)
{
	AppendMenu (popupMenu, MF_SEPARATOR, 0, NULL);
}

void FileMenu::EnablePopupMenuItem (UINT id, bool enabled)
{
	if (DBGERROR (commandToPopupMenu.find (id) == commandToPopupMenu.end ())) {
		return;
	}
	HMENU popupMenu = commandToPopupMenu.at (id);
	EnableMenuItem (popupMenu, id, enabled ? MF_ENABLED : MF_DISABLED);
}

}
