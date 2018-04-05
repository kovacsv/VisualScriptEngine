#include "ContextFactory.hpp"
#include "wxDrawingContext.hpp"

#if defined(_WIN32)
#include "WAS_BitmapContextGdi.hpp"
#endif

//#define FORCE_USE_WX_CONTEXT

#if defined(FORCE_USE_WX_CONTEXT) || !defined(_WIN32)
#define USE_WX_CONTEXT
#endif

void* GetNativeHandle (wxPanel* panel)
{
#if defined(USE_WX_CONTEXT)
	return panel;
#else
	return panel->GetHandle ();
#endif
}

std::shared_ptr<NUIE::NativeDrawingContext> CreateNativeDrawingContext ()
{
#if defined(USE_WX_CONTEXT)
	return std::shared_ptr<NUIE::NativeDrawingContext> (new wxDrawingContext ());
#else
	return std::shared_ptr<NUIE::NativeDrawingContext> (new BitmapContextGdi ());
#endif
}
