#ifndef WINDOWSAPPUTILITIES_HPP
#define WINDOWSAPPUTILITIES_HPP

#include "Geometry.hpp"
#include "CommandStructure.hpp"
#include "NodeEditor.hpp"

#include <windows.h>

namespace UI
{

class SetCaptureHandler
{
public:
	SetCaptureHandler ();

	void	HandleMouseDown (HWND hwnd);
	void	HandleMouseUp ();

private:
	int counter;
};

NUIE::CommandPtr	SelectCommandFromContextMenu (HWND hwnd, const NUIE::Point& position, const NUIE::CommandStructure& commands);
bool				ForwardInputEventToNodeEditor (NUIE::NodeEditor& nodeEditor, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

}

#endif
