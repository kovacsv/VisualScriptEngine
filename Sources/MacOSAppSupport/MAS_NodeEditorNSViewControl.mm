#include "MAS_NodeEditorNSViewControl.hpp"
#include "MAS_CocoaAppUtils.hpp"
#include "MAS_NSViewContext.hpp"
#include "NE_Debug.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

static void MouseDownEvent (NUIE::NodeEditor* nodeEditor, NSEvent* event, NUIE::MouseButton button, int x, int y)
{
	NUIE::ModifierKeys modifierKeys = MAS::GetModifierKeysFromEvent (event);
	if (event.clickCount == 2) {
		nodeEditor->OnMouseDoubleClick (modifierKeys, button, x, y);
	} else {
		nodeEditor->OnMouseDown (modifierKeys, button, x, y);
	}
}

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

- (void) setNodeEditorControl : (MAS::NodeEditorNSViewControl*) newNodeEditorControl
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
	if ([event modifierFlags] & NSEventModifierFlagControl) {
		nodeEditor->OnContextMenuRequest (position.GetX (), position.GetY ());
	} else {
		MouseDownEvent (nodeEditor, event, NUIE::MouseButton::Left, position.GetX (), position.GetY ());
	}
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
	MouseDownEvent (nodeEditor, event, NUIE::MouseButton::Right, position.GetX (), position.GetY ());
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
	MouseDownEvent (nodeEditor, event, NUIE::MouseButton::Middle, position.GetX (), position.GetY ());
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
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	if ([event modifierFlags] & NSEventModifierFlagOption || [event subtype] == 0) {
		NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
		NUIE::MouseWheelRotation rotation = NUIE::MouseWheelRotation::Forward;
		if ([event scrollingDeltaX] + [event scrollingDeltaY] < 0) {
			rotation = NUIE::MouseWheelRotation::Backward;
		}
		nodeEditor->OnMouseWheel (MAS::GetModifierKeysFromEvent(event), rotation, position.GetX (), position.GetY ());
	} else {
		nodeEditor->OffsetViewBox ([event scrollingDeltaX], [event scrollingDeltaY]);
	}
}

- (void) magnifyWithEvent : (NSEvent*) event
{
	NUIE::Point position = MAS::GetViewPositionFromEvent (self, event);
	NUIE::MouseWheelRotation rotation = NUIE::MouseWheelRotation::Forward;
	if ([event magnification] < 0) {
		rotation = NUIE::MouseWheelRotation::Backward;
	}
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OnMouseWheel(MAS::GetModifierKeysFromEvent(event), rotation, position.GetX (), position.GetY ());
}

- (void) swipeWithEvent : (NSEvent*) event {
	NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
	nodeEditor->OffsetViewBox ([event deltaX], [event deltaY]);
}

- (void) keyDown : (NSEvent*) event
{
	if (!nodeEditorControl->IsMouseOverEditorWindow ()) {
		return;
	}

	NUIE::CommandCode commandCode = NUIE::CommandCode::Undefined;
	NSString* characters = [event charactersIgnoringModifiers];
	
	if ([characters length] == 1) {
		unichar character = tolower ([characters characterAtIndex:0]);
		bool isCommandPressed = [event modifierFlags] & NSEventModifierFlagCommand;
		bool isShiftPressed = [event modifierFlags] & NSEventModifierFlagShift;
		if (isCommandPressed) {
			if (character == 'a') {
				commandCode = NUIE::CommandCode::SelectAll;
			} else if (character == 'c') {
				commandCode = NUIE::CommandCode::Copy;
			} else if (character == 'v') {
				commandCode = NUIE::CommandCode::Paste;
			} else if (character == 'g') {
				if (isShiftPressed) {
					commandCode = NUIE::CommandCode::Ungroup;
				} else {
					commandCode = NUIE::CommandCode::Group;
				}
			} else if (character == 'z') {
				if (isShiftPressed) {
					commandCode = NUIE::CommandCode::Redo;
				} else {
					commandCode = NUIE::CommandCode::Undo;
				}
			}
		} else {
			static const unichar EscapeKeyCode = u'\x1b';
			if (character == NSDeleteFunctionKey || character == NSDeleteCharacter) {
				commandCode = NUIE::CommandCode::Delete;
			} else if (character == EscapeKeyCode) {
				commandCode = NUIE::CommandCode::Escape;
			}
		}
	}
	if (commandCode != NUIE::CommandCode::Undefined) {
		NUIE::NodeEditor* nodeEditor = nodeEditorControl->GetNodeEditor ();
		nodeEditor->ExecuteCommand (commandCode);
	}
}

- (void) cursorUpdate : (NSEvent *) theEvent {
	#pragma unused (theEvent)
	[[NSCursor arrowCursor] set];
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
	@autoreleasepool {
		@try {
			[nsView release];
		} @catch (NSException*) {
			
		}
	}
}

bool NodeEditorNSViewControl::Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height)
{
	nodeEditor = nodeEditorPtr;
	DBGASSERT (nodeEditor != nullptr);

	@autoreleasepool {
		@try {
			NSRect viewRect = NSMakeRect (x, y, width, height);
			nsView = [[CocoaNSViewControl alloc] initWithFrame : viewRect];
			[((CocoaNSViewControl*) nsView) setNodeEditorControl : this];
			[((NSView*) nativeParentHandle) addSubview : nsView];
		} @catch (NSException*) {
			
		}
	}
	
	nativeContext->Init (nsView);
	nativeContext->Resize (width, height);
	return true;
}

void* NodeEditorNSViewControl::GetEditorNativeHandle () const
{
	return nsView;
}

bool NodeEditorNSViewControl::IsMouseOverEditorWindow () const
{
	NSPoint mousePos = [NSEvent mouseLocation];
	NSPoint windowLocation = [[nsView window] convertPointFromScreen : mousePos];
	NSPoint viewLocation = [nsView convertPoint : windowLocation fromView: nil];
	return NSPointInRect (viewLocation, [nsView bounds]);
}

void NodeEditorNSViewControl::Resize (int x, int y, int width, int height)
{
	@autoreleasepool {
		@try {
			NSRect viewRect = NSMakeRect (x, y, width, height);
			[nsView setFrame:viewRect];
			if (nodeEditor != nullptr) {
				nodeEditor->OnResize (width, height);
			}
		} @catch (NSException*) {
			
		}
	}
}

void NodeEditorNSViewControl::Invalidate ()
{
	@autoreleasepool {
		@try {
			[nsView setNeedsDisplay:YES];
		} @catch (NSException*) {
			
		}
	}
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
