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

	HDC			GetContext ();
	HBITMAP		GetBitmap ();

private:
	HDC			memoryDC;
	HBITMAP		memoryBitmap;
};

}

#endif
