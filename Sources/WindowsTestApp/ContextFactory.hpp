#ifndef CONTEXTFACTORY_HPP
#define CONTEXTFACTORY_HPP

#include "DrawingContext.hpp"

#include <memory>
#include <wx/wx.h>

void*											GetNativeHandle (wxPanel* panel);
std::shared_ptr<NUIE::NativeDrawingContext>		CreateNativeDrawingContext ();

#endif
