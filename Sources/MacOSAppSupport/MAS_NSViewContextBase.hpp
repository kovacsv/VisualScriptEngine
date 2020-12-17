#ifndef MAS_NSVIEWCONTEXTBASE_HPP
#define MAS_NSVIEWCONTEXTBASE_HPP

#include "NUIE_DrawingContext.hpp"
#include "NUIE_Drawing.hpp"
#include "NUIE_DrawingCacheKeys.hpp"
#include "MAS_NSImageLoader.hpp"

#include <unordered_map>

#ifdef __cplusplus
#ifdef __OBJC__
	@class NSView;
	@class NSFont;
	@class NSImage;
#else
	struct NSView;
	struct NSFont;
	struct NSImage;
#endif
#endif

namespace MAS
{

class NSViewContextBase : public NUIE::NativeDrawingContext
{
public:
	NSViewContextBase ();
	NSViewContextBase (const NSImageLoaderPtr& imageLoader);
	NSViewContextBase (const NSViewContextBase& rhs) = delete;
	virtual ~NSViewContextBase ();

	virtual int					GetWidth () const override;
	virtual int					GetHeight () const override;

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

protected:
	NSFont*						GetFont (const NUIE::Font& font);
	
	int							width;
	int							height;
	NSImageLoaderPtr			imageLoader;
	
	std::unordered_map<NUIE::FontCacheKey, NSFont*>		fontCache;
};

}

#endif
