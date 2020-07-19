#include "MAS_NSViewContext.hpp"
#include "MAS_CocoaAppUtils.hpp"
#include "NE_Debug.hpp"

#import <Cocoa/Cocoa.h>

namespace MAS
{

static const float SafetyTextRatio = 1.05f;
	
static NSColor* CreateColor (const NUIE::Color& color)
{
	return [NSColor colorWithRed:color.GetR () / 255.0f green:color.GetG () / 255.0f blue:color.GetB () / 255.0f alpha:1.0f];
}
	
NSViewContext::NSViewContext () :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0),
	nsView (nil)
{

}

NSViewContext::~NSViewContext ()
{

}

void NSViewContext::Init (void* nativeHandle)
{
	nsView = (NSView*) nativeHandle;
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
	// TODO
	width = newWidth;
	height = newHeight;
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
	[bezierPath moveToPoint:CreatePoint((NSView*) (NSView*) nsView, beg)];
	[bezierPath lineToPoint:CreatePoint((NSView*) (NSView*) nsView, end)];
	[bezierPath stroke];
}

void NSViewContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	[CreateColor (pen.GetColor ()) set];
	NSBezierPath* bezierPath = [NSBezierPath bezierPath];
	[bezierPath setLineWidth:pen.GetThickness ()];
	[bezierPath moveToPoint:CreatePoint((NSView*) (NSView*) nsView, p1)];
	[bezierPath curveToPoint:CreatePoint((NSView*) (NSView*) nsView, p4) controlPoint1:CreatePoint((NSView*) (NSView*) nsView, p2) controlPoint2:CreatePoint((NSView*) (NSView*) nsView, p3)];
	[bezierPath stroke];
}

void NSViewContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	[CreateColor (pen.GetColor ()) set];
	NSFrameRectWithWidth (CreateRect ((NSView*) (NSView*) nsView, rect), pen.GetThickness ());
}

void NSViewContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	[CreateColor (color) set];
	NSRectFill (CreateRect ((NSView*) (NSView*) nsView, rect));
}

void NSViewContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	[CreateColor (pen.GetColor ()) set];
	NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect((NSView*) (NSView*) nsView, rect)];
	[bezierPath setLineWidth:pen.GetThickness ()];
	[bezierPath stroke];
}

void NSViewContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	[CreateColor (color) set];
	NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect((NSView*) (NSView*) nsView, rect)];
	[bezierPath fill];
}

void NSViewContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	[CreateColor (textColor) set];
	NSString* nsText = StdStringToNSString (text);
	NSString* nsFontName = StdStringToNSString (font.GetFamily ());
	
	NSMutableParagraphStyle* style = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
	if (hAnchor == NUIE::HorizontalAnchor::Left) {
		style.alignment = NSTextAlignmentLeft;
	} else if (hAnchor == NUIE::HorizontalAnchor::Center) {
		style.alignment = NSTextAlignmentCenter;
	} else if (hAnchor == NUIE::HorizontalAnchor::Right) {
		style.alignment = NSTextAlignmentRight;
	}
	NSDictionary* attributes = @{
		NSFontAttributeName: [NSFont fontWithName:nsFontName size:font.GetSize ()],
		NSForegroundColorAttributeName : CreateColor (textColor),
		NSParagraphStyleAttributeName : style
	};
	NSRect textRect = CreateRect((NSView*) (NSView*) nsView, rect);
	NSSize textSize = [nsText sizeWithAttributes:attributes];
	if (vAnchor == NUIE::VerticalAnchor::Top) {
		// nothing to do
	} else if (vAnchor == NUIE::VerticalAnchor::Center) {
		textRect.origin.y -= (textRect.size.height - textSize.height) / 2.0;
	} else if (vAnchor == NUIE::VerticalAnchor::Bottom) {
		textRect.origin.y -= (textRect.size.height - textSize.height);
	}
	[nsText drawInRect:textRect withAttributes:attributes];
}

NUIE::Size NSViewContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	NSString* nsText = StdStringToNSString (text);
	NSString* nsFontName = StdStringToNSString (font.GetFamily ());
	NSDictionary* attributes = @{NSFontAttributeName: [NSFont fontWithName:nsFontName size:font.GetSize ()]};
	NSSize size = [nsText sizeWithAttributes:attributes];
	return NUIE::Size (size.width * SafetyTextRatio, size.height * SafetyTextRatio);
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
