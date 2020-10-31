#include "WAS_GdiplusUtils.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

GdiplusInitializer::GdiplusInitializer ()
{
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartupOutput output;
	DBGONLY (Gdiplus::Status status =) Gdiplus::GdiplusStartup (&gdiPlusToken, &input, &output);
	DBGASSERT (status == Gdiplus::Ok);
}

GdiplusInitializer::~GdiplusInitializer ()
{
	Gdiplus::GdiplusShutdown (gdiPlusToken);
}

}
