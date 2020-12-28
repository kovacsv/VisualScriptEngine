#include "WAS_GdiplusOffscreenContext.hpp"
#include "WAS_OffscreenBitmap.hpp"
#include "NE_Debug.hpp"

#include <cmath>

namespace WAS
{

static Gdiplus::Point CreatePoint (const NUIE::Point& point)
{
	Gdiplus::Point gdiPoint ((int) std::ceil (point.GetX ()), (int) std::ceil (point.GetY ()));
	return gdiPoint;
}

static Gdiplus::Rect CreateRect (const NUIE::Rect& rect)
{
	Gdiplus::Rect gdiRect ((int) std::ceil (rect.GetLeft ()), (int) std::ceil (rect.GetTop ()), (int) std::ceil (rect.GetWidth ()), (int) std::ceil (rect.GetHeight ()));
	return gdiRect;
}

static Gdiplus::RectF CreateRectF (const NUIE::Rect& rect)
{
	Gdiplus::RectF gdiRect ((float) rect.GetLeft (), (float) rect.GetTop (), (float) rect.GetWidth (), (float) rect.GetHeight ());
	return gdiRect;
}

class DisableAntialiasGuard
{
public:
	DisableAntialiasGuard (Gdiplus::Graphics* graphics) :
		graphics (graphics),
		oldMode (graphics->GetSmoothingMode ())
	{
		graphics->SetSmoothingMode (Gdiplus::SmoothingMode::SmoothingModeNone);
	}

	~DisableAntialiasGuard ()
	{
		graphics->SetSmoothingMode (oldMode);
	}

private:
	Gdiplus::Graphics*		graphics;
	Gdiplus::SmoothingMode	oldMode;
};

GdiplusOffscreenContext::GdiplusOffscreenContext () :
	NUIE::NativeDrawingContext (),
	gdiplusInitializer (),
	width (0),
	height (0),
	bitmap (new Gdiplus::Bitmap (width, height)),
	graphics (new Gdiplus::Graphics (bitmap.get ()))
{
	
}

GdiplusOffscreenContext::~GdiplusOffscreenContext ()
{

}

void GdiplusOffscreenContext::Init (void* nativeHandle)
{
	HWND hwnd = (HWND) nativeHandle;

	RECT clientRect;
	GetClientRect (hwnd, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;
	InitGraphics ();
}

void GdiplusOffscreenContext::BlitToWindow (void* nativeHandle)
{
	HWND hwnd = (HWND) nativeHandle;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);
	BlitToContext (hdc);
	EndPaint (hwnd, &ps);
}

void GdiplusOffscreenContext::BlitToContext (void* nativeContext)
{
	HDC hdc = (HDC) nativeContext;

	OffscreenBitmap offscreenBitmap;
	offscreenBitmap.Init (hdc, width, height);

	HANDLE oldHandle = offscreenBitmap.SelectBitmapObject ();
	Gdiplus::Graphics targetGraphics (offscreenBitmap.GetContext ());
	targetGraphics.DrawImage (bitmap.get (), 0, 0, width, height);
	targetGraphics.ReleaseHDC (offscreenBitmap.GetContext ());
	BitBlt (hdc, 0, 0, width, height, offscreenBitmap.GetContext (), 0, 0, SRCCOPY);
	offscreenBitmap.SelectOtherObject (oldHandle);
}

void GdiplusOffscreenContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	InitGraphics ();
}

int GdiplusOffscreenContext::GetWidth () const
{
	return width;
}

int GdiplusOffscreenContext::GetHeight () const
{
	return height;
}

void GdiplusOffscreenContext::BeginDraw ()
{
	graphics->SetSmoothingMode (Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
}

void GdiplusOffscreenContext::EndDraw ()
{

}

bool GdiplusOffscreenContext::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void GdiplusOffscreenContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	const NUIE::Color& color = pen.GetColor ();
	Gdiplus::Pen gdiPen (Gdiplus::Color (color.GetR (), color.GetG (), color.GetB ()), (Gdiplus::REAL) pen.GetThickness ());
	gdiPen.SetLineCap (Gdiplus::LineCap::LineCapRound, Gdiplus::LineCap::LineCapRound, Gdiplus::DashCap::DashCapRound);
	graphics->DrawLine (&gdiPen, CreatePoint (beg), CreatePoint (end));
}

void GdiplusOffscreenContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	const NUIE::Color& color = pen.GetColor ();
	Gdiplus::Pen gdiPen (Gdiplus::Color (color.GetR (), color.GetG (), color.GetB ()), (Gdiplus::REAL) pen.GetThickness ());
	gdiPen.SetLineCap (Gdiplus::LineCap::LineCapRound, Gdiplus::LineCap::LineCapRound, Gdiplus::DashCap::DashCapRound);
	graphics->DrawBezier (&gdiPen, CreatePoint (p1), CreatePoint (p2), CreatePoint (p3), CreatePoint (p4));
}

void GdiplusOffscreenContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	DisableAntialiasGuard antialiasGuard (graphics.get ());
	const NUIE::Color& color = pen.GetColor ();
	Gdiplus::Pen gdiPen (Gdiplus::Color (color.GetR (), color.GetG (), color.GetB ()), (Gdiplus::REAL) pen.GetThickness ());
	gdiPen.SetLineCap (Gdiplus::LineCap::LineCapRound, Gdiplus::LineCap::LineCapRound, Gdiplus::DashCap::DashCapRound);
	Gdiplus::Rect gdiRect = CreateRect (rect);
	graphics->DrawRectangle (&gdiPen, gdiRect);
}

void GdiplusOffscreenContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	DisableAntialiasGuard antialiasGuard (graphics.get ());
	Gdiplus::SolidBrush brush (Gdiplus::Color (color.GetR (), color.GetG (), color.GetB ()));
	Gdiplus::Rect gdiRect = CreateRect (rect);
	graphics->FillRectangle (&brush, gdiRect);
}

void GdiplusOffscreenContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	const NUIE::Color& color = pen.GetColor ();
	Gdiplus::Pen gdiPen (Gdiplus::Color (color.GetR (), color.GetG (), color.GetB ()), (Gdiplus::REAL) pen.GetThickness ());
	Gdiplus::Rect gdiRect = CreateRect (rect);
	graphics->DrawEllipse (&gdiPen, gdiRect);
}

void GdiplusOffscreenContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	Gdiplus::SolidBrush brush (Gdiplus::Color (color.GetR (), color.GetG (), color.GetB ()));
	Gdiplus::Rect gdiRect = CreateRect (rect);
	graphics->FillEllipse (&brush, gdiRect);
}

void GdiplusOffscreenContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	Gdiplus::Font gdiFont (font.GetFamily ().c_str (), (Gdiplus::REAL) font.GetSize ());
	Gdiplus::SolidBrush brush (Gdiplus::Color (textColor.GetR (), textColor.GetG (), textColor.GetB ()));
	Gdiplus::RectF gdiRect = CreateRectF (rect);
	
	Gdiplus::StringFormat format;

	switch (hAnchor) {
		case NUIE::HorizontalAnchor::Left:
			format.SetAlignment (Gdiplus::StringAlignment::StringAlignmentNear);
			break;
		case NUIE::HorizontalAnchor::Center:
			format.SetAlignment (Gdiplus::StringAlignment::StringAlignmentCenter);
			break;
		case NUIE::HorizontalAnchor::Right:
			format.SetAlignment (Gdiplus::StringAlignment::StringAlignmentFar);
			break;
	}

	switch (vAnchor) {
		case NUIE::VerticalAnchor::Top:
			format.SetLineAlignment (Gdiplus::StringAlignment::StringAlignmentNear);
			break;
		case NUIE::VerticalAnchor::Center:
			format.SetLineAlignment (Gdiplus::StringAlignment::StringAlignmentCenter);
			break;
		case NUIE::VerticalAnchor::Bottom:
			format.SetLineAlignment (Gdiplus::StringAlignment::StringAlignmentFar);
			break;
	}

	graphics->DrawString (text.c_str (), (int) text.length (), &gdiFont, gdiRect, &format, &brush);
}

NUIE::Size GdiplusOffscreenContext::MeasureText (const NUIE::Font& font, const std::wstring & text)
{
	Gdiplus::Font gdiFont (font.GetFamily ().c_str (), (Gdiplus::REAL) font.GetSize ());
	Gdiplus::RectF textRect (0, 0, 0, 0);

	if (graphics->MeasureString (text.c_str (), (int) text.length (), &gdiFont, textRect, NULL, &textRect) != Gdiplus::Status::Ok) {
		DBGBREAK ();
		return NUIE::Size (100, 20);
	}

	return NUIE::Size (textRect.Width, textRect.Height);
}

bool GdiplusOffscreenContext::CanDrawIcon ()
{
	return false;
}

void GdiplusOffscreenContext::DrawIcon (const NUIE::Rect&, const NUIE::IconId&)
{
	DBGBREAK ();
}

void GdiplusOffscreenContext::InitGraphics ()
{
	bitmap.reset (new Gdiplus::Bitmap (width, height));
	graphics.reset (new Gdiplus::Graphics (bitmap.get ()));
}

}
