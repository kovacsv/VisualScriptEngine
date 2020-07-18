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

static NUIE::Point GetPositionFromEvent (NSView* view, const NSEvent* event)
{
	NSPoint position = [view convertPoint:[event locationInWindow] fromView:nil];
	return NUIE::Point (position.x, view.frame.size.height - position.y);
}

@interface CocoaNSViewControl : NSView
{
@private NUIE::NodeEditor* nodeEditor;
}
@end

@implementation CocoaNSViewControl

- (BOOL) acceptsFirstResponder
{
	return YES;
}

- (id) initWithFrame : (NSRect) frame
{
	self = [super initWithFrame:frame];
	if (self) {
		nodeEditor = nil;
	}
	return self;
}

-(void) setNodeEditor : (NUIE::NodeEditor*) newNodeEditor
{
	nodeEditor = newNodeEditor;
}

- (void) drawRect : (NSRect) dirtyRect
{
	#pragma unused (dirtyRect)
	nodeEditor->Draw ();
	/*
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
	 */
}

- (void) mouseDown : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	nodeEditor->OnMouseDown (GetKeysFromEvent (event), NUIE::MouseButton::Left, position.GetX (), position.GetY ());
}

- (void) mouseUp : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	nodeEditor->OnMouseUp (GetKeysFromEvent (event), NUIE::MouseButton::Left, position.GetX (), position.GetY ());
}

- (void) rightMouseDown : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	nodeEditor->OnMouseDown (GetKeysFromEvent (event), NUIE::MouseButton::Right, position.GetX (), position.GetY ());
}

- (void) rightMouseUp : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	nodeEditor->OnMouseUp (GetKeysFromEvent (event), NUIE::MouseButton::Right, position.GetX (), position.GetY ());
}

- (void) otherMouseDown : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	nodeEditor->OnMouseDown (GetKeysFromEvent (event), NUIE::MouseButton::Middle, position.GetX (), position.GetY ());
}

- (void) otherMouseUp : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	nodeEditor->OnMouseUp (GetKeysFromEvent (event), NUIE::MouseButton::Middle, position.GetX (), position.GetY ());
}

- (void) mouseDragged : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	nodeEditor->OnMouseMove (GetKeysFromEvent (event), position.GetX (), position.GetY ());
}

- (void) rightMouseDragged : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	nodeEditor->OnMouseMove (GetKeysFromEvent (event), position.GetX (), position.GetY ());
}

- (void) otherMouseDragged : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	nodeEditor->OnMouseMove (GetKeysFromEvent (event), position.GetX (), position.GetY ());
}

- (void) scrollWheel : (NSEvent *) event
{
	NUIE::Point position = GetPositionFromEvent (self, event);
	NUIE::MouseWheelRotation rotation = NUIE::MouseWheelRotation::Forward;
	if ([event deltaX] + [event deltaY] < 0) {
		rotation = NUIE::MouseWheelRotation::Backward;
	}
	nodeEditor->OnMouseWheel(GetKeysFromEvent(event), rotation, position.GetX (), position.GetY ());
}

- (void) keyDown : (NSEvent *) event
{
	#pragma unused (event)
	// TODO
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
	
	nativeContext->Init (subView);
	nativeContext->Resize (width, height);
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
