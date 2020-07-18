#ifndef MAS_MACOSAPPUTILS_HPP
#define MAS_MACOSAPPUTILS_HPP

#include "NUIE_InputEventHandler.hpp"

#import <Cocoa/Cocoa.h>

namespace MAS
{

NUIE::ModifierKeys	GetModifierKeysFromEvent (const NSEvent* event);
NUIE::Point			GetPositionFromEvent (NSView* view, const NSEvent* event);

NSPoint				CreatePoint (const NSView* view, const NUIE::Point& point);
NSRect				CreateRect (const NSView* view, const NUIE::Rect& rect);
	
}

#endif
