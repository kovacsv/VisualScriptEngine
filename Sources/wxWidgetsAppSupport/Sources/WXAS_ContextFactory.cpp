#include "WXAS_ContextFactory.hpp"
#include "WXAS_wxDrawingContext.hpp"

#if defined(_WIN32)
#include "WAS_BitmapContextGdi.hpp"
#endif

//#define FORCE_USE_WX_CONTEXT

#if defined(FORCE_USE_WX_CONTEXT) || !defined(_WIN32)
#define USE_WX_CONTEXT
#endif

namespace WXAS
{

void* GetNativeHandle (wxPanel* panel)
{
#if defined(USE_WX_CONTEXT)
	return panel;
#else
	return panel->GetHandle ();
#endif
}

std::unique_ptr<NUIE::NativeDrawingContext> CreateNativeDrawingContext ()
{
#if defined(USE_WX_CONTEXT)
	return std::unique_ptr<NUIE::NativeDrawingContext> (new wxDrawingContext ());
#else
	return std::unique_ptr<NUIE::NativeDrawingContext> (new WAS::BitmapContextGdi ());
#endif
}

}
