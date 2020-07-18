#include "Application.hpp"

#import <Cocoa/Cocoa.h>

@interface WindowController : NSWindowController<NSWindowDelegate>
{

}
@end

@implementation WindowController

- (id) init
{
	self = [super init];
	if (self) {
	}
	return self;
}

-(void) windowDidLoad
{
    
}

-(void) windowWillClose : (NSNotification*) notification
{
	#pragma unused (notification)
	[NSApp terminate:self];
}

-(void) windowDidResize:(NSNotification *) notification
{
	#pragma unused (notification)
}

@end

Application::Application ()
{
    
}

void Application::Run ()
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSApp = [NSApplication sharedApplication];

	NSRect windowRect = NSMakeRect (0.0f, 0.0f, 600.0f, 400.0f);
	NSWindow* myWindow = [
	[NSWindow alloc]
		initWithContentRect:windowRect
		styleMask:
			NSWindowStyleMaskTitled |
			NSWindowStyleMaskClosable |
			NSWindowStyleMaskMiniaturizable |
			NSWindowStyleMaskResizable
		backing:NSBackingStoreBuffered
		defer:NO
	];

	[myWindow setTitle:@"VisualScriptEngine Embedding Demo"];

	WindowController* myWindowController = [[WindowController alloc] autorelease];
	[myWindowController setWindow:myWindow];
	[myWindow setDelegate:myWindowController];

	NSView* contentView = [[[NSView alloc] initWithFrame:windowRect] autorelease];
	[myWindow setContentView:contentView];

	[myWindow makeKeyAndOrderFront:nil];

	[NSApp run];

	[NSApp release];
	[pool release];
}
