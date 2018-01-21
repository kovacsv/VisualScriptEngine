#include "ControlUtilities.hpp"

MouseCaptureHandler::MouseCaptureHandler (wxPanel* panel) :
	panel (panel),
	counter (0)
{

}

void MouseCaptureHandler::OnMouseDown ()
{
	panel->CaptureMouse ();
	counter++;
}

void MouseCaptureHandler::OnMouseUp ()
{
	counter--;
	if (counter == 0) {
		panel->ReleaseMouse ();
	}
	counter = 0;
}

void MouseCaptureHandler::OnCaptureLost ()
{
	counter = 0;
}
