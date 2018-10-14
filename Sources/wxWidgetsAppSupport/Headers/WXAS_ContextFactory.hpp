#ifndef CONTEXTFACTORY_HPP
#define CONTEXTFACTORY_HPP

#include "NUIE_DrawingContext.hpp"

#include <memory>
#include <wx/wx.h>

namespace WXAS
{

void*											GetNativeHandle (wxPanel* panel);
std::unique_ptr<NUIE::NativeDrawingContext>		CreateNativeDrawingContext ();

}

#endif
