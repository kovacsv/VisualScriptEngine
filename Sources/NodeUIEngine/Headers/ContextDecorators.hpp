#ifndef CONTEXTDECORATORS_HPP
#define CONTEXTDECORATORS_HPP

#include "Geometry.hpp"
#include "ViewBox.hpp"
#include "NodeDrawingContext.hpp"
#include <string>

namespace NUIE
{

class ViewBoxNodeContextDecorator : public NodeDrawingContextDecorator
{
public:
	ViewBoxNodeContextDecorator (NodeDrawingContext& decorated, const ViewBox& viewBox);

	virtual void	DrawLine (const Point& beg, const Point& end, const Pen& pen) override;
	virtual void	DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;
	virtual void	DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void	FillRect (const Rect& rect, const Color& color) override;
	virtual void	DrawEllipse (const Rect& rect, const Pen& pen) override;
	virtual void	FillEllipse (const Rect& rect, const Color& color) override;
	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color) override;
	virtual void	OnNodeDrawingBegin (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::Rect& rect) override;

private:
	const ViewBox& viewBox;
};

class ColorChangerNodeContextDecorator : public NodeDrawingContextDecorator
{
public:
	ColorChangerNodeContextDecorator (NodeDrawingContext& decorated);

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

class ColorBlenderNodeContextDecorator : public ColorChangerNodeContextDecorator
{
public:
	ColorBlenderNodeContextDecorator (NodeDrawingContext& decorated, const Color& blendColor);

	virtual void	OnNodeDrawingBegin (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::Rect& rect) override;

private:
	virtual Color	GetChangedColor (const Color& origColor) override;

	Color blendColor;
};

class TextSkipperNodeContextDecorator : public NodeDrawingContextDecorator
{
public:
	TextSkipperNodeContextDecorator (NodeDrawingContext& decorated, bool isPreviewMode);

	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color) override;

private:
	bool isPreviewMode;
};

}

#endif
