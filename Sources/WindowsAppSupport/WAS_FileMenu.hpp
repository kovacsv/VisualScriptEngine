#ifndef WAS_FILEMENU_HPP
#define WAS_FILEMENU_HPP

#include "WAS_IncludeWindowsHeaders.hpp"

#include <string>
#include <unordered_map>

namespace WAS
{

class FileMenu
{
public:
	FileMenu ();

	HMENU	GetMenuBar ();

	HMENU	AddPopupMenu (const std::wstring& name);
	void	AddPopupMenuItem (HMENU popupMenu, UINT commandId, const std::wstring& name);
	void	AddPopupMenuSeparator (HMENU popupMenu);

	void	EnableItem (UINT id, bool enabled);

private:
	HMENU								menuBar;
	std::unordered_map<UINT, HMENU>		commandToPopupMenu;
};

}

#endif
