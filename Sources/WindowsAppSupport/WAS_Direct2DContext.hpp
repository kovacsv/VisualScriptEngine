#ifndef WAS_DIRECT2DCONTEXT_HPP
#define WAS_DIRECT2DCONTEXT_HPP

#include <d2d1.h>
#include <dwrite.h>
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

class Direct2DContext : public NUIE::NativeDrawingContext
{
public:
	Direct2DContext (Direct2DImageLoader* imageLoader);
	Direct2DContext (const Direct2DContext& rhs) = delete;
	virtual ~Direct2DContext ();

	virtual void				Init (void* nativeHandle) override;
	virtual void				BlitToWindow (void* nativeHandle) override;
	virtual void				BlitToContext (void* nativeContext) override;

	virtual void				Resize (int newWidth, int newHeight) override;

	virtual double				GetWidth () const override;
	virtual double				GetHeight () const override;

	virtual void				BeginDraw () override;
	virtual void				EndDraw () override;

	virtual bool				NeedToDraw (ItemPreviewMode mode) override;

	virtual void				DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen) override;
	virtual void				DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen) override;

	virtual void				DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void				FillRect (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void				DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void				FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void				DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor) override;
	virtual NUIE::Size			MeasureText (const NUIE::Font& font, const std::wstring& text) override;

	virtual void				DrawIcon (const NUIE::Rect& rect, const NUIE::IconId& iconId) override;

private:
	void						CreateRenderTarget ();

	HWND						hwnd;
	int							width;
	int							height;
	ID2D1HwndRenderTarget*		renderTarget;
	Direct2DImageLoader*		imageLoader;
};

}

#endif
