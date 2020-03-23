#ifndef WAS_DIRECT2DIMAGELOADER_HPP
#define WAS_DIRECT2DIMAGELOADER_HPP

#include <d2d1.h>
#include <wincodec.h>
#include <unordered_map>

#include "NUIE_DrawingContext.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"

namespace WAS
{

class Direct2DImageLoader
{
public:
	Direct2DImageLoader ();
	virtual ~Direct2DImageLoader ();

	ID2D1Bitmap*				LoadDirect2DImage (const NUIE::IconId& iconId, ID2D1RenderTarget* renderTarget);
	void						ClearCache ();

	virtual ID2D1Bitmap*		CreateDirect2DImage (const NUIE::IconId& iconId, ID2D1RenderTarget* renderTarget) = 0;

protected:
	IWICImagingFactory*								imagingFactory;
	std::unordered_map<NUIE::IconId, ID2D1Bitmap*>	imageCache;
};

class Direct2DDecoderImageLoader : public Direct2DImageLoader
{
public:
	Direct2DDecoderImageLoader ();
	virtual ~Direct2DDecoderImageLoader ();

	virtual ID2D1Bitmap*		CreateDirect2DImage (const NUIE::IconId& iconId, ID2D1RenderTarget* renderTarget) override;
	virtual IWICBitmapDecoder*	CreateDecoder (const NUIE::IconId& iconId) = 0;
};

class Direct2DImageLoaderFromFile : public Direct2DDecoderImageLoader
{
public:
	Direct2DImageLoaderFromFile ();
	virtual ~Direct2DImageLoaderFromFile ();

	virtual IWICBitmapDecoder*	CreateDecoder (const NUIE::IconId& iconId) override;
	virtual std::wstring		GetFilePath (const NUIE::IconId& iconId) = 0;
};

class Direct2DImageLoaderFromResource : public Direct2DDecoderImageLoader
{
public:
	Direct2DImageLoaderFromResource ();
	virtual ~Direct2DImageLoaderFromResource ();
	
	virtual IWICBitmapDecoder*	CreateDecoder (const NUIE::IconId& iconId) override;
	virtual HRSRC				GetImageResHandle (const NUIE::IconId& iconId) = 0;
};

class Direct2DImageLoaderFromIcon : public Direct2DImageLoader
{
public:
	Direct2DImageLoaderFromIcon ();
	virtual ~Direct2DImageLoaderFromIcon ();

	virtual ID2D1Bitmap*		CreateDirect2DImage (const NUIE::IconId& iconId, ID2D1RenderTarget* renderTarget) override;
	virtual HICON				GetIconHandle (const NUIE::IconId& iconId) = 0;
};

}

#endif
