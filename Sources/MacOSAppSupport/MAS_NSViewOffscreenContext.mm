#include "MAS_NSViewOffscreenContext.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

namespace MAS
{

NSViewOffscreenContext::NSViewOffscreenContext (Orientation orientation) :
	NSViewOffscreenContext (orientation, nullptr)
{

}

NSViewOffscreenContext::NSViewOffscreenContext (Orientation orientation, const NSImageLoaderPtr& imageLoader) :
	NSViewContextBase (imageLoader),
	orientation (orientation),
	image (nil)
{

}

NSViewOffscreenContext::~NSViewOffscreenContext ()
{
	@autoreleasepool {
		@try {
			if (image != nil) {
				[image release];
			}
		} @catch (NSException*) {
			
		}
	}
}

void NSViewOffscreenContext::Init (void* nativeHandle)
{
	NSView* nsView = (NSView*) nativeHandle;
	width = nsView.frame.size.width;
	height = nsView.frame.size.height;
	
	CGSize size = { (CGFloat) width, (CGFloat) height };
	image = [[NSImage alloc] initWithSize : size];
}

void NSViewOffscreenContext::BlitToWindow (void*)
{
	
}

void NSViewOffscreenContext::BlitToContext (void* cgContext)
{
	CGContextRef ctx = (CGContextRef) cgContext;
	if (ctx != nil) {
		if (orientation == Orientation::FlippedVertically) {
			CGContextTranslateCTM (ctx, 0.0, height);
			CGContextScaleCTM (ctx, 1.0, -1.0);
		}
		CGRect imgRect = CGRectMake (0, 0, width, height);
		CGImageRef cgImage = [image CGImageForProposedRect: &imgRect context: [NSGraphicsContext currentContext] hints: nil];
		CGContextDrawImage (ctx, imgRect, cgImage);
		if (orientation == Orientation::FlippedVertically) {
			CGContextScaleCTM (ctx, 1.0, -1.0);
			CGContextTranslateCTM (ctx, 0.0, -height);
		}
	}
}

void NSViewOffscreenContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	CGSize size = { (CGFloat) width, (CGFloat) height };
	image = [[NSImage alloc] initWithSize : size];
}

void NSViewOffscreenContext::BeginDraw ()
{
	@try {
		[image lockFocus];
	} @catch (NSException*) {
		
	}
}

void NSViewOffscreenContext::EndDraw ()
{
	@try {
		[image unlockFocus];
	} @catch (NSException*) {
		
	}
}

}
