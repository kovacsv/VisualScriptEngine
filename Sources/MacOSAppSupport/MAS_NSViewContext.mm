#include "MAS_NSViewContext.hpp"
#include "MAS_CocoaAppUtils.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

#include "NE_Debug.hpp"

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
	@autoreleasepool {
		@try {
			for (auto& it : fontCache) {
				[it.second release];
			}
		} @catch (NSException*) {
			
		}
	}
}

void NSViewContext::Init (void* nativeHandle)
{
	nsView = (NSView*) nativeHandle;
}

void NSViewContext::BlitToWindow (void*)
{

}

void NSViewContext::BlitToContext (void*)
{

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
	@autoreleasepool {
		@try {
			[CreateColor (pen.GetColor ()) set];
			NSBezierPath* bezierPath = [NSBezierPath bezierPath];
			[bezierPath setLineWidth:pen.GetThickness ()];
			[bezierPath moveToPoint:CreatePoint (nsView, beg)];
			[bezierPath lineToPoint:CreatePoint (nsView, end)];
			[bezierPath stroke];
		} @catch (NSException*) {
			
		}
	}
}

void NSViewContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	@autoreleasepool {
		@try {
			[CreateColor (pen.GetColor ()) set];
			NSBezierPath* bezierPath = [NSBezierPath bezierPath];
			[bezierPath setLineWidth:pen.GetThickness ()];
			[bezierPath moveToPoint:CreatePoint (nsView, p1)];
			[bezierPath curveToPoint:CreatePoint (nsView, p4) controlPoint1:CreatePoint (nsView, p2) controlPoint2:CreatePoint (nsView, p3)];
			[bezierPath stroke];
		} @catch (NSException*) {
			
		}
	}
}

void NSViewContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	@autoreleasepool {
		@try {
			[CreateColor (pen.GetColor ()) set];
			NSFrameRectWithWidth (CreateRect (nsView, rect), pen.GetThickness ());
		} @catch (NSException*) {
			
		}
	}
}

void NSViewContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	@autoreleasepool {
		@try {
			[CreateColor (color) set];
			NSRectFill (CreateRect (nsView, rect));
		} @catch (NSException*) {
			
		}
	}
}

void NSViewContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	@autoreleasepool {
		@try {
			[CreateColor (pen.GetColor ()) set];
			NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect (nsView, rect)];
			[bezierPath setLineWidth:pen.GetThickness ()];
			[bezierPath stroke];
		} @catch (NSException*) {
			
		}
	}
}

void NSViewContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	@autoreleasepool {
		@try {
			[CreateColor (color) set];
			NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect (nsView, rect)];
			[bezierPath fill];
		} @catch (NSException*) {
			
		}
	}
}

void NSViewContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	@autoreleasepool {
		@try {
			// TODO: speed (cache [NSString(NSStringDrawing) sizeWithAttributes:] result)
			
			[CreateColor (textColor) set];
			NSString* nsText = MAS::StdWStringToNSString (text);
			
			NSMutableParagraphStyle* style = [[[NSParagraphStyle defaultParagraphStyle] mutableCopy] autorelease];
			if (hAnchor == NUIE::HorizontalAnchor::Left) {
				style.alignment = NSTextAlignmentLeft;
			} else if (hAnchor == NUIE::HorizontalAnchor::Center) {
				style.alignment = NSTextAlignmentCenter;
			} else if (hAnchor == NUIE::HorizontalAnchor::Right) {
				style.alignment = NSTextAlignmentRight;
			}
			NSDictionary* attributes = @{
										 NSFontAttributeName: GetFont (font),
										 NSForegroundColorAttributeName : CreateColor (textColor),
										 NSParagraphStyleAttributeName : style
										 };
			NSRect textRect = CreateRect (nsView, rect);
			NSSize textSize = [nsText sizeWithAttributes:attributes];
			if (vAnchor == NUIE::VerticalAnchor::Top) {
				// nothing to do
			} else if (vAnchor == NUIE::VerticalAnchor::Center) {
				textRect.origin.y -= (textRect.size.height - textSize.height) / 2.0;
			} else if (vAnchor == NUIE::VerticalAnchor::Bottom) {
				textRect.origin.y -= (textRect.size.height - textSize.height);
			}
			[nsText drawInRect:textRect withAttributes:attributes];
		} @catch (NSException*) {
			
		}
	}
}

NUIE::Size NSViewContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	@autoreleasepool {
		@try {
			NSString* nsText = MAS::StdWStringToNSString (text);
			NSDictionary* attributes = @{NSFontAttributeName: GetFont (font)};
			NSSize size = [nsText sizeWithAttributes:attributes];
			return NUIE::Size (size.width * SafetyTextRatio, size.height * SafetyTextRatio);
		} @catch (NSException*) {

		}
	}
	return NUIE::Size ();
}

bool NSViewContext::CanDrawIcon ()
{
	return false;
}

void NSViewContext::DrawIcon (const NUIE::Rect&, const NUIE::IconId&)
{
	DBGBREAK ();
}

NSFont* NSViewContext::GetFont (const NUIE::Font& font)
{
	@autoreleasepool {
		@try {
			NUIE::FontCacheKey key (font);
			auto found = fontCache.find (key);
			if (found == fontCache.end ()) {
				NSString* nsFontName = MAS::StdWStringToNSString (key.family);
				NSFont* nsFont = [[NSFont fontWithName:nsFontName size:key.size] copy];
				fontCache.insert ({ key, nsFont });
			}
			return fontCache.at (key);
		} @catch (NSException*) {
			return nil;
		}
	}
	return nil;
}

}
