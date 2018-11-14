#include "NUIE_DrawingImage.hpp"

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

DrawingText::DrawingText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor) :
	rect (rect),
	font (font),
	text (text),
	hAnchor (hAnchor),
	vAnchor (vAnchor),
	textColor (textColor)
{

}

DrawingText::~DrawingText ()
{

}

void DrawingText::Draw (DrawingContext& context) const
{
	context.DrawFormattedText (rect, font, text, hAnchor, vAnchor, textColor);
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

}
