#include "NUIE_ViewBox.hpp"

namespace NUIE
{

ViewBox::ViewBox (const Point& offset, double scale) :
	offset (offset),
	scale (scale)
{
}

ViewBox::ViewBox () :
	offset (Point ()),
	scale (1.0)
{

}

void ViewBox::Reset ()
{
	offset.Set (0.0, 0.0);
	scale = 1.0;
}

void ViewBox::Set (const Point& newOffset, double newScale)
{
	offset = newOffset;
	scale = newScale;
}

const Point& ViewBox::GetOffset () const
{
	return offset;
}

void ViewBox::SetOffset (const Point& newOffset)
{
	offset = newOffset;
}

double ViewBox::GetScale () const
{
	return scale;
}

void ViewBox::SetScale (double newScale)
{
	scale = newScale;
}

void ViewBox::SetScale (double newScale, const Point& fixViewPoint)
{
	offset = fixViewPoint - ((fixViewPoint - offset) * newScale) / scale;
	scale = newScale;
}

Point ViewBox::ModelToView (const Point& point) const
{
	return offset + (point * scale);
}

Size ViewBox::ModelToView (const Size& size) const
{
	return size * scale;
}

Rect ViewBox::ModelToView (const Rect& rect) const
{
	return Rect::FromPositionAndSize (ModelToView (rect.GetTopLeft ()), ModelToView (rect.GetSize ()));
}

Point ViewBox::ViewToModel (const Point& point) const
{
	return (point - offset) / scale;
}

Rect ViewBox::ViewToModel (const Rect& rect) const
{
	return Rect::FromPositionAndSize (ViewToModel (rect.GetTopLeft ()), ViewToModel (rect.GetSize ()));
}

Size ViewBox::ViewToModel (const Size& size) const
{
	return size / scale;
}

Pen ViewBox::ModelToView (const Pen& pen) const
{
	return Pen (pen.GetColor (), pen.GetThickness () * scale);
}

Font ViewBox::ModelToView (const Font& font) const
{
	return Font (font.GetFamily (), font.GetSize () * scale);
}

ViewBox FitRectToSize (const Size& targetSize, double targetPadding, const Rect& rect)
{
	double targetWidth = targetSize.GetWidth () - 2.0 * targetPadding;
	double targetHeight = targetSize.GetHeight () - 2.0 * targetPadding;

	double scale = 0.0;
	double targetScale = targetWidth / targetHeight;
	double rectScale = rect.GetWidth () / rect.GetHeight ();
	if (rectScale > targetScale) {
		scale = targetWidth / rect.GetWidth ();
	} else {
		scale = targetHeight / rect.GetHeight ();
	}

	ViewBox scaleViewBox (Point (0.0, 0.0), scale);
	Point newOffset = scaleViewBox.ModelToView (-rect.GetTopLeft ()) + Point (targetPadding, targetPadding);

	Rect viewBoundingRect = scaleViewBox.ModelToView (rect);
	Point centerOffset (
		(viewBoundingRect.GetWidth () - targetWidth) / 2.0,
		(viewBoundingRect.GetHeight () - targetHeight) / 2.0
	);

	return ViewBox (newOffset - centerOffset, scale);
}

ViewBox CenterRectToSize (const Size& targetSize, double targetScale, const Rect& rect)
{
	Point rectPosition = rect.GetPosition () * targetScale;
	Size rectSize = rect.GetSize () * targetScale;

	Point centerTopLeft (
		(targetSize.GetWidth () - rectSize.GetWidth ()) / 2.0 - rectPosition.GetX (),
		(targetSize.GetHeight () - rectSize.GetHeight ()) / 2.0 - rectPosition.GetY ()
	);

	return ViewBox (centerTopLeft, targetScale);
}

}
