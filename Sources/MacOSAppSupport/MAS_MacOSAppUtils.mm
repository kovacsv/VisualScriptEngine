#include "MAS_MacOSAppUtils.hpp"

namespace MAS
{

NUIE::ModifierKeys GetModifierKeysFromEvent (const NSEvent* event)
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

NUIE::Point GetViewPositionFromEvent (NSView* view, const NSEvent* event)
{
	NSPoint position = [view convertPoint:[event locationInWindow] fromView:nil];
	return NUIE::Point (position.x, view.frame.size.height - position.y);
}
	
NSPoint CreatePoint (const NSView* view, const NUIE::Point& point)
{
	return NSMakePoint (point.GetX (), view.frame.size.height - point.GetY ());
}
	
NSPoint CreateScreenPoint (const NSView* view, const NUIE::Point& point)
{
	NSPoint viewPoint = MAS::CreatePoint (view, point);
	return [view convertPoint:viewPoint toView:nil];
}
	
NSRect CreateRect (const NSView* view, const NUIE::Rect& rect)
{
	return NSMakeRect (rect.GetX (), view.frame.size.height - rect.GetHeight () - rect.GetY (), rect.GetWidth (), rect.GetHeight ());
}
	
}
