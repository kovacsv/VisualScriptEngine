#include "NUIE_DrawingContext.hpp"

namespace NUIE
{

DrawingContext::DrawingContext ()
{

}

DrawingContext::~DrawingContext ()
{

}

DrawingContextDecorator::DrawingContextDecorator (DrawingContext& decorated) :
	decorated (decorated)
{

}

DrawingContextDecorator::~DrawingContextDecorator ()
{

}

void DrawingContextDecorator::Resize (int newWidth, int newHeight)
{
	decorated.Resize (newWidth, newHeight);
}

int DrawingContextDecorator::GetWidth () const
{
	return decorated.GetWidth ();
}

int DrawingContextDecorator::GetHeight () const
{
	return decorated.GetHeight ();
}

void DrawingContextDecorator::BeginDraw ()
{
	decorated.BeginDraw ();
}

void DrawingContextDecorator::EndDraw ()
{
	decorated.EndDraw ();
}

bool DrawingContextDecorator::NeedToDraw (ItemPreviewMode mode)
{
	return decorated.NeedToDraw (mode);
}

void DrawingContextDecorator::DrawLine (const Point& beg, const Point& end, const Pen& pen)
{
	decorated.DrawLine (beg, end, pen);
}

void DrawingContextDecorator::DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen)
{
	decorated.DrawBezier (p1, p2, p3, p4, pen);
}

void DrawingContextDecorator::DrawRect (const Rect& rect, const Pen& pen)
{
	decorated.DrawRect (rect, pen);
}

void DrawingContextDecorator::FillRect (const Rect& rect, const Color& color)
{
	decorated.FillRect (rect, color);
}

void DrawingContextDecorator::DrawEllipse (const Rect& rect, const Pen& pen)
{
	decorated.DrawEllipse (rect, pen);
}

void DrawingContextDecorator::FillEllipse (const Rect& rect, const Color& color)
{
	decorated.FillEllipse (rect, color);
}

void DrawingContextDecorator::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor)
{
	decorated.DrawFormattedText (rect, font, text, hAnchor, vAnchor, textColor);
}

Size DrawingContextDecorator::MeasureText (const Font& font, const std::wstring& text)
{
	return decorated.MeasureText (font, text);
}

bool DrawingContextDecorator::CanDrawIcon ()
{
	return decorated.CanDrawIcon ();
}

void DrawingContextDecorator::DrawIcon (const Rect& rect, const IconId& iconId)
{
	decorated.DrawIcon (rect, iconId);
}

void NullDrawingContext::Resize (int, int)
{

}

int NullDrawingContext::GetWidth () const
{
	return 0;
}

int NullDrawingContext::GetHeight () const
{
	return 0;
}

void NullDrawingContext::BeginDraw ()
{

}

void NullDrawingContext::EndDraw ()
{

}

bool NullDrawingContext::NeedToDraw (ItemPreviewMode)
{
	return false;
}

void NullDrawingContext::DrawLine (const Point&, const Point&, const Pen&)
{

}

void NullDrawingContext::DrawBezier (const Point&, const Point&, const Point&, const Point&, const Pen&)
{

}

void NullDrawingContext::DrawRect (const Rect&, const Pen&)
{

}

void NullDrawingContext::FillRect (const Rect&, const Color&)
{

}

void NullDrawingContext::DrawEllipse (const Rect&, const Pen&)
{

}

void NullDrawingContext::FillEllipse (const Rect&, const Color&)
{

}

void NullDrawingContext::DrawFormattedText (const Rect&, const Font&, const std::wstring&, HorizontalAnchor, VerticalAnchor, const Color&)
{

}

Size NullDrawingContext::MeasureText (const Font&, const std::wstring&)
{
	return Size ();
}

bool NullDrawingContext::CanDrawIcon ()
{
	return false;
}

void NullDrawingContext::DrawIcon (const Rect&, const IconId&)
{

}

}
