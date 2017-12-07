#ifndef NODEBITMAPCONTEXTGDI_HPP
#define NODEBITMAPCONTEXTGDI_HPP

#include "BitmapContextGdi.hpp"
#include "NodeImageCache.hpp"
#include <memory>

class BitmapImageCacheData : public NUIE::NodeImageCachedContext
{
public:
	BitmapImageCacheData (const NE::Checksum& checksum, const NUIE::IntRect& rect) :
		NodeImageCachedContext (checksum, rect),
		context (rect.GetWidth (), rect.GetHeight ())
	{

	}

	virtual NUIE::DrawingContext& GetContext () override
	{
		return context;
	}

	virtual const NUIE::DrawingContext& GetContext () const override
	{
		return context;
	}

	BitmapContextGdi* GetTypedContext ()
	{
		return &context;
	}

private:
	BitmapContextGdi context;
};

class NodeBitmapContextGdi : public NUIE::NodeDrawingContext
{
public:
	NodeBitmapContextGdi (int width, int height);
	virtual ~NodeBitmapContextGdi ();

	virtual void		Resize (int newWidth, int newHeight) override;

	virtual double		GetWidth () const override;
	virtual double		GetHeight () const override;

	virtual void		BeginDraw () override;
	virtual void		EndDraw () override;

	virtual void		DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen) override;
	virtual void		DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen) override;

	virtual void		DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void		FillRect (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void		DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void		FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void		DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& color) override;
	virtual NUIE::Size	MeasureText (const NUIE::Font& font, const std::wstring& text) override;

	virtual void		OnNodeDrawingBegin (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::Rect& rect) override;
	virtual void		OnNodeDrawingEnd (const NE::NodeId& nodeId) override;

	void				DrawToHDC (HDC hdc, int x, int y);

private:
	NUIE::DrawingContext*						ChooseContext ();

	BitmapContextGdi							bitmapContext;
	BitmapImageCacheData*						currentNodeCacheData;
	NUIE::NodeImageCache<BitmapImageCacheData>	nodeImageCache;
};

#endif
