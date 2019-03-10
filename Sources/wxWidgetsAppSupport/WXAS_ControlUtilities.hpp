#ifndef WXAS_CONTROLUTILITIES_HPP
#define WXAS_CONTROLUTILITIES_HPP

#include "NUIE_InputEventHandler.hpp"
#include "NUIE_MenuCommands.hpp"
#include <wx/wx.h>

namespace WXAS
{

// On double click event the mouse down and up events are not in pair
class MouseCaptureHandler
{
public:
	MouseCaptureHandler (wxPanel* panel);

	void		OnMouseDown ();
	void		OnMouseUp ();
	void		OnCaptureLost ();

private:
	wxPanel*	panel;
	int			counter;
};

class BusyCursorGuard
{
public:
	BusyCursorGuard ();
	~BusyCursorGuard ();
};

NUIE::ModifierKeys		GetModiferKeysFromEvent (wxKeyboardState& evt);
NUIE::Key				GetKeyFromEvent (wxKeyEvent& evt);

void					SetTextControlValidator (wxTextCtrl* textCtrl, const std::wstring& validChars);
NUIE::MenuCommandPtr	SelectCommandFromContextMenu (wxPanel* panel, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands);

}

#endif
