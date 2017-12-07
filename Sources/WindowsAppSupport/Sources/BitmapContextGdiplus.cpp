#include "BitmapContextGdiplus.hpp"
#include "Debug.hpp"

BitmapContextGdiplus::BitmapContextGdiplus (int width, int height) :
	bitmap (new Gdiplus::Bitmap (width, height)),
	graphics (new Gdiplus::Graphics (bitmap.get ())),
	width (width),
	height (height)
{
	InitGraphics ();
}

BitmapContextGdiplus::~BitmapContextGdiplus ()
{

}

void BitmapContextGdiplus::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	bitmap.reset (new Gdiplus::Bitmap (width, height));
	graphics.reset (new Gdiplus::Graphics (bitmap.get ()));
	InitGraphics ();
}

double BitmapContextGdiplus::GetWidth () const
{
	return width;
}

double BitmapContextGdiplus::GetHeight () const
{
	return height;
}

void BitmapContextGdiplus::BeginDraw ()
{

}

void BitmapContextGdiplus::EndDraw ()
{

}

void BitmapContextGdiplus::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	Gdiplus::Pen gdiPen (Gdiplus::Color (pen.GetColor ().GetR (), pen.GetColor ().GetG (), pen.GetColor ().GetB ()), (Gdiplus::REAL) pen.GetThickness ());
	graphics->DrawLine (&gdiPen, CreatePoint (beg), CreatePoint (end));
}

void BitmapContextGdiplus::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	Gdiplus::Pen gdiPen (Gdiplus::Color (pen.GetColor ().GetR (), pen.GetColor ().GetG (), pen.GetColor ().GetB ()), (Gdiplus::REAL) pen.GetThickness ());
	graphics->DrawBezier (&gdiPen, CreatePoint (p1), CreatePoint (p2), CreatePoint (p3), CreatePoint (p4));
}

void BitmapContextGdiplus::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	Gdiplus::Pen gdiPen (Gdiplus::Color (pen.GetColor ().GetR (), pen.GetColor ().GetG (), pen.GetColor ().GetB ()), (Gdiplus::REAL) pen.GetThickness ());
	Gdiplus::Rect gdiRect = CreateRect (rect);
	graphics->DrawRectangle (&gdiPen, gdiRect);
}

void BitmapContextGdiplus::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	Gdiplus::SolidBrush brush (Gdiplus::Color (color.GetR (), color.GetG (), color.GetB ()));
	Gdiplus::Rect gdiRect = CreateRect (rect);
	graphics->FillRectangle (&brush, gdiRect);
}

void BitmapContextGdiplus::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	Gdiplus::Pen gdiPen (Gdiplus::Color (pen.GetColor ().GetR (), pen.GetColor ().GetG (), pen.GetColor ().GetB ()), (Gdiplus::REAL) pen.GetThickness ());
	Gdiplus::Rect gdiRect = CreateRect (rect);
	graphics->DrawEllipse (&gdiPen, gdiRect);
}

void BitmapContextGdiplus::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	Gdiplus::SolidBrush brush (Gdiplus::Color (color.GetR (), color.GetG (), color.GetB ()));
	Gdiplus::Rect gdiRect = CreateRect (rect);
	graphics->FillEllipse (&brush, gdiRect);
}

void BitmapContextGdiplus::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& color)
{
	Gdiplus::Font gdiFont (font.GetFamily ().c_str (), (Gdiplus::REAL) font.GetSize ());
	Gdiplus::SolidBrush brush (Gdiplus::Color (color.GetR (), color.GetG (), color.GetB ()));
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

NUIE::Size BitmapContextGdiplus::MeasureText (const NUIE::Font& font, const std::wstring & text)
{
	Gdiplus::Font gdiFont (font.GetFamily ().c_str (), (Gdiplus::REAL) font.GetSize ());
	Gdiplus::RectF textRect (0, 0, 0, 0);

	if (graphics->MeasureString (text.c_str (), (int) text.length (), &gdiFont, textRect, NULL, &textRect) != Gdiplus::Status::Ok) {
		DBGBREAK ();
		return NUIE::Size (100, 20);
	}

	return NUIE::Size (textRect.Width, textRect.Height);
}

void BitmapContextGdiplus::Setup (HWND hwnd)
{

}

void BitmapContextGdiplus::DrawToHDC (HDC hdc)
{
	HDC memoryDC = CreateCompatibleDC (hdc);
	HBITMAP memoryBitmap = CreateCompatibleBitmap (hdc, width, height);

	HANDLE oldHandle = SelectObject (memoryDC, memoryBitmap);
	Gdiplus::Graphics targetGraphics (memoryDC);
	targetGraphics.DrawImage (bitmap.get (), 0, 0, width, height);
	targetGraphics.ReleaseHDC (memoryDC);
	BitBlt (hdc, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);
	SelectObject (memoryDC, oldHandle);

	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);
}

Gdiplus::Point BitmapContextGdiplus::CreatePoint (const NUIE::Point& point) const
{
	Gdiplus::Point gdiPoint ((int) std::ceil (point.GetX ()), (int) std::ceil (point.GetY ()));
	return gdiPoint;
}

Gdiplus::Rect BitmapContextGdiplus::CreateRect (const NUIE::Rect& rect) const
{
	Gdiplus::Rect gdiRect ((int) std::ceil (rect.GetX ()), (int) std::ceil (rect.GetY ()), (int) std::ceil (rect.GetWidth ()), (int) std::ceil (rect.GetHeight ()));
	return gdiRect;
}

Gdiplus::RectF BitmapContextGdiplus::CreateRectF (const NUIE::Rect& rect) const
{
	Gdiplus::RectF gdiRect ((float) rect.GetX (), (float) rect.GetY (), (float) rect.GetWidth (), (float) rect.GetHeight ());
	return gdiRect;
}

void BitmapContextGdiplus::InitGraphics ()
{
	graphics->SetSmoothingMode (Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
}
