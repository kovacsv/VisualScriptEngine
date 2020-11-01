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

HBITMAP LoadBitmapFromResource (LPWSTR resourceName, LPWSTR resourceType)
{
	GdiplusInitializer initializer;
	
	HRSRC resourceHandle = FindResource (NULL, resourceName, resourceType);
	if (resourceHandle == NULL) {
		return NULL;
	}

	DWORD resourceSize = SizeofResource (NULL, resourceHandle);
	if (resourceSize == 0) {
		return NULL;
	}

	HGLOBAL resourceData = LoadResource (NULL, resourceHandle);
	if (resourceData == NULL) {
		return NULL;
	}

	const void* resourcePtr = LockResource (resourceData);
	if (resourcePtr == NULL) {
		return NULL;
	}

	HGLOBAL globalHandle = GlobalAlloc (GMEM_MOVEABLE, resourceSize);
	HBITMAP resultBitmap = NULL;
	if (globalHandle != NULL) {
		void* globalPtr = GlobalLock (globalHandle);
		CopyMemory (globalPtr, resourcePtr, resourceSize);

		IStream* stream;
		if (CreateStreamOnHGlobal (globalHandle, FALSE, &stream) == S_OK) {
			Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromStream (stream);
			stream->Release ();
			if (bitmap != nullptr) {
				bitmap->GetHBITMAP (Gdiplus::Color (255, 255, 255), &resultBitmap);
				delete bitmap;
			}
		}
		GlobalUnlock (globalHandle);
		GlobalFree (globalHandle);
	}

	return resultBitmap;
}

}
