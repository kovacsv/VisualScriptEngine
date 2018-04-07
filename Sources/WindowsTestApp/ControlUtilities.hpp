#ifndef CONTROLUTILITIES_HPP
#define CONTROLUTILITIES_HPP

#include "NUIE_MouseEventHandler.hpp"
#include <wx/wx.h>

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

NUIE::ModifierKeys GetKeysFromEvent (wxMouseEvent& evt);

#endif
