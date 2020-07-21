#include "MAS_NSViewContext.hpp"
#include "MAS_CocoaAppUtils.hpp"
#include "NE_Debug.hpp"

#import <Cocoa/Cocoa.h>

namespace MAS
{

static const float SafetyTextRatio = 1.05f;
	
NSViewContext::NSViewContext () :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0),
	nsView (nil),
	fontCache ()
{

}

NSViewContext::~NSViewContext ()
{
	for (auto& it : fontCache) {
		[(NSFont*) it.second release];
	}
}

void NSViewContext::Init (void* nativeHandle)
{
	nsView = (NSView*) nativeHandle;
}

void NSViewContext::BlitToWindow (void* nativeHandle)
{
	#pragma unused (nativeHandle)
}

void NSViewContext::BlitToContext (void* nativeContext)
{
	#pragma unused (nativeContext)
}

void NSViewContext::Resize (int newWidth, int newHeight)
{
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
	[bezierPath moveToPoint:CreatePoint ((NSView*) nsView, beg)];
	[bezierPath lineToPoint:CreatePoint ((NSView*) nsView, end)];
	[bezierPath stroke];
}

void NSViewContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	[CreateColor (pen.GetColor ()) set];
	NSBezierPath* bezierPath = [NSBezierPath bezierPath];
	[bezierPath setLineWidth:pen.GetThickness ()];
	[bezierPath moveToPoint:CreatePoint ((NSView*) nsView, p1)];
	[bezierPath curveToPoint:CreatePoint ((NSView*) nsView, p4) controlPoint1:CreatePoint ((NSView*) nsView, p2) controlPoint2:CreatePoint ((NSView*) nsView, p3)];
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
	NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect ((NSView*) nsView, rect)];
	[bezierPath setLineWidth:pen.GetThickness ()];
	[bezierPath stroke];
}

void NSViewContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	[CreateColor (color) set];
	NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect ((NSView*) nsView, rect)];
	[bezierPath fill];
}

void NSViewContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	// TODO: speed (cache [NSString(NSStringDrawing) sizeWithAttributes:] result)
	
	[CreateColor (textColor) set];
	NSString* nsText = StdStringToNSString (text);
	
	NSMutableParagraphStyle* style = [[[NSParagraphStyle defaultParagraphStyle] mutableCopy] autorelease];
	if (hAnchor == NUIE::HorizontalAnchor::Left) {
		style.alignment = NSTextAlignmentLeft;
	} else if (hAnchor == NUIE::HorizontalAnchor::Center) {
		style.alignment = NSTextAlignmentCenter;
	} else if (hAnchor == NUIE::HorizontalAnchor::Right) {
		style.alignment = NSTextAlignmentRight;
	}
	NSDictionary* attributes = @{
		NSFontAttributeName: (NSFont*) GetFont (font),
		NSForegroundColorAttributeName : CreateColor (textColor),
		NSParagraphStyleAttributeName : style
	};
	NSRect textRect = CreateRect ((NSView*) nsView, rect);
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
	NSDictionary* attributes = @{NSFontAttributeName: (NSFont*) GetFont (font)};
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

void* NSViewContext::GetFont (const NUIE::Font& font)
{
	NUIE::FontCacheKey key (font);
	auto found = fontCache.find (key);
	if (found == fontCache.end ()) {
		NSString* nsFontName = StdStringToNSString (key.family);
		NSFont* nsFont = [[NSFont fontWithName:nsFontName size:key.size] copy];
		fontCache.insert ({ key, nsFont });
	}
	return fontCache.at (key);
}

}
