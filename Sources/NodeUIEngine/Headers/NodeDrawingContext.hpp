#ifndef NODEDRAWINGCONTEXT_HPP
#define NODEDRAWINGCONTEXT_HPP

#include "DrawingContext.hpp"
#include "NodeId.hpp"
#include "Checksum.hpp"

namespace NUIE
{

class NodeDrawingContext : public DrawingContext
{
public:
	NodeDrawingContext ();
	virtual ~NodeDrawingContext ();

	virtual void	OnNodeDrawingBegin (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::Rect& rect) = 0;
	virtual void	OnNodeDrawingEnd (const NE::NodeId& nodeId) = 0;
};

class NodeDrawingContextDecorator : public NodeDrawingContext
{
public:
	NodeDrawingContextDecorator (NodeDrawingContext& decorated);
	virtual ~NodeDrawingContextDecorator ();

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

	virtual void	DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color) override;
	virtual Size	MeasureText (const Font& font, const std::wstring& text) override;

	virtual void	OnNodeDrawingBegin (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::Rect& rect) override;
	virtual void	OnNodeDrawingEnd (const NE::NodeId& nodeId) override;

protected:
	NodeDrawingContext& decorated;
};

}

#endif
