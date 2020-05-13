#include "NUIE_ContextDecorators.hpp"

namespace NUIE
{

ViewBoxContextDecorator::ViewBoxContextDecorator (DrawingContext& decorated, const ViewBox& viewBox) :
	DrawingContextDecorator (decorated),
	viewBox (viewBox)
{

}

void ViewBoxContextDecorator::DrawLine (const Point& beg, const Point& end, const Pen& pen)
{
	decorated.DrawLine (viewBox.ModelToView (beg), viewBox.ModelToView (end), viewBox.ModelToView (pen));
}

void ViewBoxContextDecorator::DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen)
{
	decorated.DrawBezier (viewBox.ModelToView (p1), viewBox.ModelToView (p2), viewBox.ModelToView (p3), viewBox.ModelToView (p4), viewBox.ModelToView (pen));
}

void ViewBoxContextDecorator::DrawRect (const Rect& rect, const Pen& pen)
{
	decorated.DrawRect (viewBox.ModelToView (rect), viewBox.ModelToView (pen));
}

void ViewBoxContextDecorator::FillRect (const Rect& rect, const Color& color)
{
	decorated.FillRect (viewBox.ModelToView (rect), color);
}

void ViewBoxContextDecorator::DrawEllipse (const Rect& rect, const Pen& pen)
{
	decorated.DrawEllipse (viewBox.ModelToView (rect), viewBox.ModelToView (pen));
}

void ViewBoxContextDecorator::FillEllipse (const Rect& rect, const Color& color)
{
	decorated.FillEllipse (viewBox.ModelToView (rect), color);
}

void ViewBoxContextDecorator::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor)
{
	decorated.DrawFormattedText (viewBox.ModelToView (rect), viewBox.ModelToView (font), text, hAnchor, vAnchor, textColor);
}

void ViewBoxContextDecorator::DrawIcon (const Rect& rect, const IconId& iconId)
{
	decorated.DrawIcon (viewBox.ModelToView (rect), iconId);
}

ColorChangerContextDecorator::ColorChangerContextDecorator (DrawingContext& decorated) :
	DrawingContextDecorator (decorated)
{

}

void ColorChangerContextDecorator::DrawLine (const Point& beg, const Point& end, const Pen& pen)
{
	decorated.DrawLine (beg, end, GetChangedPen (pen));
}

void ColorChangerContextDecorator::DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen)
{
	decorated.DrawBezier (p1, p2, p3, p4, GetChangedPen (pen));
}

void ColorChangerContextDecorator::DrawRect (const Rect& rect, const Pen& pen)
{
	decorated.DrawRect (rect, GetChangedPen (pen));
}

void ColorChangerContextDecorator::FillRect (const Rect& rect, const Color& color)
{
	decorated.FillRect (rect, GetChangedColor (color));
}

void ColorChangerContextDecorator::DrawEllipse (const Rect& rect, const Pen& pen)
{
	decorated.DrawEllipse (rect, GetChangedPen (pen));
}

void ColorChangerContextDecorator::FillEllipse (const Rect& rect, const Color& color)
{
	decorated.FillEllipse (rect, GetChangedColor (color));
}

void ColorChangerContextDecorator::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor)
{
	return decorated.DrawFormattedText (rect, font, text, hAnchor, vAnchor, GetChangedColor (textColor));
}

void ColorChangerContextDecorator::DrawIcon (const Rect& rect, const IconId& iconId)
{
	return decorated.DrawIcon (rect, iconId);
}

Pen ColorChangerContextDecorator::GetChangedPen (const Pen& origPen)
{
	return Pen (GetChangedColor (origPen.GetColor ()), origPen.GetThickness ());
}

ColorBlenderContextDecorator::ColorBlenderContextDecorator (DrawingContext& decorated, const BlendColor& blendColor) :
	ColorChangerContextDecorator (decorated),
	blendColor (blendColor)
{

}

Color ColorBlenderContextDecorator::GetChangedColor (const Color& origColor)
{
	const Color& newColor = blendColor.GetColor ();
	double blendRatio = blendColor.GetRatio ();
	double origRatio = 1.0 - blendColor.GetRatio ();
	return Color (
		(unsigned char) (origRatio * origColor.GetR () + blendRatio * newColor.GetR ()),
		(unsigned char) (origRatio * origColor.GetG () + blendRatio * newColor.GetG ()),
		(unsigned char) (origRatio * origColor.GetB () + blendRatio * newColor.GetB ())
	);
}

PreviewContextDecorator::PreviewContextDecorator (DrawingContext& decorated, bool isPreviewMode) :
	DrawingContextDecorator (decorated),
	isPreviewMode (isPreviewMode)
{

}

bool PreviewContextDecorator::NeedToDraw (ItemPreviewMode mode)
{
	if (isPreviewMode && mode == ItemPreviewMode::HideInPreview) {
		return false;
	}
	return true;
}

}
