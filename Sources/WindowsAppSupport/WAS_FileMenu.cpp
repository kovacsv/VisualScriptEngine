#include "WAS_FileMenu.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

FileMenu::FileMenu () :
	menuBar (nullptr),
	idToPopupMenu ()
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

void FileMenu::AddPopupMenuItem (HMENU popupMenu, UINT id, const std::wstring& name)
{
	if (DBGERROR (idToPopupMenu.find (id) != idToPopupMenu.end ())) {
		return;
	}
	AppendMenu (popupMenu, MF_STRING, id, name.c_str ());
	idToPopupMenu.insert ({ id, popupMenu });
}

void FileMenu::AddPopupMenuSeparator (HMENU popupMenu)
{
	AppendMenu (popupMenu, MF_SEPARATOR, 0, NULL);
}

void FileMenu::EnablePopupMenuItem (UINT id, bool enabled)
{
	if (DBGERROR (idToPopupMenu.find (id) == idToPopupMenu.end ())) {
		return;
	}
	HMENU popupMenu = idToPopupMenu.at (id);
	EnableMenuItem (popupMenu, id, enabled ? MF_ENABLED : MF_DISABLED);
}

}
