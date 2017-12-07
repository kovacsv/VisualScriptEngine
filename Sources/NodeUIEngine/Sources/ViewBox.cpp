#include "ViewBox.hpp"

namespace NUIE
{

ViewBox::ViewBox (const Point& offset, double scale) :
	offset (offset),
	scale (scale)
{
}

void ViewBox::Reset ()
{
	offset.Set (0.0, 0.0);
	scale = 1.0;
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

NE::Stream::Status ReadViewBox (NE::InputStream& inputStream, ViewBox& viewBox)
{
	Point offset;
	double scale = 0.0;
	ReadPoint (inputStream, offset);
	inputStream.Read (scale);
	viewBox.SetOffset (offset);
	viewBox.SetScale (scale);
	return inputStream.GetStatus ();
}

NE::Stream::Status WriteViewBox (NE::OutputStream& outputStream, const ViewBox& viewBox)
{
	WritePoint (outputStream, viewBox.GetOffset ());
	outputStream.Write (viewBox.GetScale ());
	return outputStream.GetStatus ();
}

}
