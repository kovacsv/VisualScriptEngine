#include "BitmapContextGdi.hpp"
#include "Debug.hpp"

HANDLE CreateHandle (const PenCacheKey& key)
{
	return ::CreatePen (PS_SOLID, key.thickness, RGB (key.r, key.g, key.b));
}

HANDLE CreateHandle (const BrushCacheKey& key)
{
	return ::CreateSolidBrush (RGB (key.r, key.g, key.b));
}

HANDLE CreateHandle (const FontCacheKey& key)
{
	return ::CreateFont (key.size, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, key.family.c_str ());
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

BitmapContextGdi::BitmapContextGdi () :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0),
	memoryDC (NULL),
	memoryBitmap (NULL)
{

}

BitmapContextGdi::~BitmapContextGdi ()
{
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);
}

void BitmapContextGdi::Init (void* nativeHandle)
{
	HWND hwnd = (HWND) nativeHandle;

	RECT clientRect;
	GetClientRect (hwnd, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;

	HDC hdc = GetDC (NULL);
	memoryDC = CreateCompatibleDC (hdc);
	memoryBitmap = CreateCompatibleBitmap (hdc, width, height);
}

void BitmapContextGdi::Blit (void* nativeHandle)
{
	HWND hwnd = (HWND) nativeHandle;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	BitBlt (hdc, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);
	EndPaint (hwnd, &ps);
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

void BitmapContextGdi::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
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
	SetBkMode (memoryDC, TRANSPARENT);
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

POINT BitmapContextGdi::CreatePoint (const NUIE::Point& point) const
{
	POINT gdiPoint;
	gdiPoint.x = (int) std::floor (point.GetX ()) - 1;
	gdiPoint.y = (int) std::floor (point.GetY ()) - 1;
	return gdiPoint;
}

RECT BitmapContextGdi::CreateRect (const NUIE::Rect& rect) const
{
	NUIE::IntRect intRect (rect);
	RECT gdiRect;
	gdiRect.left = intRect.GetX ();
	gdiRect.top = intRect.GetY ();
	gdiRect.right = intRect.GetX () + intRect.GetWidth ();
	gdiRect.bottom = intRect.GetY () + intRect.GetHeight ();
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
