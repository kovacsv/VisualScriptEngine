#include "MAS_NSViewContext.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

namespace MAS
{

NSViewContext::NSViewContext () :
	NSViewContext (nullptr)
{

}

NSViewContext::NSViewContext (const NSImageLoaderPtr& imageLoader) :
	NSViewContextBase (imageLoader),
	nsView (nil)
{

}

NSViewContext::~NSViewContext ()
{

}

void NSViewContext::Init (void* nativeHandle)
{
	nsView = (NSView*) nativeHandle;
	width = nsView.frame.size.width;
	height = nsView.frame.size.height;
}

void NSViewContext::BlitToWindow (void*)
{

}

void NSViewContext::BlitToContext (void*)
{

}

void NSViewContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}

}
