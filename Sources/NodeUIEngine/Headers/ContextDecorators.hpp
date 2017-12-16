#ifndef CONTEXTDECORATORS_HPP
#define CONTEXTDECORATORS_HPP

#include "Geometry.hpp"
#include "ViewBox.hpp"
#include "DrawingContext.hpp"
#include <string>

namespace NUIE
{

class ViewBoxContextDecorator : public DrawingContextDecorator
{
public:
	ViewBoxContextDecorator (DrawingContext& decorated, const ViewBox& viewBox);

	virtual void	DrawLine (const Point& beg, const Point& end, const Pen& pen) override;
	virtual void	DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;
	virtual void	DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void	FillRect (const Rect& rect, const Color& color) override;
	virtual void	DrawEllipse (const Rect& rect, const Pen& pen) override;
	virtual void	FillEllipse (const Rect& rect, const Color& color) override;
	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color) override;

private:
	const ViewBox& viewBox;
};

class ColorChangerContextDecorator : public DrawingContextDecorator
{
public:
	ColorChangerContextDecorator (DrawingContext& decorated);

	virtual void	DrawLine (const Point& beg, const Point& end, const Pen& pen) override;
	virtual void	DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;
	virtual void	DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void	FillRect (const Rect& rect, const Color& color) override;
	virtual void	DrawEllipse (const Rect& rect, const Pen& pen) override;
	virtual void	FillEllipse (const Rect& rect, const Color& color) override;
	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color) override;

private:
	virtual Color	GetChangedColor (const Color& origColor) = 0;
	Pen				GetChangedPen (const Pen& origPen);
};

class ColorBlenderContextDecorator : public ColorChangerContextDecorator
{
public:
	ColorBlenderContextDecorator (DrawingContext& decorated, const Color& blendColor);

private:
	virtual Color	GetChangedColor (const Color& origColor) override;

	Color blendColor;
};

class TextSkipperContextDecorator : public DrawingContextDecorator
{
public:
	TextSkipperContextDecorator (DrawingContext& decorated, bool isPreviewMode);

	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color) override;

private:
	bool isPreviewMode;
};

}

#endif
