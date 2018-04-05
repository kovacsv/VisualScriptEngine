#ifndef WINDOWSAPPUTILS_HPP
#define WINDOWSAPPUTILS_HPP

#include <windows.h>

#include "NUIE_Geometry.hpp"
#include "NUIE_CommandStructure.hpp"
#include "NUIE_MouseEventHandler.hpp"

namespace WAS
{

NUIE::KeySet		GetKeysFromEvent (WPARAM wParam);
NUIE::CommandPtr	SelectCommandFromContextMenu (HWND hwnd, const NUIE::Point& position, const NUIE::CommandStructure& commands);

}

#endif
