#include "ContextDecorators.hpp"

namespace NUIE
{

ViewBoxNodeContextDecorator::ViewBoxNodeContextDecorator (NodeDrawingContext& decorated, const ViewBox& viewBox) :
	NodeDrawingContextDecorator (decorated),
	viewBox (viewBox)
{

}

void ViewBoxNodeContextDecorator::DrawLine (const Point& beg, const Point& end, const Pen& pen)
{
	decorated.DrawLine (viewBox.ModelToView (beg), viewBox.ModelToView (end), viewBox.ModelToView (pen));
}

void ViewBoxNodeContextDecorator::DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen)
{
	decorated.DrawBezier (viewBox.ModelToView (p1), viewBox.ModelToView (p2), viewBox.ModelToView (p3), viewBox.ModelToView (p4), viewBox.ModelToView (pen));
}

void ViewBoxNodeContextDecorator::DrawRect (const Rect& rect, const Pen& pen)
{
	decorated.DrawRect (viewBox.ModelToView (rect), viewBox.ModelToView (pen));
}

void ViewBoxNodeContextDecorator::FillRect (const Rect& rect, const Color& color)
{
	decorated.FillRect (viewBox.ModelToView (rect), color);
}

void ViewBoxNodeContextDecorator::DrawEllipse (const Rect& rect, const Pen& pen)
{
	decorated.DrawEllipse (viewBox.ModelToView (rect), viewBox.ModelToView (pen));
}

void ViewBoxNodeContextDecorator::FillEllipse (const Rect& rect, const Color& color)
{
	decorated.FillEllipse (viewBox.ModelToView (rect), color);
}

void ViewBoxNodeContextDecorator::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color)
{
	decorated.DrawFormattedText (viewBox.ModelToView (rect), viewBox.ModelToView (font), text, hAnchor, vAnchor, color);
}

void ViewBoxNodeContextDecorator::OnNodeDrawingBegin (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::Rect& rect)
{
	NE::Checksum myChecksum = checksum;
	myChecksum.Add (viewBox.GetScale ());
	decorated.OnNodeDrawingBegin (nodeId, myChecksum, viewBox.ModelToView (rect));
}

ColorChangerNodeContextDecorator::ColorChangerNodeContextDecorator (NodeDrawingContext& decorated) :
	NodeDrawingContextDecorator (decorated)
{

}

void ColorChangerNodeContextDecorator::DrawLine (const Point& beg, const Point& end, const Pen& pen)
{
	decorated.DrawLine (beg, end, GetChangedPen (pen));
}

void ColorChangerNodeContextDecorator::DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen)
{
	decorated.DrawBezier (p1, p2, p3, p4, GetChangedPen (pen));
}

void ColorChangerNodeContextDecorator::DrawRect (const Rect& rect, const Pen& pen)
{
	decorated.DrawRect (rect, GetChangedPen (pen));
}

void ColorChangerNodeContextDecorator::FillRect (const Rect& rect, const Color& color)
{
	decorated.FillRect (rect, GetChangedColor (color));
}

void ColorChangerNodeContextDecorator::DrawEllipse (const Rect& rect, const Pen& pen)
{
	decorated.DrawEllipse (rect, GetChangedPen (pen));
}

void ColorChangerNodeContextDecorator::FillEllipse (const Rect& rect, const Color& color)
{
	decorated.FillEllipse (rect, GetChangedColor (color));
}

void ColorChangerNodeContextDecorator::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color)
{
	return decorated.DrawFormattedText (rect, font, text, hAnchor, vAnchor, GetChangedColor (color));
}

Pen ColorChangerNodeContextDecorator::GetChangedPen (const Pen& origPen)
{
	return Pen (GetChangedColor (origPen.GetColor ()), origPen.GetThickness ());
}

ColorBlenderNodeContextDecorator::ColorBlenderNodeContextDecorator (NodeDrawingContext& decorated, const Color& blendColor) :
	ColorChangerNodeContextDecorator (decorated),
	blendColor (blendColor)
{

}

Color ColorBlenderNodeContextDecorator::GetChangedColor (const Color& origColor)
{
	return Color (
		(unsigned char) ((origColor.GetR () + blendColor.GetR ()) / 2.0),
		(unsigned char) ((origColor.GetG () + blendColor.GetG ()) / 2.0),
		(unsigned char) ((origColor.GetB () + blendColor.GetB ()) / 2.0)
	);
}

void ColorBlenderNodeContextDecorator::OnNodeDrawingBegin (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::Rect& rect)
{
	NE::Checksum myChecksum = checksum;
	AddColorToChecksum (myChecksum, blendColor);
	decorated.OnNodeDrawingBegin (nodeId, myChecksum, rect);
}

TextSkipperNodeContextDecorator::TextSkipperNodeContextDecorator (NodeDrawingContext& decorated, bool isPreviewMode) :
	NodeDrawingContextDecorator (decorated),
	isPreviewMode (isPreviewMode)
{

}

void TextSkipperNodeContextDecorator::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color)
{
	if (!isPreviewMode) {
		decorated.DrawFormattedText (rect, font, text, hAnchor, vAnchor, color);
	}
}

}
