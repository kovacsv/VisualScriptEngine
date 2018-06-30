#ifndef WINDOWSAPPUTILS_HPP
#define WINDOWSAPPUTILS_HPP

#include "WAS_IncludeWindowsHeaders.hpp"
#include "NUIE_Geometry.hpp"
#include "NUIE_CommandStructure.hpp"
#include "NUIE_InputEventHandler.hpp"

namespace WAS
{

NUIE::ModifierKeys	GetModiferKeysFromEvent (WPARAM wParam);
NUIE::UICommandPtr	SelectCommandFromContextMenu (HWND hwnd, const NUIE::Point& position, const NUIE::UICommandStructure& commands);

}

#endif
