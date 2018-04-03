#ifndef BITMAPCONTEXTGDIPLUS_HPP
#define BITMAPCONTEXTGDIPLUS_HPP

#include "DrawingContext.hpp"

#include <windows.h>
#include <gdiplus.h>
#include <memory>

class BitmapContextGdiplus : public NUIE::NativeDrawingContext
{
public:
	BitmapContextGdiplus ();
	virtual ~BitmapContextGdiplus ();

	virtual void		Init (void* nativeHandle) override;
	virtual void		BlitToWindow (void* nativeHandle) override;

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

	virtual void		DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor) override;
	virtual NUIE::Size	MeasureText (const NUIE::Font& font, const std::wstring& text) override;

private:
	Gdiplus::Point		CreatePoint (const NUIE::Point& point) const;
	Gdiplus::Rect		CreateRect (const NUIE::Rect& rect) const;
	Gdiplus::RectF		CreateRectF (const NUIE::Rect& rect) const;
	void				InitGraphics ();

	int									width;
	int									height;
	std::unique_ptr<Gdiplus::Bitmap>	bitmap;
	std::unique_ptr<Gdiplus::Graphics>	graphics;
};

#endif
