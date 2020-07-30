#ifndef NUIE_DRAWINGCONTEXT_HPP
#define NUIE_DRAWINGCONTEXT_HPP

#include "NUIE_IconId.hpp"
#include "NUIE_Geometry.hpp"
#include "NUIE_Drawing.hpp"
#include <string>
#include <memory>

namespace NUIE
{

class DrawingContext
{
public:
	enum class ItemPreviewMode
	{
		ShowInPreview,
		HideInPreview
	};

	DrawingContext ();
	virtual ~DrawingContext ();

	virtual void	Resize (int newWidth, int newHeight) = 0;

	virtual int		GetWidth () const = 0;
	virtual int		GetHeight () const = 0;

	virtual void	BeginDraw () = 0;
	virtual void	EndDraw () = 0;

	virtual bool	NeedToDraw (ItemPreviewMode mode) = 0;

	virtual void	DrawLine (const Point& beg, const Point& end, const Pen& pen) = 0;
	virtual void	DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) = 0;
	
	virtual void	DrawRect (const Rect& rect, const Pen& pen) = 0;
	virtual void	FillRect (const Rect& rect, const Color& color) = 0;

	virtual void	DrawEllipse (const Rect& rect, const Pen& pen) = 0;
	virtual void	FillEllipse (const Rect& rect, const Color& color) = 0;

	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor) = 0;
	virtual Size	MeasureText (const Font& font, const std::wstring& text) = 0;

	virtual bool	CanDrawIcon () = 0;
	virtual void	DrawIcon (const Rect& rect, const IconId& iconId) = 0;
};

class DrawingContextDecorator : public DrawingContext
{
public:
	DrawingContextDecorator (DrawingContext& decorated);
	virtual ~DrawingContextDecorator ();

	virtual void	Resize (int newWidth, int newHeight) override;

	virtual int		GetWidth () const override;
	virtual int		GetHeight () const override;

	virtual void	BeginDraw () override;
	virtual void	EndDraw () override;

	virtual bool	NeedToDraw (ItemPreviewMode mode) override;

	virtual void	DrawLine (const Point& beg, const Point& end, const Pen& pen) override;
	virtual void	DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;

	virtual void	DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void	FillRect (const Rect& rect, const Color& color) override;

	virtual void	DrawEllipse (const Rect& rect, const Pen& pen) override;
	virtual void	FillEllipse (const Rect& rect, const Color& color) override;

	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor) override;
	virtual Size	MeasureText (const Font& font, const std::wstring& text) override;

	virtual bool	CanDrawIcon () override;
	virtual void	DrawIcon (const Rect& rect, const IconId& iconId) override;

protected:
	DrawingContext& decorated;
};

class NullDrawingContext : public DrawingContext
{
public:
	virtual void	Resize (int newWidth, int newHeight) override;

	virtual int		GetWidth () const override;
	virtual int		GetHeight () const override;

	virtual void	BeginDraw () override;
	virtual void	EndDraw () override;

	virtual bool	NeedToDraw (ItemPreviewMode mode) override;

	virtual void	DrawLine (const Point& beg, const Point& end, const Pen& pen) override;
	virtual void	DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;

	virtual void	DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void	FillRect (const Rect& rect, const Color& color) override;

	virtual void	DrawEllipse (const Rect& rect, const Pen& pen) override;
	virtual void	FillEllipse (const Rect& rect, const Color& color) override;

	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor) override;
	virtual Size	MeasureText (const Font& font, const std::wstring& text) override;

	virtual bool	CanDrawIcon () override;
	virtual void	DrawIcon (const Rect& rect, const IconId& iconId) override;
};

class NativeDrawingContext : public DrawingContext
{
public:
	virtual void	Init (void* nativeHandle) = 0;
	virtual void	BlitToWindow (void* nativeHandle) = 0;
	virtual void	BlitToContext (void* nativeContext) = 0;
};

using NativeDrawingContextPtr = std::shared_ptr<NativeDrawingContext>;
using NativeDrawingContextConstPtr = std::shared_ptr<const NativeDrawingContext>;

}

#endif
