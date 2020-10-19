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
	void	AddPopupMenuItem (HMENU popupMenu, UINT id, const std::wstring& name);
	void	AddPopupMenuSeparator (HMENU popupMenu);

	void	EnablePopupMenuItem (UINT id, bool enabled);

private:
	HMENU									menuBar;
	std::unordered_map<UINT_PTR, HMENU>		idToPopupMenu;
};

}

#endif
