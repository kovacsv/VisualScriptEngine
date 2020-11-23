#ifndef WAS_TOOLBAR_HPP
#define WAS_TOOLBAR_HPP

#include "WAS_IncludeWindowsHeaders.hpp"

#include <string>
#include <unordered_map>

namespace WAS
{

class Toolbar
{
public:
	Toolbar ();
	~Toolbar ();

	void		Init (HWND parentHandle);
	void		AddItem (HBITMAP bitmap, HBITMAP disabledBitmap, UINT id);
	void		AddSeparator ();
	void		EnableItem (UINT id, bool enabled);
	int			GetHeight ();
	void		AutoSize ();

private:
	HWND		toolbarHandle;
	HIMAGELIST	imageList;
	HIMAGELIST	disabledImageList;
};

}

#endif
