#include "WAS_OffscreenBitmap.hpp"

namespace WAS
{

OffscreenBitmap::OffscreenBitmap () :
	memoryDC (NULL),
	memoryBitmap (NULL)
{

}

OffscreenBitmap::~OffscreenBitmap ()
{
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);
}

void OffscreenBitmap::Init (int width, int height)
{
	HDC hdc = GetDC (NULL);
	Init (hdc, width, height);
}

void OffscreenBitmap::Init (HDC hdc, int width, int height)
{
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);
	memoryDC = CreateCompatibleDC (hdc);
	memoryBitmap = CreateCompatibleBitmap (hdc, width, height);
}

HANDLE OffscreenBitmap::SelectBitmapObject ()
{
	return SelectObject (memoryDC, memoryBitmap);
}

void OffscreenBitmap::SelectOtherObject (HANDLE handle)
{
	SelectObject (memoryDC, handle);
}

HDC OffscreenBitmap::GetContext ()
{
	return memoryDC;
}

HBITMAP OffscreenBitmap::GetBitmap ()
{
	return memoryBitmap;
}

SelectBitmapGuard::SelectBitmapGuard (OffscreenBitmap& bitmap) :
	bitmap (bitmap),
	oldHandle (NULL)
{
	oldHandle = bitmap.SelectBitmapObject ();
}

SelectBitmapGuard::~SelectBitmapGuard ()
{
	bitmap.SelectOtherObject (oldHandle);
}

}
