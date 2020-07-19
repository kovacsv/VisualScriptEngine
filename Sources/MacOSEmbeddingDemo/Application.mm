#include "Application.hpp"
#include "MAS_CocoaAppUtils.hpp"

// TODO: temporary
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"

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
	NSRect clientRect = [[[self window] contentView] frame];
	uiEnvironment->Resize (0, 0, clientRect.size.width, clientRect.size.height);
}

@end



AppEventHandler::AppEventHandler () :
	nsView (nil)
{
	
}

AppEventHandler::~AppEventHandler ()
{
	
}

void AppEventHandler::Init (void* nsViewPtr)
{
	nsView = nsViewPtr;
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, commands);
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::UINodePtr&, const NUIE::MenuCommandStructure& commands)
{
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, commands);
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, commands);

}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::UIInputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, commands);

}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::UINodeGroupPtr&, const NUIE::MenuCommandStructure& commands)
{
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, commands);

}

void AppEventHandler::OnDoubleClick(NUIE::MouseButton mouseButton, const NUIE::Point& position)
{
	#pragma unused (mouseButton)
	#pragma unused (position)
}

bool AppEventHandler::OnParameterSettings(NUIE::ParameterInterfacePtr paramAccessor, const NUIE::UINodePtr& uiNode)
{
	#pragma unused (paramAccessor)
	#pragma unused (uiNode)
	return false;
}

bool AppEventHandler::OnParameterSettings(NUIE::ParameterInterfacePtr paramAccessor, const NUIE::UINodeGroupPtr& uiGroup)
{
	#pragma unused (paramAccessor)
	#pragma unused (uiGroup)
	return false;
}

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
	eventHandler.Init (nativeParentHandle);
}

void AppNodeUIEnvironment::Resize (int x, int y, int width, int height)
{
	nodeEditorControl.Resize (x, y, width, height);
}

const NE::StringConverter& AppNodeUIEnvironment::GetStringConverter ()
{
	return stringConverter;
}

const NUIE::SkinParams& AppNodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

NUIE::DrawingContext& AppNodeUIEnvironment::GetDrawingContext( )
{
	return nodeEditorControl.GetDrawingContext ();
}

double AppNodeUIEnvironment::GetWindowScale ()
{
	return 1.0;
}

NE::EvaluationEnv& AppNodeUIEnvironment::GetEvaluationEnv ()
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

NUIE::EventHandler& AppNodeUIEnvironment::GetEventHandler ()
{
	return eventHandler;
}

NUIE::ClipboardHandler& AppNodeUIEnvironment::GetClipboardHandler ()
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

	// TODO: temporary
	nodeEditor.AddNode(NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), NUIE::Point (100, 100), 5, 1)));
	nodeEditor.AddNode(NUIE::UINodePtr (new BI::ViewerNode (NE::LocString (L"Viewer"), NUIE::Point (300, 200))));

	uiEnvironment.Init (&nodeEditor, contentView, 0, 0, windowRect.size.width, windowRect.size.height);
	[myWindow makeKeyAndOrderFront:nil];

	[NSApp run];

	[NSApp release];
	[pool release];
}
