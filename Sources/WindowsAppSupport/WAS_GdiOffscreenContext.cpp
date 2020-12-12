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
	return ::CreateFont (key.size, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, key.family.c_str ());
}

class SelectObjectGuard
{
public:
	SelectObjectGuard (HDC hdcToSelect, HANDLE objectToSelect) :
		hdc (NULL),
		oldHandle (NULL)
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

GdiOffscreenContext::GdiOffscreenContext () :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0),
	memoryDC (NULL),
	memoryBitmap (NULL)
{

}

GdiOffscreenContext::~GdiOffscreenContext ()
{
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);
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
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	BitBlt (hdc, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);
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
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	POINT gdiBeg = CreatePoint (beg);
	POINT gdiEnd = CreatePoint (end);
	SelectObject (memoryDC, GetStockObject (NULL_BRUSH));
	SelectObject (memoryDC, penCache.Get (pen));
	::MoveToEx (memoryDC, gdiBeg.x, gdiBeg.y, NULL);
	::LineTo (memoryDC, gdiEnd.x, gdiEnd.y);
}

void GdiOffscreenContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	POINT points[4] = {
		CreatePoint (p1),
		CreatePoint (p2),
		CreatePoint (p3),
		CreatePoint (p4)
	};
	SelectObject (memoryDC, GetStockObject (NULL_BRUSH));
	SelectObject (memoryDC, penCache.Get (pen));
	::PolyBezier (memoryDC, points, 4);
}

void GdiOffscreenContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	RECT gdiRect = CreateRect (rect, pen);
	SelectObject (memoryDC, GetStockObject (NULL_BRUSH));
	SelectObject (memoryDC, penCache.Get (pen));
    ::Rectangle (memoryDC, gdiRect.left, gdiRect.top, gdiRect.right, gdiRect.bottom);
}

void GdiOffscreenContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	RECT gdiRect = CreateRect (rect);
	::FillRect (memoryDC, &gdiRect, (HBRUSH) brushCache.Get (color));
}

void GdiOffscreenContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	RECT gdiRect = CreateRect (rect, pen);
	SelectObject (memoryDC, GetStockObject (NULL_BRUSH));
	SelectObject (memoryDC, penCache.Get (pen));
    ::Ellipse (memoryDC, gdiRect.left, gdiRect.top, gdiRect.right, gdiRect.bottom);
}

void GdiOffscreenContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	RECT gdiRect = CreateRect (rect);
	SelectObject (memoryDC, brushCache.Get (color));
	SelectObject (memoryDC, GetStockObject (NULL_PEN));
    ::Ellipse (memoryDC, gdiRect.left, gdiRect.top, gdiRect.right, gdiRect.bottom);
}

void GdiOffscreenContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
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

NUIE::Size GdiOffscreenContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	SelectObjectGuard selectGuard (memoryDC, memoryBitmap);
	SelectObject (memoryDC, fontCache.Get (font));
	RECT gdiRect = { 0, 0, 0, 0 };
	::DrawText (memoryDC, text.c_str (), (int) text.length (), &gdiRect, DT_CALCRECT);
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
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);

	HDC hdc = GetDC (NULL);
	memoryDC = CreateCompatibleDC (hdc);
	memoryBitmap = CreateCompatibleBitmap (hdc, width, height);
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
	gdiRect.left = intRect.GetX ();
	gdiRect.top = intRect.GetY ();
	gdiRect.right = intRect.GetX () + intRect.GetWidth ();
	gdiRect.bottom = intRect.GetY () + intRect.GetHeight ();
	return gdiRect;
}

RECT GdiOffscreenContext::CreateRect (const NUIE::Rect& rect, const NUIE::Pen& pen) const
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

}
