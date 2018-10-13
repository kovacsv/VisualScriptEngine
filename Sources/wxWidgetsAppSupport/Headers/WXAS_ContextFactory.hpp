#ifndef CONTEXTFACTORY_HPP
#define CONTEXTFACTORY_HPP

#include "NUIE_DrawingContext.hpp"

#include <memory>
#include <wx/wx.h>

void*											GetNativeHandle (wxPanel* panel);
std::shared_ptr<NUIE::NativeDrawingContext>		CreateNativeDrawingContext ();

#endif
