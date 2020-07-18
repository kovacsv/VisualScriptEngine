#include "Application.hpp"

#import <Cocoa/Cocoa.h>

@interface WindowController : NSWindowController<NSWindowDelegate>
{
@private MAS::NodeEditorNSViewControl* editorControl;
}
@end

@implementation WindowController

- (id) init
{
	self = [super init];
	if (self) {
		editorControl = nil;
	}
	return self;
}

-(void) setNodeEditorControl : (MAS::NodeEditorNSViewControl*) newEditorControl
{
	editorControl = newEditorControl;
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

Application::Application () :
	editorControl ()
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
	NSView* contentView = [[[NSView alloc] initWithFrame:windowRect] autorelease];

	[myWindowController setWindow:myWindow];
	[myWindowController setNodeEditorControl:&editorControl];

	[myWindow setContentView:contentView];
	[myWindow setDelegate:myWindowController];

	editorControl.Init (nullptr, contentView, 0, 0, windowRect.size.width, windowRect.size.height);
	[myWindow makeKeyAndOrderFront:nil];

	[NSApp run];

	[NSApp release];
	[pool release];
}
