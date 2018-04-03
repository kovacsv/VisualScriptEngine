#include "DrawingControl.hpp"
#include "ContextDecorators.hpp"
#include "wxDrawingContext.hpp"

DrawingControl::DrawingControl (wxWindow *parent, const std::shared_ptr<ResultImage>& resultImage) :
	wxPanel (parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
	captureHandler (this),
	resultImage (resultImage),
	drawingContext (new wxDrawingContext ()),
	viewBox (NUIE::Point (0.0, 0.0), 1.0),
	lastMousePos (nullptr)
{
	drawingContext->Init (this);
}

void DrawingControl::OnPaint (wxPaintEvent& evt)
{
	drawingContext->FillRect (NUIE::Rect (-10, -10, drawingContext->GetWidth () + 20, drawingContext->GetHeight () + 20), NUIE::Color (255, 255, 255));
	NUIE::ViewBoxContextDecorator viewBoxDecorator (*drawingContext, viewBox);
	resultImage->Draw (viewBoxDecorator);
	resultImage->Validate ();
	drawingContext->BlitToWindow (this);
}

void DrawingControl::OnMouseCaptureLost (wxMouseCaptureLostEvent& evt)
{
	captureHandler.OnCaptureLost ();
}

void DrawingControl::OnResize (wxSizeEvent& evt)
{
	wxSize size = evt.GetSize ();
	drawingContext->Resize (size.GetWidth (), size.GetHeight ());
}

void DrawingControl::OnMiddleButtonDown (wxMouseEvent& evt)
{
	captureHandler.OnMouseDown ();
	lastMousePos.reset (new NUIE::Point (evt.GetX (), evt.GetY ()));
}

void DrawingControl::OnMiddleButtonUp (wxMouseEvent& evt)
{
	captureHandler.OnMouseUp ();
	lastMousePos.reset (nullptr);
}

void DrawingControl::OnMouseMove (wxMouseEvent& evt)
{
	if (lastMousePos != nullptr) {
		NUIE::Point diff = NUIE::Point (evt.GetX (), evt.GetY ()) - *lastMousePos;
		viewBox.SetOffset (viewBox.GetOffset () + diff);
		lastMousePos->Set (evt.GetX (), evt.GetY ());
		RedrawImage ();
	}
}

void DrawingControl::OnMouseWheel (wxMouseEvent& evt)
{
	double scaleRatio = (evt.GetWheelRotation () > 0 ? 1.1 : 0.9);
	viewBox.SetScale (viewBox.GetScale () * scaleRatio, NUIE::Point (evt.GetX (), evt.GetY ()));
	RedrawImage ();
}

void DrawingControl::ClearImage ()
{
	resultImage->Clear ();
	RedrawImage ();
}

void DrawingControl::RedrawImage ()
{
	Refresh (false);
}

BEGIN_EVENT_TABLE(DrawingControl, wxPanel)
EVT_PAINT (DrawingControl::OnPaint)
EVT_SIZE (DrawingControl::OnResize)
EVT_MOUSE_CAPTURE_LOST (DrawingControl::OnMouseCaptureLost)
EVT_MIDDLE_DOWN (DrawingControl::OnMiddleButtonDown)
EVT_MIDDLE_UP (DrawingControl::OnMiddleButtonUp)
EVT_MOUSEWHEEL (DrawingControl::OnMouseWheel)
EVT_MOTION (DrawingControl::OnMouseMove)
END_EVENT_TABLE ()
