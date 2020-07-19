#include "MAS_NodeEditorNSViewControl.hpp"
#include "MAS_CocoaAppUtils.hpp"
#include "MAS_NSViewContext.hpp"
#include "NE_Debug.hpp"

#import <Cocoa/Cocoa.h>

@interface CocoaNSViewControl : NSView
{
@private MAS::NodeEditorNSViewControl* nodeEditorControl;
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
		nodeEditorControl = nil;
	}
	return self;
}

-(void) setNodeEditorControl : (MAS::NodeEditorNSViewControl*) newNodeEditorControl
{
	nodeEditorControl = newNodeEditorControl;
}

- (void) drawRect : (NSRect) dirtyRect
{
	#pragma unused (dirtyRect)
	nodeEditorControl->Draw ();
}

- (void) mouseDown : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseDown (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Left, position.GetX (), position.GetY ());
}

- (void) mouseUp : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseUp (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Left, position.GetX (), position.GetY ());
}

- (void) rightMouseDown : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseDown (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Right, position.GetX (), position.GetY ());
}

- (void) rightMouseUp : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseUp (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Right, position.GetX (), position.GetY ());
}

- (void) otherMouseDown : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseDown (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Middle, position.GetX (), position.GetY ());
}

- (void) otherMouseUp : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseUp (MAS::GetModifierKeysFromEvent (event), NUIE::MouseButton::Middle, position.GetX (), position.GetY ());
}

- (void) mouseDragged : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseMove (MAS::GetModifierKeysFromEvent (event), position.GetX (), position.GetY ());
}

- (void) rightMouseDragged : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseMove (MAS::GetModifierKeysFromEvent (event), position.GetX (), position.GetY ());
}

- (void) otherMouseDragged : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseMove (MAS::GetModifierKeysFromEvent (event), position.GetX (), position.GetY ());
}

- (void) scrollWheel : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::MouseWheelRotation rotation = NUIE::MouseWheelRotation::Forward;
	if ([event deltaX] + [event deltaY] < 0) {
		rotation = NUIE::MouseWheelRotation::Backward;
	}
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseWheel(MAS::GetModifierKeysFromEvent(event), rotation, position.GetX (), position.GetY ());
}

- (void) keyDown : (NSEvent*) event
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
		NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
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
	nsView (nil)
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
	nsView = [[[CocoaNSViewControl alloc] initWithFrame:viewRect] autorelease];
	[((CocoaNSViewControl*) nsView) setNodeEditorControl:this];
	[((NSView*) nativeParentHandle) addSubview:((NSView*) nsView)];
	
	nativeContext->Init (nsView);
	nativeContext->Resize (width, height);
	return true;
}

void* NodeEditorNSViewControl::GetEditorNativeHandle () const
{
	return nsView;
}

bool NodeEditorNSViewControl::IsEditorFocused () const
{
	// TODO
	return false;
}

void NodeEditorNSViewControl::Resize (int x, int y, int width, int height)
{
	NSRect viewRect = NSMakeRect (x, y, width, height);
	[((NSView*) nsView) setFrame:viewRect];
	if (nodeEditor != nullptr) {
		nodeEditor->OnResize (width, height);
	}
}

void NodeEditorNSViewControl::Invalidate ()
{
	[((NSView*) nsView) setNeedsDisplay:YES];
}

void NodeEditorNSViewControl::Draw ()
{
	nodeEditor->Draw ();
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
