#include "MAS_NodeEditorNSViewControl.hpp"
#include "NE_Debug.hpp"

#import <Cocoa/Cocoa.h>

@interface CocoaNSViewControl : NSView
{
@public NUIE::NodeEditor* nodeEditor;
}
@end

@implementation CocoaNSViewControl

- (id) init
{
	self = [super init];
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

- (void) mouseDown:(NSEvent *) event
{
	#pragma unused (event)
	NSAlert* alert = [[[NSAlert alloc] init] autorelease];
	[alert runModal];
}

@end

namespace MAS
{

NodeEditorNSViewControl::NodeEditorNSViewControl () :
	NodeEditorNSViewControl (NUIE::NativeDrawingContextPtr (nullptr)) // TODO
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
	// TODO
	//DBGASSERT (nodeEditor != nullptr);

	NSRect viewRect = NSMakeRect (x, y, width, height);
	subView = [[[CocoaNSViewControl alloc] initWithFrame:viewRect] autorelease];
	[((CocoaNSViewControl*) subView) setNodeEditor:nullptr];
	[((NSView*) nativeParentHandle) addSubview:((NSView*) subView)];
	
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
}

void NodeEditorNSViewControl::Invalidate ()
{
	// TODO
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
