#include "MAS_NSViewContext.hpp"
#include "NE_Debug.hpp"

#import <Cocoa/Cocoa.h>

namespace MAS
{

static NSPoint CreatePoint (const NSView* view, const NUIE::Point& point)
{
	return NSMakePoint (point.GetX (), view.frame.size.height - point.GetY ());
}
	
static NSRect CreateRect (const NSView* view, const NUIE::Rect& rect)
{
	return NSMakeRect (rect.GetX (), view.frame.size.height - rect.GetHeight () - rect.GetY (), rect.GetWidth (), rect.GetHeight ());
}

static NSColor* CreateColor (const NUIE::Color& color)
{
	return [NSColor colorWithRed:color.GetR () / 255.0f green:color.GetG () / 255.0f blue:color.GetB () / 255.0f alpha:1.0f];
}
	
NSViewContext::NSViewContext () :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0),
	nsView (nullptr)
{

}

NSViewContext::~NSViewContext ()
{

}

void NSViewContext::Init (void* nativeHandle)
{
	// TODO
	#pragma unused (nativeHandle)
	nsView = nativeHandle;
}

void NSViewContext::BlitToWindow (void* nativeHandle)
{
	// TODO
	#pragma unused (nativeHandle)
}

void NSViewContext::BlitToContext (void* nativeContext)
{
	// TODO
	#pragma unused (nativeContext)
}

void NSViewContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	// TODO
}

double NSViewContext::GetWidth () const
{
	return width;
}

double NSViewContext::GetHeight () const
{
	return height;
}

void NSViewContext::BeginDraw ()
{

}

void NSViewContext::EndDraw ()
{

}

bool NSViewContext::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void NSViewContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	[CreateColor (pen.GetColor ()) set];
	NSBezierPath* bezierPath = [NSBezierPath bezierPath];
	[bezierPath setLineWidth:pen.GetThickness ()];
	[bezierPath moveToPoint:CreatePoint((NSView*) nsView, beg)];
	[bezierPath lineToPoint:CreatePoint((NSView*) nsView, end)];
	[bezierPath stroke];
}

void NSViewContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	[CreateColor (pen.GetColor ()) set];
	NSBezierPath* bezierPath = [NSBezierPath bezierPath];
	[bezierPath setLineWidth:pen.GetThickness ()];
	[bezierPath moveToPoint:CreatePoint((NSView*) nsView, p1)];
	[bezierPath curveToPoint:CreatePoint((NSView*) nsView, p4) controlPoint1:CreatePoint((NSView*) nsView, p2) controlPoint2:CreatePoint((NSView*) nsView, p3)];
	[bezierPath stroke];
}

void NSViewContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	[CreateColor (pen.GetColor ()) set];
	NSFrameRectWithWidth (CreateRect ((NSView*) nsView, rect), pen.GetThickness ());
}

void NSViewContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	[CreateColor (color) set];
	NSRectFill (CreateRect ((NSView*) nsView, rect));
}

void NSViewContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	[CreateColor (pen.GetColor ()) set];
	NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect((NSView*) nsView, rect)];
	[bezierPath setLineWidth:pen.GetThickness ()];
	[bezierPath stroke];
}

void NSViewContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	[CreateColor (color) set];
	NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect((NSView*) nsView, rect)];
	[bezierPath fill];
}

void NSViewContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	// TODO
	#pragma unused (rect)
	#pragma unused (font)
	#pragma unused (text)
	#pragma unused (hAnchor)
	#pragma unused (vAnchor)
	#pragma unused (textColor)
}

NUIE::Size NSViewContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	// TODO
	#pragma unused (font)
	#pragma unused (text)
	return NUIE::Size (text.length() * 10, 20);
}

bool NSViewContext::CanDrawIcon ()
{
	return false;
}

void NSViewContext::DrawIcon (const NUIE::Rect&, const NUIE::IconId&)
{
	DBGBREAK ();
}

}
