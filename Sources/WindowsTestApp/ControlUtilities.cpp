#include "ControlUtilities.hpp"

MouseCaptureHandler::MouseCaptureHandler (wxPanel* panel) :
	panel (panel),
	counter (0)
{

}

void MouseCaptureHandler::OnMouseDown ()
{
	if (counter == 0) {
		panel->CaptureMouse ();
	}
	counter++;
}

void MouseCaptureHandler::OnMouseUp ()
{
	counter--;
	if (counter == 0) {
		panel->ReleaseMouse ();
	}
}

void MouseCaptureHandler::OnCaptureLost ()
{
	counter = 0;
}

static NUIE::KeySet GetKeysFromEvent (WPARAM wParam)
{

}

NUIE::KeySet GetKeysFromEvent (wxMouseEvent& evt)
{
	std::unordered_set<NUIE::KeyCode> keys;
	if (evt.ControlDown ()) {
		keys.insert (NUIE::KeyCode::Control);
	}
	if (evt.ShiftDown ()) {
		keys.insert (NUIE::KeyCode::Shift);
	}
	return NUIE::KeySet (keys);
}
