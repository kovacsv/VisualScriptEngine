#include "DrawingControl.hpp"

DrawingControl::DrawingControl (const std::shared_ptr<ResultImage>& resultImage) :
	UI::CustomControl (),
	bitmapContext (),
	resultImage (resultImage)
{

}

void DrawingControl::OnCreate (HWND hwnd)
{
	bitmapContext.Init (hwnd);
}

void DrawingControl::OnPaint (HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);
	bitmapContext.DrawToHDC (hdc);
	EndPaint (hwnd, &ps);
}

void DrawingControl::OnResize (HWND hwnd, int newWidth, int newHeight)
{
	bitmapContext.Resize (newWidth, newHeight);
	resultImage->Invalidate ();
	Invalidate ();
}

void DrawingControl::Clear ()
{
	resultImage->Clear ();
	Invalidate ();
}

void DrawingControl::Invalidate ()
{
	if (resultImage->IsModified ()) {
		bitmapContext.FillRect (NUIE::Rect (-10, -10, bitmapContext.GetWidth () + 20, bitmapContext.GetHeight () + 20), NUIE::Color (255, 255, 255));
		//NUIE::ViewBoxContextDecorator centerDecorator (bitmapContext, NUIE::ViewBox (NUIE::Point (bitmapContext.GetWidth () / 2.0, bitmapContext.GetHeight () / 2.0), 1.0));
		resultImage->Draw (bitmapContext);
		InvalidateRect (windowHandle, NULL, FALSE);
		resultImage->Validate ();
	}
}
