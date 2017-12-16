#include "DrawingImage.hpp"

#include <algorithm>

namespace NUIE
{

DrawingItem::DrawingItem ()
{

}

DrawingItem::~DrawingItem ()
{

}

DrawingLine::DrawingLine (const Point& beg, const Point& end, const Pen& pen) :
	beg (beg),
	end (end),
	pen (pen)
{

}

DrawingLine::~DrawingLine ()
{

}

void DrawingLine::Draw (DrawingContext& context) const
{
	context.DrawLine (beg, end, pen);
}

void DrawingLine::AddToChecksum (NE::Checksum& checksum) const
{
	AddPointToChecksum (checksum, beg);
	AddPointToChecksum (checksum, end);
	AddPenToChecksum (checksum, pen);
}

DrawingBezier::DrawingBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) :
	p1 (p1),
	p2 (p2),
	p3 (p3),
	p4 (p4),
	pen (pen)
{

}

DrawingBezier::~DrawingBezier ()
{

}

void DrawingBezier::Draw (DrawingContext& context) const
{
	context.DrawBezier (p1, p2, p3, p4, pen);
}

void DrawingBezier::AddToChecksum (NE::Checksum& checksum) const
{
	AddPointToChecksum (checksum, p1);
	AddPointToChecksum (checksum, p2);
	AddPointToChecksum (checksum, p3);
	AddPointToChecksum (checksum, p4);
	AddPenToChecksum (checksum, pen);	
}

DrawingRect::DrawingRect (const Rect& rect, const Pen& pen) :
	rect (rect),
	pen (pen)
{

}

DrawingRect::~DrawingRect ()
{

}

void DrawingRect::Draw (DrawingContext& context) const
{
	context.DrawRect (rect, pen);
}

void DrawingRect::AddToChecksum (NE::Checksum& checksum) const
{
	AddRectToChecksum (checksum, rect);
	AddPenToChecksum (checksum, pen);
}

DrawingFillRect::DrawingFillRect (const Rect& rect, const Color& color) :
	rect (rect),
	color (color)
{

}

DrawingFillRect::~DrawingFillRect ()
{

}

void DrawingFillRect::Draw (DrawingContext& context) const
{
	context.FillRect (rect, color);
}

void DrawingFillRect::AddToChecksum (NE::Checksum& checksum) const
{
	AddRectToChecksum (checksum, rect);
	AddColorToChecksum (checksum, color);
}

DrawingEllipse::DrawingEllipse (const Rect& rect, const Pen& pen) :
	rect (rect),
	pen (pen)
{

}

DrawingEllipse::~DrawingEllipse ()
{

}

void DrawingEllipse::Draw (DrawingContext& context) const
{
	context.DrawEllipse (rect, pen);
}

void DrawingEllipse::AddToChecksum (NE::Checksum& checksum) const
{
	AddRectToChecksum (checksum, rect);
	AddPenToChecksum (checksum, pen);
}

DrawingFillEllipse::DrawingFillEllipse (const Rect& rect, const Color& color) :
	rect (rect),
	color (color)
{

}

DrawingFillEllipse::~DrawingFillEllipse ()
{

}

void DrawingFillEllipse::Draw (DrawingContext& context) const
{
	context.FillEllipse (rect, color);
}

void DrawingFillEllipse::AddToChecksum (NE::Checksum& checksum) const
{
	AddRectToChecksum (checksum, rect);
	AddColorToChecksum (checksum, color);
}

DrawingText::DrawingText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& backgroundColor, const Color& textColor) :
	rect (rect),
	font (font),
	text (text),
	hAnchor (hAnchor),
	vAnchor (vAnchor),
	backgroundColor (backgroundColor),
	textColor (textColor)
{

}

DrawingText::~DrawingText ()
{

}

void DrawingText::Draw (DrawingContext& context) const
{
	context.DrawFormattedText (rect, font, text, hAnchor, vAnchor, backgroundColor, textColor);
}

void DrawingText::AddToChecksum (NE::Checksum& checksum) const
{
	AddRectToChecksum (checksum, rect);
	AddFontToChecksum (checksum, font);
	checksum.Add (text);
	checksum.Add ((int) hAnchor);
	checksum.Add ((int) vAnchor);
	AddColorToChecksum (checksum, backgroundColor);
	AddColorToChecksum (checksum, textColor);
}

MultiDrawingItem::MultiDrawingItem ()
{

}

MultiDrawingItem::~MultiDrawingItem ()
{

}

void MultiDrawingItem::AddItem (const DrawingItemConstPtr& item)
{
	items.push_back (item);
}

void MultiDrawingItem::Draw (DrawingContext& context) const
{
	for (const DrawingItemConstPtr& item : items) {
		item->Draw (context);
	}
}

void MultiDrawingItem::AddToChecksum (NE::Checksum& checksum) const
{
	for (const DrawingItemConstPtr& item : items) {
		item->AddToChecksum (checksum);
	}
}

DrawingImage::DrawingImage ()
{

}

DrawingImage::~DrawingImage ()
{

}

bool DrawingImage::IsEmpty () const
{
	return items.empty ();
}

void DrawingImage::Clear ()
{
	items.clear ();
}

void DrawingImage::AddItem (const DrawingItemConstPtr& item)
{
	items.push_back (item);
}

void DrawingImage::RemoveItem (const DrawingItemConstPtr& item)
{
	auto found = std::find (items.begin (), items.end (), item);
	if (found != items.end ()) {
		items.erase (found);
	}
}

void DrawingImage::Draw (DrawingContext& context) const
{
	for (const DrawingItemConstPtr& item : items) {
		item->Draw (context);
	}
}

NE::Checksum DrawingImage::GetChecksum () const
{
	NE::Checksum checksum;
	for (const DrawingItemConstPtr& item : items) {
		item->AddToChecksum (checksum);
	}
	return checksum;
}

}
