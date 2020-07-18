#include "MAS_NodeEditorNSViewControl.hpp"
#include "MAS_MacOSAppUtils.hpp"
#include "MAS_NSViewContext.hpp"
#include "NE_Debug.hpp"

#import <Cocoa/Cocoa.h>

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
}

- (void) mouseDown : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	nodeEditor->OnMouseDown (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Left, position.GetX (), position.GetY ());
}

- (void) mouseUp : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	nodeEditor->OnMouseUp (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Left, position.GetX (), position.GetY ());
}

- (void) rightMouseDown : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	nodeEditor->OnMouseDown (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Right, position.GetX (), position.GetY ());
}

- (void) rightMouseUp : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	nodeEditor->OnMouseUp (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Right, position.GetX (), position.GetY ());
}

- (void) otherMouseDown : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	nodeEditor->OnMouseDown (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Middle, position.GetX (), position.GetY ());
}

- (void) otherMouseUp : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	nodeEditor->OnMouseUp (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Middle, position.GetX (), position.GetY ());
}

- (void) mouseDragged : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	nodeEditor->OnMouseMove (MAS::GetModifierKeysFromEvent (event), position.GetX (), position.GetY ());
}

- (void) rightMouseDragged : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	nodeEditor->OnMouseMove (MAS::GetModifierKeysFromEvent (event), position.GetX (), position.GetY ());
}

- (void) otherMouseDragged : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	nodeEditor->OnMouseMove (MAS::GetModifierKeysFromEvent (event), position.GetX (), position.GetY ());
}

- (void) scrollWheel : (NSEvent *) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::MouseWheelRotation rotation = NUIE::MouseWheelRotation::Forward;
	if ([event deltaX] + [event deltaY] < 0) {
		rotation = NUIE::MouseWheelRotation::Backward;
	}
	nodeEditor->OnMouseWheel(MAS::GetModifierKeysFromEvent(event), rotation, position.GetX (), position.GetY ());
}

- (void) keyDown : (NSEvent *) event
{
	NUIE::Key pressedKey (NUIE::KeyCode::Undefined);
	NSString* characters = [event charactersIgnoringModifiers];
	if ([characters length] == 1) {
		unichar character = [characters characterAtIndex:0];
		bool isControlPressed = [event modifierFlags] & NSEventModifierFlagControl;
		if (isControlPressed) {
			if (character == 'a') {
				pressedKey.SetKeyCode (NUIE::KeyCode::SelectAll);
			} else if (character == 'c') {
				pressedKey.SetKeyCode (NUIE::KeyCode::Copy);
			} else if (character == 'v') {
				pressedKey.SetKeyCode (NUIE::KeyCode::Paste);
			} else if (character == 'g') {
				pressedKey.SetKeyCode (NUIE::KeyCode::Group);
			} else if (character == 'z') {
				pressedKey.SetKeyCode (NUIE::KeyCode::Undo);
			} else if (character == 'y') {
				pressedKey.SetKeyCode (NUIE::KeyCode::Redo);
			}
		} else {
			// TODO: escape key
			if (character == NSDeleteFunctionKey) {
				pressedKey.SetKeyCode (NUIE::KeyCode::Delete);
			}
		}
	}
	if (pressedKey.IsValid ()) {
		nodeEditor->OnKeyPress (pressedKey);
	}
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
