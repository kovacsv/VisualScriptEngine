#ifndef DRAWINGCONTEXT_HPP
#define DRAWINGCONTEXT_HPP

#include "Geometry.hpp"
#include "Drawing.hpp"
#include <string>

namespace NUIE
{

class DrawingContext
{
public:
	virtual void	Resize (int newWidth, int newHeight) = 0;

	virtual double	GetWidth () const = 0;
	virtual double	GetHeight () const = 0;

	virtual void	BeginDraw () = 0;
	virtual void	EndDraw () = 0;

	virtual void	DrawLine (const Point& beg, const Point& end, const Pen& pen) = 0;
	virtual void	DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) = 0;
	
	virtual void	DrawRect (const Rect& rect, const Pen& pen) = 0;
	virtual void	FillRect (const Rect& rect, const Color& color) = 0;

	virtual void	DrawEllipse (const Rect& rect, const Pen& pen) = 0;
	virtual void	FillEllipse (const Rect& rect, const Color& color) = 0;

	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor) = 0;
	virtual Size	MeasureText (const Font& font, const std::wstring& text) = 0;
};

class DrawingContextDecorator : public DrawingContext
{
public:
	DrawingContextDecorator (DrawingContext& decorated);
	virtual ~DrawingContextDecorator ();

	virtual void	Resize (int newWidth, int newHeight) override;

	virtual double	GetWidth () const override;
	virtual double	GetHeight () const override;

	virtual void	BeginDraw () override;
	virtual void	EndDraw () override;

	virtual void	DrawLine (const Point& beg, const Point& end, const Pen& pen) override;
	virtual void	DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;

	virtual void	DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void	FillRect (const Rect& rect, const Color& color) override;

	virtual void	DrawEllipse (const Rect& rect, const Pen& pen) override;
	virtual void	FillEllipse (const Rect& rect, const Color& color) override;

	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor) override;
	virtual Size	MeasureText (const Font& font, const std::wstring& text) override;

protected:
	DrawingContext& decorated;
};

class NullDrawingContext : public DrawingContext
{
public:
	NullDrawingContext ();
	virtual ~NullDrawingContext ();

	virtual void	Resize (int newWidth, int newHeight) override;

	virtual double	GetWidth () const override;
	virtual double	GetHeight () const override;

	virtual void	BeginDraw () override;
	virtual void	EndDraw () override;

	virtual void	DrawLine (const Point& beg, const Point& end, const Pen& pen) override;
	virtual void	DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;

	virtual void	DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void	FillRect (const Rect& rect, const Color& color) override;

	virtual void	DrawEllipse (const Rect& rect, const Pen& pen) override;
	virtual void	FillEllipse (const Rect& rect, const Color& color) override;

	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor) override;
	virtual Size	MeasureText (const Font& font, const std::wstring& text) override;
};

}

#endif
