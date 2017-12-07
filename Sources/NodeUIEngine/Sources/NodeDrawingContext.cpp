#include "NodeDrawingContext.hpp"

namespace NUIE
{

NodeDrawingContext::NodeDrawingContext ()
{

}

NodeDrawingContext::~NodeDrawingContext ()
{

}

NodeDrawingContextDecorator::NodeDrawingContextDecorator (NodeDrawingContext& decorated) :
	decorated (decorated)
{

}

NodeDrawingContextDecorator::~NodeDrawingContextDecorator ()
{

}

void NodeDrawingContextDecorator::Resize (int newWidth, int newHeight)
{
	decorated.Resize (newWidth, newHeight);
}

double NodeDrawingContextDecorator::GetWidth () const
{
	return decorated.GetWidth ();
}

double NodeDrawingContextDecorator::GetHeight () const
{
	return decorated.GetHeight ();
}

void NodeDrawingContextDecorator::BeginDraw ()
{
	decorated.BeginDraw ();
}

void NodeDrawingContextDecorator::EndDraw ()
{
	decorated.EndDraw ();
}

void NodeDrawingContextDecorator::DrawLine (const Point& beg, const Point& end, const Pen& pen)
{
	decorated.DrawLine (beg, end, pen);
}

void NodeDrawingContextDecorator::DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen)
{
	decorated.DrawBezier (p1, p2, p3, p4, pen);
}

void NodeDrawingContextDecorator::DrawRect (const Rect& rect, const Pen& pen)
{
	decorated.DrawRect (rect, pen);
}

void NodeDrawingContextDecorator::FillRect (const Rect& rect, const Color& color)
{
	decorated.FillRect (rect, color);
}

void NodeDrawingContextDecorator::DrawEllipse (const Rect& rect, const Pen& pen)
{
	decorated.DrawEllipse (rect, pen);
}

void NodeDrawingContextDecorator::FillEllipse (const Rect& rect, const Color& color)
{
	decorated.FillEllipse (rect, color);
}

void NodeDrawingContextDecorator::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color)
{
	decorated.DrawFormattedText (rect, font, text, hAnchor, vAnchor, color);
}

Size NodeDrawingContextDecorator::MeasureText (const Font& font, const std::wstring& text)
{
	return decorated.MeasureText (font, text);
}

void NodeDrawingContextDecorator::OnNodeDrawingBegin (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::Rect& rect)
{
	decorated.OnNodeDrawingBegin (nodeId, checksum, rect);
}

void NodeDrawingContextDecorator::OnNodeDrawingEnd (const NE::NodeId& nodeId)
{
	decorated.OnNodeDrawingEnd (nodeId);
}

}
