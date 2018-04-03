#ifndef WINDOWSAPPUTILS_HPP
#define WINDOWSAPPUTILS_HPP

#include <windows.h>

#include "Geometry.hpp"
#include "CommandStructure.hpp"
#include "MouseEventHandler.hpp"

NUIE::KeySet		GetKeysFromEvent (WPARAM wParam);
NUIE::CommandPtr	SelectCommandFromContextMenu (HWND hwnd, const NUIE::Point& position, const NUIE::CommandStructure& commands);

#endif
