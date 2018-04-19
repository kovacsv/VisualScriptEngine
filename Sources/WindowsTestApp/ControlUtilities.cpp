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
	if (counter < 0) {
		counter = 0;
	}
}

void MouseCaptureHandler::OnCaptureLost ()
{
	counter--;
}

NUIE::ModifierKeys GetModiferKeysFromEvent (wxKeyboardState& evt)
{
	std::unordered_set<NUIE::ModifierKeyCode> keys;
	if (evt.ControlDown ()) {
		keys.insert (NUIE::ModifierKeyCode::Control);
	}
	if (evt.ShiftDown ()) {
		keys.insert (NUIE::ModifierKeyCode::Shift);
	}
	return NUIE::ModifierKeys (keys);
}

NUIE::Key GetKeyFromEvent (wxKeyEvent& evt)
{
	wxChar unicodeKey = evt.GetUnicodeKey ();
	if (unicodeKey < WXK_START) {
		int key = evt.GetKeyCode ();
		switch (key) {
			case WXK_DELETE:
				return NUIE::Key (NUIE::SpecialKeyCode::Delete);
		}
		return NUIE::InvalidKey;
	}
	return NUIE::Key (unicodeKey);
}
