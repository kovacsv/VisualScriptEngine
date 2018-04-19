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
	std::unordered_set<NUIE::KeyCode> keys;
	if (evt.ControlDown ()) {
		keys.insert (NUIE::KeyCode::Control);
	}
	if (evt.ShiftDown ()) {
		keys.insert (NUIE::KeyCode::Shift);
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
				return NUIE::Key (NUIE::Key::SpecialKey::Delete);
		}
		return NUIE::InvalidKey;
	}
	return NUIE::Key (unicodeKey);
}
