#include "DrawingControl.hpp"
#include "ContextDecorators.hpp"
#include "BitmapContextGdiplus.hpp"

DrawingControl::DrawingControl (const std::shared_ptr<ResultImage>& resultImage) :
	UI::CustomControl (),
	resultImage (resultImage),
	drawingContext (new BitmapContextGdiplus ()),
	viewBox (NUIE::Point (0.0, 0.0), 1.0),
	lastMousePos (nullptr)
{

}

void DrawingControl::OnCreate (HWND hwnd)
{
	drawingContext->Init (hwnd);
}

void DrawingControl::OnPaint (HWND hwnd)
{
	drawingContext->FillRect (NUIE::Rect (-10, -10, drawingContext->GetWidth () + 20, drawingContext->GetHeight () + 20), NUIE::Color (255, 255, 255));
	NUIE::ViewBoxContextDecorator viewBoxDecorator (*drawingContext, viewBox);
	resultImage->Draw (viewBoxDecorator);
	resultImage->Validate ();

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);
	drawingContext->DrawToHDC (hdc);
	EndPaint (hwnd, &ps);
}

void DrawingControl::OnMouseDown (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{
	if (button == UI::MouseButton::Middle) {
		lastMousePos.reset (new NUIE::Point (x, y));
	}
}

void DrawingControl::OnMouseUp (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{
	if (button == UI::MouseButton::Middle) {
		lastMousePos.reset (nullptr);
	}
}

void DrawingControl::OnMouseMove (HWND hwnd, UI::Keys keys, int x, int y)
{
	if (lastMousePos != nullptr) {
		NUIE::Point diff = NUIE::Point (x, y) - *lastMousePos;
		viewBox.SetOffset (viewBox.GetOffset () + diff);
		lastMousePos->Set (x, y);
		RedrawImage ();
	}
}

void DrawingControl::OnMouseWheel (HWND hwnd, UI::Keys keys, int x, int y, int delta)
{
	double scaleRatio = (delta > 0 ? 1.1 : 0.9);
	viewBox.SetScale (viewBox.GetScale () * scaleRatio, NUIE::Point (x, y));
	RedrawImage ();
}

void DrawingControl::OnResize (HWND hwnd, int newWidth, int newHeight)
{
	drawingContext->Resize (newWidth, newHeight);
	RedrawImage ();
}

void DrawingControl::ClearImage ()
{
	resultImage->Clear ();
	RedrawImage ();
}

void DrawingControl::RedrawImage ()
{
	InvalidateRect (GetWindowHandle (), NULL, FALSE);
}
