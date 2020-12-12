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
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);

	HDC hdc = GetDC (NULL);
	memoryDC = CreateCompatibleDC (hdc);
	memoryBitmap = CreateCompatibleBitmap (hdc, width, height);
}

HDC OffscreenBitmap::GetContext ()
{
	return memoryDC;
}

HBITMAP OffscreenBitmap::GetBitmap ()
{
	return memoryBitmap;
}

}
