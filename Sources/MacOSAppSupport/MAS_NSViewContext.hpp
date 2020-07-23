#ifndef MAS_NSVIEWCONTEXT_HPP
#define MAS_NSVIEWCONTEXT_HPP

#include "NUIE_DrawingContext.hpp"
#include "NUIE_Drawing.hpp"
#include "NUIE_DrawingCacheKeys.hpp"
#include "MAS_NSImageLoader.hpp"

#include <unordered_map>

#ifdef __cplusplus
#ifdef __OBJC__
	@class NSView;
	@class NSFont;
#else
	struct NSView;
	struct NSFont;
#endif
#endif

namespace MAS
{

class NSViewContext : public NUIE::NativeDrawingContext
{
public:
	NSViewContext ();
	NSViewContext (const NSImageLoaderPtr& imageLoader);
	NSViewContext (const NSViewContext& rhs) = delete;
	virtual ~NSViewContext ();

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
	NSFont*						GetFont (const NUIE::Font& font);
	
	int							width;
	int							height;
	NSView*						nsView;
	NSImageLoaderPtr			imageLoader;
	
	std::unordered_map<NUIE::FontCacheKey, NSFont*>		fontCache;
};

}

#endif
