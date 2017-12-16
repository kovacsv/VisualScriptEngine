#include "BitmapContextGdi.hpp"
#include "Debug.hpp"

PenCacheKey::PenCacheKey () :
	thickness (0),
	r (0),
	g (0),
	b (0)
{

}

PenCacheKey::PenCacheKey (const NUIE::Pen& pen) :
	thickness ((int) pen.GetThickness ()),
	r (pen.GetColor ().GetR ()),
	g (pen.GetColor ().GetG ()),
	b (pen.GetColor ().GetB ())
{

}

HANDLE PenCacheKey::CreateHandle ()
{
	return ::CreatePen (PS_SOLID, thickness, RGB (r, g, b));
}

bool PenCacheKey::operator== (const PenCacheKey& rhs) const
{
	return thickness == rhs.thickness && r == rhs.r && g == rhs.g && b == rhs.b;
}

BrushCacheKey::BrushCacheKey () :
	r (0),
	g (0),
	b (0)
{

}

BrushCacheKey::BrushCacheKey (const NUIE::Color& color) :
	r (color.GetR ()),
	g (color.GetG ()),
	b (color.GetB ())
{

}

HANDLE BrushCacheKey::CreateHandle ()
{
	return ::CreateSolidBrush (RGB (r, g, b));
}

bool BrushCacheKey::operator== (const BrushCacheKey& rhs) const
{
	return r == rhs.r && g == rhs.g && b == rhs.b;
}

FontCacheKey::FontCacheKey () :
	family (L""),
	size (0)
{

}

FontCacheKey::FontCacheKey (const NUIE::Font& font) :
	family (font.GetFamily ()),
	size ((int) (font.GetSize ()))
{

}

HANDLE FontCacheKey::CreateHandle ()
{
	return ::CreateFont (size, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, family.c_str ());
}

bool FontCacheKey::operator== (const FontCacheKey& rhs) const
{
	return family == rhs.family && size == rhs.size;
}

class SelectObjectGuard
{
public:
	SelectObjectGuard (HDC hdcToSelect, HANDLE objectToSelect)
	{
		hdc = hdcToSelect;
		oldHandle = SelectObject (hdc, objectToSelect);
	}

	~SelectObjectGuard ()
	{
		SelectObject (hdc, oldHandle);
	}

private:
	HDC		hdc;
	HANDLE	oldHandle;
};

BitmapContextGdi::BitmapContextGdi (int width, int height) :
	width (width),
	height (height),
	memoryDC (NULL),
	memoryBitmap (NULL)
{
	HDC hdc = GetDC (NULL);
	memoryDC = CreateCompatibleDC (hdc);
	memoryBitmap = CreateCompatibleBitmap (hdc, width, height);
}

BitmapContextGdi::~BitmapContextGdi ()
{
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);
}

void BitmapContextGdi::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);

	HDC hdc = GetDC (NULL);
	memoryDC = CreateCompatibleDC (hdc);
	memoryBitmap = CreateCompatibleBitmap (hdc, width, height);
}

double BitmapContextGdi::GetWidth () const
{
	return width;
}

double BitmapContextGdi::GetHeight () const
{
	return height;
}

void BitmapContextGdi::BeginDraw ()
{

}

void BitmapContextGdi::EndDraw ()
{

}

void BitmapContextGdi::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	POINT gdiBeg = CreatePoint (beg);
	POINT gdiEnd = CreatePoint (end);
	SelectObject (memoryDC, GetStockObject (NULL_BRUSH));
	SelectObject (memoryDC, penCache.Get (pen));
	::MoveToEx (memoryDC, gdiBeg.x, gdiBeg.y, NULL);
	::LineTo (memoryDC, gdiEnd.x, gdiEnd.y);
}

void BitmapContextGdi::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	POINT points[4] = {
		CreatePoint (p1),
		CreatePoint (p2),
		CreatePoint (p3),
		CreatePoint (p4)
	};
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	SelectObject (memoryDC, GetStockObject (NULL_BRUSH));
	SelectObject (memoryDC, penCache.Get (pen));
	::PolyBezier (memoryDC, points, 4);
}

void BitmapContextGdi::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	RECT gdiRect = CreateRect (rect, pen);
	SelectObject (memoryDC, GetStockObject (NULL_BRUSH));
	SelectObject (memoryDC, penCache.Get (pen));
    ::Rectangle (memoryDC, gdiRect.left, gdiRect.top, gdiRect.right, gdiRect.bottom);
}

void BitmapContextGdi::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	RECT gdiRect = CreateRect (rect);
	::FillRect (memoryDC, &gdiRect, (HBRUSH) brushCache.Get (color));
}

void BitmapContextGdi::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	RECT gdiRect = CreateRect (rect, pen);
	SelectObject (memoryDC, GetStockObject (NULL_BRUSH));
	SelectObject (memoryDC, penCache.Get (pen));
    ::Ellipse (memoryDC, gdiRect.left, gdiRect.top, gdiRect.right, gdiRect.bottom);
}

void BitmapContextGdi::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	RECT gdiRect = CreateRect (rect);
	SelectObject (memoryDC, brushCache.Get (color));
	SelectObject (memoryDC, GetStockObject (NULL_PEN));
    ::Ellipse (memoryDC, gdiRect.left, gdiRect.top, gdiRect.right, gdiRect.bottom);
}

void BitmapContextGdi::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& backgroundColor, const NUIE::Color& textColor)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);

	UINT format = 0;
	switch (hAnchor) {
		case NUIE::HorizontalAnchor::Left:
			format |= DT_LEFT;
			break;
		case NUIE::HorizontalAnchor::Center:
			format |= DT_CENTER;
			break;
		case NUIE::HorizontalAnchor::Right:
			format |= DT_RIGHT;
			break;
	}

	switch (vAnchor) {
		case NUIE::VerticalAnchor::Top:
			format |= DT_TOP;
			break;
		case NUIE::VerticalAnchor::Center:
			format |= DT_VCENTER | DT_SINGLELINE;
			break;
		case NUIE::VerticalAnchor::Bottom:
			format |= DT_BOTTOM;
			break;
	}
	
	RECT gdiRect = CreateRect (rect);
	SetBkColor (memoryDC, RGB (backgroundColor.GetR (), backgroundColor.GetG (), backgroundColor.GetB ()));
	SetTextColor (memoryDC, RGB (textColor.GetR (), textColor.GetG (), textColor.GetB ()));
	SelectObject (memoryDC, fontCache.Get (font));
	::DrawText (memoryDC, text.c_str (), (int) text.length (), &gdiRect, format);
}

NUIE::Size BitmapContextGdi::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	SelectObject (memoryDC, fontCache.Get (font));
	RECT gdiRect = { 0, 0, 0, 0 };
	::DrawText (memoryDC, text.c_str (), (int) text.length (), &gdiRect, DT_CALCRECT);
	return NUIE::Size (gdiRect.right - gdiRect.left + 5, gdiRect.bottom - gdiRect.top);
}

void BitmapContextGdi::StretchToContext (BitmapContextGdi& targetContext, int x, int y, int targetWidth, int targetHeight)
{
	SelectObjectGuard targetSelectGuard (targetContext.memoryDC, targetContext.memoryBitmap);
	StretchToHDC (targetContext.memoryDC, x, y, targetWidth, targetHeight);
}

void BitmapContextGdi::DrawToContext (BitmapContextGdi& targetContext, int x, int y)
{
	SelectObjectGuard targetSelectGuard (targetContext.memoryDC, targetContext.memoryBitmap);
	DrawToHDC (targetContext.memoryDC, x, y);
}

void BitmapContextGdi::DrawToHDC (HDC targetDC, int x, int y)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	BitBlt (targetDC, x, y, width, height, memoryDC, 0, 0, SRCCOPY);
}

void BitmapContextGdi::StretchToHDC (HDC targetDC, int x, int y, int targetWidth, int targetHeight)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	SetStretchBltMode (targetDC, HALFTONE);
	StretchBlt (targetDC, x, y, targetWidth, targetHeight, memoryDC, 0, 0, width, height, SRCCOPY);
}

POINT BitmapContextGdi::CreatePoint (const NUIE::Point& point) const
{
	POINT gdiPoint;
	gdiPoint.x = (int) std::floor (point.GetX ()) - 1;
	gdiPoint.y = (int) std::floor (point.GetY ()) - 1;
	return gdiPoint;
}

RECT BitmapContextGdi::CreateRect (const NUIE::Rect& rect) const
{
	RECT gdiRect;
	gdiRect.left = (int) std::floor (rect.GetLeft ()) - 1;
	gdiRect.top = (int) std::floor (rect.GetTop ()) - 1;
	gdiRect.right = (int) std::floor (rect.GetRight ());
	gdiRect.bottom = (int) std::floor (rect.GetBottom ());
	return gdiRect;
}

RECT BitmapContextGdi::CreateRect (const NUIE::Rect& rect, const NUIE::Pen& pen) const
{
	RECT gdiRect = CreateRect (rect);
	int penThickness = (int) std::floor (pen.GetThickness ());
	if (penThickness > 1) {
		int leftDiff = (int) (penThickness / 2.0);
		int rightDiff = penThickness - leftDiff - 1;
		gdiRect.left += leftDiff;
		gdiRect.top += leftDiff;
		gdiRect.right -= rightDiff;
		gdiRect.bottom -= rightDiff;
	}
	return gdiRect;
}
