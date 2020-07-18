#include "MAS_NodeEditorNSViewControl.hpp"
#include "MAS_NSViewContext.hpp"
#include "NE_Debug.hpp"

#import <Cocoa/Cocoa.h>

static NUIE::ModifierKeys GetKeysFromEvent (const NSEvent* event)
{
	NUIE::ModifierKeys keys;
	NSEventModifierFlags flags = [event modifierFlags];
	if (flags & NSEventModifierFlagControl) {
		keys.Insert (NUIE::ModifierKeyCode::Control);
	} else if (flags & NSEventModifierFlagShift) {
		keys.Insert (NUIE::ModifierKeyCode::Shift);
	}
	return keys;
}

static NUIE::Point GetPositionFromEvent (const NSEvent* event)
{
	NSPoint position = [event locationInWindow];
	return NUIE::Point (position.x, position.y);
}

@interface CocoaNSViewControl : NSView
{
@private NUIE::NodeEditor* nodeEditor;
@private NSTrackingArea* trackingArea;
}
@end

@implementation CocoaNSViewControl

- (id) initWithFrame : (NSRect) frame
{
	self = [super initWithFrame:frame];
	if (self) {
		nodeEditor = nil;
		trackingArea = [[NSTrackingArea alloc]
			initWithRect : frame
			options : (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInActiveApp)
			owner : self
			userInfo : nil
		];
		[self addTrackingArea:trackingArea];
	}
	return self;
}

- (void)updateTrackingAreas
{
	[self removeTrackingArea:trackingArea];
	trackingArea = [[NSTrackingArea alloc]
		initWithRect : [self frame]
		options : (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInActiveApp)
		owner : self
		userInfo : nil
	];
	[self addTrackingArea:trackingArea];
}

-(void) setNodeEditor : (NUIE::NodeEditor*) newNodeEditor
{
	nodeEditor = newNodeEditor;
}

- (void) drawRect : (NSRect) dirtyRect
{
	nodeEditor->Draw ();
	
	[[NSColor whiteColor] set];
	NSRectFill (dirtyRect);
	
	[[NSColor redColor] set];
	
	// The three vertices of a triangle
	NSPoint p1 = NSMakePoint(100, 100);
	NSPoint p2 = NSMakePoint(200, 300);
	NSPoint p3 = NSMakePoint(300, 100);
	
	// Control points
	NSPoint c1 = NSMakePoint(200, 200);
	NSPoint c2 = NSMakePoint(200, 0);
	
	// Constructing the path for the triangle
	NSBezierPath *bp = [NSBezierPath bezierPath];
	[bp moveToPoint:p1];
	[bp lineToPoint:p2];
	[bp lineToPoint:p3];
	[bp curveToPoint:p1 controlPoint1:c1 controlPoint2:c2];
	[bp closePath];
	[bp stroke];
}

- (void) mouseDown : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (event);
	nodeEditor->OnMouseDown (GetKeysFromEvent (event), NUIE::MouseButton::Left, position.GetX (), position.GetY ());
}

- (void) mouseUp : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (event);
	nodeEditor->OnMouseUp (GetKeysFromEvent (event), NUIE::MouseButton::Left, position.GetX (), position.GetY ());
	
	NSAlert* alert = [[[NSAlert alloc] init] autorelease];
	[alert runModal];
}

- (void) rightMouseDown : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (event);
	nodeEditor->OnMouseDown (GetKeysFromEvent (event), NUIE::MouseButton::Right, position.GetX (), position.GetY ());
}

- (void) rightMouseUp : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (event);
	nodeEditor->OnMouseUp (GetKeysFromEvent (event), NUIE::MouseButton::Right, position.GetX (), position.GetY ());
	
	NSAlert* alert = [[[NSAlert alloc] init] autorelease];
	[alert runModal];
}

- (void) otherMouseDown : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (event);
	nodeEditor->OnMouseDown (GetKeysFromEvent (event), NUIE::MouseButton::Middle, position.GetX (), position.GetY ());
}

- (void) otherMouseUp : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (event);
	nodeEditor->OnMouseUp (GetKeysFromEvent (event), NUIE::MouseButton::Middle, position.GetX (), position.GetY ());
	
	NSAlert* alert = [[[NSAlert alloc] init] autorelease];
	[alert runModal];
}

- (void) mouseMoved : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (event);
	nodeEditor->OnMouseMove (GetKeysFromEvent (event), position.GetX (), position.GetY ());
}

@end

namespace MAS
{

NodeEditorNSViewControl::NodeEditorNSViewControl () :
	NodeEditorNSViewControl (NUIE::NativeDrawingContextPtr (new NSViewContext ()))
{

}

NodeEditorNSViewControl::NodeEditorNSViewControl (const NUIE::NativeDrawingContextPtr& nativeContext) :
	NativeNodeEditorControl (),
	nodeEditor (nullptr),
	nativeContext (nativeContext),
	subView (nullptr)
{

}

NodeEditorNSViewControl::~NodeEditorNSViewControl ()
{

}

bool NodeEditorNSViewControl::Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height)
{
	nodeEditor = nodeEditorPtr;
	DBGASSERT (nodeEditor != nullptr);

	NSRect viewRect = NSMakeRect (x, y, width, height);
	subView = [[[CocoaNSViewControl alloc] initWithFrame:viewRect] autorelease];
	[((CocoaNSViewControl*) subView) setNodeEditor:nodeEditor];
	[((NSView*) nativeParentHandle) addSubview:((NSView*) subView)];
	
	// TODO
	// nativeContext->Init (subView);
	return true;
}

void* NodeEditorNSViewControl::GetEditorNativeHandle () const
{
	return subView;
}

bool NodeEditorNSViewControl::IsEditorFocused () const
{
	// TODO
	return false;
}

void NodeEditorNSViewControl::Resize (int x, int y, int width, int height)
{
	NSRect viewRect = NSMakeRect (x, y, width, height);
	[((NSView*) subView) setFrame:viewRect];
	if (nodeEditor != nullptr) {
		nodeEditor->OnResize (width, height);
	}
}

void NodeEditorNSViewControl::Invalidate ()
{
	[((NSView*) subView) setNeedsDisplay:YES];
}

void NodeEditorNSViewControl::Draw ()
{
	// TODO
}

NUIE::DrawingContext& NodeEditorNSViewControl::GetDrawingContext ()
{
	return *nativeContext;
}

NUIE::NodeEditor* NodeEditorNSViewControl::GetNodeEditor ()
{
	return nodeEditor;
}

}
