#include "Application.hpp"

#import <Cocoa/Cocoa.h>

@interface WindowController : NSWindowController<NSWindowDelegate>
{
@private AppNodeUIEnvironment* uiEnvironment;
}
@end

@implementation WindowController

- (id) init
{
	self = [super init];
	if (self) {
		uiEnvironment = nil;
	}
	return self;
}

-(void) setUIEnvironment : (AppNodeUIEnvironment*) newUIEnvironment
{
	uiEnvironment = newUIEnvironment;
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
	NSRect windowRect = [[self window] frame];
	uiEnvironment->Resize (0, 0, windowRect.size.width, windowRect.size.height);
}

@end

AppNodeUIEnvironment::AppNodeUIEnvironment () :
	stringConverter (NE::GetDefaultStringConverter ()),
	skinParams (NUIE::GetDefaultSkinParams ()),
	evaluationEnv (NE::EmptyEvaluationEnv),
	eventHandler (),
	clipboardHandler (),
	nodeEditorControl ()
{
	
}

AppNodeUIEnvironment::~AppNodeUIEnvironment ()
{

}

void AppNodeUIEnvironment::Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height)
{
	nodeEditorControl.Init (nodeEditorPtr, nativeParentHandle, x, y, width, height);
}

void AppNodeUIEnvironment::Resize (int x, int y, int width, int height)
{
	nodeEditorControl.Resize (x, y, width, height);
}

const NE::StringConverter &AppNodeUIEnvironment::GetStringConverter ()
{
	return stringConverter;
}

const NUIE::SkinParams &AppNodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

NUIE::DrawingContext &AppNodeUIEnvironment::GetDrawingContext( )
{
	return nodeEditorControl.GetDrawingContext ();
}

double AppNodeUIEnvironment::GetWindowScale ()
{
	return 1.0;
}

NE::EvaluationEnv &AppNodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void AppNodeUIEnvironment::OnEvaluationBegin ()
{

}

void AppNodeUIEnvironment::OnEvaluationEnd ()
{

}

void AppNodeUIEnvironment::OnValuesRecalculated ()
{

}

void AppNodeUIEnvironment::OnRedrawRequested ()
{
	nodeEditorControl.Invalidate ();
}

NUIE::EventHandler &AppNodeUIEnvironment::GetEventHandler ()
{
	return eventHandler;
}

NUIE::ClipboardHandler &AppNodeUIEnvironment::GetClipboardHandler ()
{
	return clipboardHandler;
}

double AppNodeUIEnvironment::GetMouseMoveMinOffset ()
{
	return 2.0;
}

Application::Application () :
	uiEnvironment (),
	nodeEditor (uiEnvironment)
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
	[myWindowController setUIEnvironment:&uiEnvironment];

	[myWindow setContentView:contentView];
	[myWindow setDelegate:myWindowController];

	uiEnvironment.Init (&nodeEditor, contentView, 0, 0, windowRect.size.width, windowRect.size.height);
	[myWindow makeKeyAndOrderFront:nil];

	[NSApp run];

	[NSApp release];
	[pool release];
}
