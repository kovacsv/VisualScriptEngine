#ifndef WINDOWSAPPUTILITIES_HPP
#define WINDOWSAPPUTILITIES_HPP

#include "CommandStructure.hpp"
#include "Geometry.hpp"

#include <windows.h>

namespace UI
{

NUIE::CommandPtr SelectCommandFromContextMenu (HWND hwnd, const NUIE::Point& position, const NUIE::CommandStructure& commands);

}

#endif
