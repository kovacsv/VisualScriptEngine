#include "WAS_GdiOffscreenContext.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

HANDLE CreateHandle (const NUIE::PenCacheKey& key)
{
	return ::CreatePen (PS_SOLID, key.thickness, RGB (key.r, key.g, key.b));
}

HANDLE CreateHandle (const NUIE::ColorCacheKey& key)
{
	return ::CreateSolidBrush (RGB (key.r, key.g, key.b));
}

HANDLE CreateHandle (const NUIE::FontCacheKey& key)
{
	return ::CreateFont (key.size, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, key.family.c_str ());
}

GdiOffscreenContext::GdiOffscreenContext () :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0),
	bitmap ()
{

}

GdiOffscreenContext::~GdiOffscreenContext ()
{
	
}

void GdiOffscreenContext::Init (void* nativeHandle)
{
	HWND hwnd = (HWND) nativeHandle;

	RECT clientRect;
	GetClientRect (hwnd, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;

	CreateOffscreenContext ();
}

void GdiOffscreenContext::BlitToWindow (void* nativeHandle)
{
	HWND hwnd = (HWND) nativeHandle;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);
	BlitToContext (hdc);
	EndPaint (hwnd, &ps);
}

void GdiOffscreenContext::BlitToContext (void* nativeContext)
{
	HDC hdc = (HDC) nativeContext;
	SelectBitmapGuard selectGuard (bitmap);
	BitBlt (hdc, 0, 0, width, height, bitmap.GetContext (), 0, 0, SRCCOPY);
}

void GdiOffscreenContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	CreateOffscreenContext ();
}

int GdiOffscreenContext::GetWidth () const
{
	return width;
}

int GdiOffscreenContext::GetHeight () const
{
	return height;
}

void GdiOffscreenContext::BeginDraw ()
{

}

void GdiOffscreenContext::EndDraw ()
{

}

bool GdiOffscreenContext::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void GdiOffscreenContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	SelectBitmapGuard selectGuard (bitmap);
	POINT gdiBeg = CreatePoint (beg);
	POINT gdiEnd = CreatePoint (end);
	bitmap.SelectOtherObject (GetStockObject (NULL_BRUSH));
	bitmap.SelectOtherObject (penCache.Get (pen));
	::MoveToEx (bitmap.GetContext (), gdiBeg.x, gdiBeg.y, NULL);
	::LineTo (bitmap.GetContext (), gdiEnd.x, gdiEnd.y);
}

void GdiOffscreenContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	SelectBitmapGuard selectGuard (bitmap);
	POINT points[4] = {
		CreatePoint (p1),
		CreatePoint (p2),
		CreatePoint (p3),
		CreatePoint (p4)
	};
	bitmap.SelectOtherObject (GetStockObject (NULL_BRUSH));
	bitmap.SelectOtherObject (penCache.Get (pen));
	::PolyBezier (bitmap.GetContext (), points, 4);
}

void GdiOffscreenContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	SelectBitmapGuard selectGuard (bitmap);
	RECT gdiRect = CreateRect (rect, pen);
	bitmap.SelectOtherObject (GetStockObject (NULL_BRUSH));
	bitmap.SelectOtherObject (penCache.Get (pen));
    ::Rectangle (bitmap.GetContext (), gdiRect.left, gdiRect.top, gdiRect.right, gdiRect.bottom);
}

void GdiOffscreenContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	SelectBitmapGuard selectGuard (bitmap);
	RECT gdiRect = CreateRect (rect);
	::FillRect (bitmap.GetContext (), &gdiRect, (HBRUSH) brushCache.Get (color));
}

void GdiOffscreenContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	SelectBitmapGuard selectGuard (bitmap);
	RECT gdiRect = CreateRect (rect, pen);
	bitmap.SelectOtherObject (GetStockObject (NULL_BRUSH));
	bitmap.SelectOtherObject (penCache.Get (pen));
    ::Ellipse (bitmap.GetContext (), gdiRect.left, gdiRect.top, gdiRect.right, gdiRect.bottom);
}

void GdiOffscreenContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	SelectBitmapGuard selectGuard (bitmap);
	RECT gdiRect = CreateRect (rect);
	bitmap.SelectOtherObject (brushCache.Get (color));
	bitmap.SelectOtherObject (GetStockObject (NULL_PEN));
    ::Ellipse (bitmap.GetContext (), gdiRect.left, gdiRect.top, gdiRect.right, gdiRect.bottom);
}

void GdiOffscreenContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	SelectBitmapGuard selectGuard (bitmap);

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
	SetBkMode (bitmap.GetContext (), TRANSPARENT);
	SetTextColor (bitmap.GetContext (), RGB (textColor.GetR (), textColor.GetG (), textColor.GetB ()));
	bitmap.SelectOtherObject (fontCache.Get (font));
	::DrawText (bitmap.GetContext (), text.c_str (), (int) text.length (), &gdiRect, format);
}

NUIE::Size GdiOffscreenContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	SelectBitmapGuard selectGuard (bitmap);
	bitmap.SelectOtherObject (fontCache.Get (font));
	RECT gdiRect = { 0, 0, 0, 0 };
	::DrawText (bitmap.GetContext (), text.c_str (), (int) text.length (), &gdiRect, DT_CALCRECT);
	return NUIE::Size (gdiRect.right - gdiRect.left + 5, gdiRect.bottom - gdiRect.top);
}

bool GdiOffscreenContext::CanDrawIcon ()
{
	return false;
}

void GdiOffscreenContext::DrawIcon (const NUIE::Rect&, const NUIE::IconId&)
{
	DBGBREAK ();
}

void GdiOffscreenContext::CreateOffscreenContext ()
{
	bitmap.Init (width, height);
}

POINT GdiOffscreenContext::CreatePoint (const NUIE::Point& point) const
{
	NUIE::IntPoint intPoint (point);
	POINT gdiPoint;
	gdiPoint.x = intPoint.GetX ();
	gdiPoint.y = intPoint.GetY ();
	return gdiPoint;
}

RECT GdiOffscreenContext::CreateRect (const NUIE::Rect& rect) const
{
	NUIE::IntRect intRect (rect);
	RECT gdiRect;
	gdiRect.left = intRect.GetLeft ();
	gdiRect.top = intRect.GetTop ();
	gdiRect.right = intRect.GetRight ();
	gdiRect.bottom = intRect.GetBottom ();
	return gdiRect;
}

RECT GdiOffscreenContext::CreateRect (const NUIE::Rect& rect, const NUIE::Pen& pen) const
{
	RECT gdiRect = CreateRect (rect);
	int penThickness = (int) std::floor (pen.GetThickness ());
	if (penThickness > 1) {
		int begDiff = (int) (penThickness / 2.0);
		int endDiff = penThickness - begDiff - 1;
		gdiRect.left += begDiff;
		gdiRect.top += begDiff;
		gdiRect.right -= endDiff;
		gdiRect.bottom -= endDiff;
	}
	return gdiRect;
}

}
