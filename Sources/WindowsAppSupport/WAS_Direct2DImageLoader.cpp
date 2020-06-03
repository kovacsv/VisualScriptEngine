#include "WAS_Direct2DImageLoader.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

template<class Interface>
static void SafeRelease (Interface** interfaceToRelease)
{
	if (*interfaceToRelease != NULL) {
		(*interfaceToRelease)->Release ();
		(*interfaceToRelease) = NULL;
	}
}

Direct2DImageLoader::Direct2DImageLoader () :
	imagingFactory (nullptr),
	imageCache ()
{
	CoInitialize (0);
	HRESULT imagingFactoryResult = CoCreateInstance (CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*) &imagingFactory);
	if (!SUCCEEDED (imagingFactoryResult)) {
		DBGBREAK ();
		return;
	}
}

Direct2DImageLoader::~Direct2DImageLoader ()
{
	ClearCache ();
	SafeRelease (&imagingFactory);
	CoUninitialize ();
}

ID2D1Bitmap* Direct2DImageLoader::LoadDirect2DImage (const NUIE::IconId& iconId, ID2D1RenderTarget* renderTarget)
{
	auto found = imageCache.find (iconId);
	if (found != imageCache.end ()) {
		return found->second;
	}
	ID2D1Bitmap* image = CreateDirect2DImage (iconId, renderTarget);
	imageCache.insert ({ iconId, image });
	return image;
}

void Direct2DImageLoader::ClearCache ()
{
	for (auto& it : imageCache) {
		SafeRelease (&it.second);
	}
	imageCache.clear ();
}

Direct2DDecoderImageLoader::Direct2DDecoderImageLoader () :
	Direct2DImageLoader ()
{
}

Direct2DDecoderImageLoader::~Direct2DDecoderImageLoader ()
{
}

ID2D1Bitmap* Direct2DDecoderImageLoader::CreateDirect2DImage (const NUIE::IconId& iconId, ID2D1RenderTarget* renderTarget)
{
	HRESULT hr;

	IWICBitmapDecoder* decoder = CreateDecoder (iconId);
	if (decoder == nullptr) {
		return nullptr;
	}

	IWICBitmapFrameDecode* source = NULL;
	hr = decoder->GetFrame (0, &source);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	IWICFormatConverter* converter = NULL;
	hr = imagingFactory->CreateFormatConverter (&converter);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	hr = converter->Initialize (
		source,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	ID2D1Bitmap* bitmap = NULL;
	hr = renderTarget->CreateBitmapFromWicBitmap (
		converter,
		NULL,
		&bitmap
	);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	SafeRelease (&decoder);
	SafeRelease (&source);
	SafeRelease (&converter);

	return bitmap;
}

Direct2DImageLoaderFromFile::Direct2DImageLoaderFromFile () :
	Direct2DDecoderImageLoader ()
{

}

Direct2DImageLoaderFromFile::~Direct2DImageLoaderFromFile ()
{

}

IWICBitmapDecoder* Direct2DImageLoaderFromFile::CreateDecoder (const NUIE::IconId& iconId)
{
	std::wstring filePath = GetFilePath (iconId);
	IWICBitmapDecoder* decoder = NULL;
	HRESULT hr = imagingFactory->CreateDecoderFromFilename (
		filePath.c_str (),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder
	);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}
	return decoder;
}

Direct2DImageLoaderFromResource::Direct2DImageLoaderFromResource () :
	Direct2DDecoderImageLoader ()
{

}

Direct2DImageLoaderFromResource::~Direct2DImageLoaderFromResource ()
{

}

IWICBitmapDecoder* Direct2DImageLoaderFromResource::CreateDecoder (const NUIE::IconId& iconId)
{
	HRESULT hr;

	HRSRC imageResHandle = GetImageResHandle (iconId);
	if (imageResHandle == NULL) {
		return nullptr;
	}
	
	HGLOBAL imageResDataHandle = LoadResource (NULL, imageResHandle);
	if (imageResDataHandle == NULL) {
		return nullptr;
	}

	void* imageFile = LockResource (imageResDataHandle);
	if (imageFile == NULL) {
		return nullptr;
	}

	IWICStream* stream = NULL;
	hr = imagingFactory->CreateStream (&stream);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	DWORD imageFileSize = SizeofResource (NULL, imageResHandle);
	hr = stream->InitializeFromMemory (reinterpret_cast<BYTE*> (imageFile), imageFileSize);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	IWICBitmapDecoder* decoder = NULL;
	hr = imagingFactory->CreateDecoderFromStream (
		stream,
		NULL,
		WICDecodeMetadataCacheOnLoad,
		&decoder
	);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	SafeRelease (&stream);
	return decoder;
}

Direct2DImageLoaderFromIcon::Direct2DImageLoaderFromIcon ()
{
}

Direct2DImageLoaderFromIcon::~Direct2DImageLoaderFromIcon ()
{
}

ID2D1Bitmap* Direct2DImageLoaderFromIcon::CreateDirect2DImage (const NUIE::IconId& iconId, ID2D1RenderTarget* renderTarget)
{
	HRESULT hr;

	HICON hIcon = GetIconHandle (iconId);
	if (hIcon == NULL) {
		return nullptr;
	}

	IWICBitmap* wicBitmap = NULL;
	hr = imagingFactory->CreateBitmapFromHICON (
		hIcon,
		&wicBitmap
	);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	IWICFormatConverter* converter = NULL;
	hr = imagingFactory->CreateFormatConverter (&converter);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	hr = converter->Initialize (
		wicBitmap,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	ID2D1Bitmap* bitmap = NULL;
	hr = renderTarget->CreateBitmapFromWicBitmap (
		converter,
		NULL,
		&bitmap
	);
	if (!SUCCEEDED (hr)) {
		return nullptr;
	}

	SafeRelease (&converter);
	SafeRelease (&wicBitmap);

	return bitmap;
}

}
