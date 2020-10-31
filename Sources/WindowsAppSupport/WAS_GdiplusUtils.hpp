#ifndef WAS_GDIPLUSUTILS_HPP
#define WAS_GDIPLUSUTILS_HPP

#include "WAS_IncludeWindowsHeaders.hpp"

namespace WAS
{

class GdiplusInitializer
{
public:
	GdiplusInitializer ();
	~GdiplusInitializer ();

private:
	ULONG_PTR	gdiPlusToken;
};

}

#endif
