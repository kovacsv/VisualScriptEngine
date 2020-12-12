#ifndef WAS_OFFSCREENBITMAP_HPP
#define WAS_OFFSCREENBITMAP_HPP

#include "WAS_IncludeWindowsHeaders.hpp"

namespace WAS
{

class OffscreenBitmap
{
public:
	OffscreenBitmap ();
	~OffscreenBitmap ();

	void		Init (int width, int height);
	void		Init (HDC hdc, int width, int height);
	HANDLE		SelectBitmapObject ();
	void		SelectOtherObject (HANDLE handle);

	HDC			GetContext ();
	HBITMAP		GetBitmap ();

private:
	HDC			memoryDC;
	HBITMAP		memoryBitmap;
};

class SelectBitmapGuard
{
public:
	SelectBitmapGuard (OffscreenBitmap& bitmap);
	~SelectBitmapGuard ();

private:
	OffscreenBitmap&	bitmap;
	HANDLE				oldHandle;
};

}

#endif
