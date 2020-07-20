#ifndef WAS_DIRECT2DOFFSCREENCONTEXT_HPP
#define WAS_DIRECT2DOFFSCREENCONTEXT_HPP

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <unordered_map>

#include "NUIE_DrawingContext.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"
#include "WAS_Direct2DImageLoader.hpp"
#include "WAS_Direct2DUtils.hpp"

namespace WAS
{

class Direct2DOffscreenContext : public NUIE::NativeDrawingContext
{
public:
	Direct2DOffscreenContext (const Direct2DImageLoaderPtr& imageLoader);
	Direct2DOffscreenContext (const Direct2DOffscreenContext& rhs) = delete;
	virtual ~Direct2DOffscreenContext ();

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

	virtual bool				CanDrawIcon () override;
	virtual void				DrawIcon (const NUIE::Rect& rect, const NUIE::IconId& iconId) override;

private:
	void						CreateRenderTarget ();
	void						CreateOffscreenContext ();

	int							width;
	int							height;

	Direct2DHandler				direct2DHandler;
	Direct2DImageLoaderPtr		imageLoader;
	ID2D1DCRenderTarget*		renderTarget;

	HDC							memoryDC;
	HBITMAP						memoryBitmap;
};

}

#endif
