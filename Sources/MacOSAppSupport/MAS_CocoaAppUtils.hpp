#ifndef MAS_COCOAAPPUTILS_HPP
#define MAS_COCOAAPPUTILS_HPP

#include "NUIE_InputEventHandler.hpp"
#include "NUIE_MenuCommands.hpp"
#include "NUIE_Geometry.hpp"
#include "NUIE_Drawing.hpp"

#import <Cocoa/Cocoa.h>

namespace MAS
{

NSString*				StdStringToNSString (const std::wstring& str);
	
NUIE::ModifierKeys		GetModifierKeysFromEvent (const NSEvent* event);
NUIE::Point				GetViewPositionFromEvent (NSView* view, const NSEvent* event);

NSPoint					CreatePoint (const NSView* view, const NUIE::Point& point);
NSPoint					CreateScreenPoint (const NSView* view, const NUIE::Point& point);
NSRect					CreateRect (const NSView* view, const NUIE::Rect& rect);
NSColor*				CreateColor (const NUIE::Color& color);
	
NUIE::MenuCommandPtr	SelectCommandFromContextMenu (const NSView* nsView, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands);
	
}

#endif
