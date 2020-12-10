#include "MAS_NSImageContext.hpp"
#include "MAS_CocoaAppUtils.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

#include "NE_Debug.hpp"

namespace MAS
{

static const float SafetyTextRatio = 1.05f;

NSImageContext::NSImageContext () :
	NSImageContext (nullptr)
{

}

NSImageContext::NSImageContext (const NSImageLoaderPtr& imageLoader) :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0),
	nsView (nil),
	imageLoader (imageLoader),
	image (nil),
	fontCache ()
{

}

NSImageContext::~NSImageContext ()
{
	@autoreleasepool {
		@try {
			for (auto& it : fontCache) {
				[it.second release];
			}
			[image release];
		} @catch (NSException*) {
			
		}
	}
}

void NSImageContext::Init (void* nativeHandle)
{
	nsView = (NSView*) nativeHandle;
}

void NSImageContext::BlitToWindow (void*)
{

}

void NSImageContext::BlitToContext (void* cgContext)
{
	NSRect imgRect = nsView.frame;
	NSSize imgSize = nsView.frame.size;

	CGContextRef ctx = (CGContextRef)cgContext;
	if (ctx != nil) {
		NSImage* newImage = FlipImageVertically (image);
		CGImageRef cgImage = [newImage CGImageForProposedRect: &imgRect context: [NSGraphicsContext currentContext] hints: nil];
		CGContextDrawImage (ctx, CGRectMake (0, 0, imgSize.width, imgSize.height), cgImage);
	}
}

void NSImageContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}

int NSImageContext::GetWidth () const
{
	return width;
}

int NSImageContext::GetHeight () const
{
	return height;
}

void NSImageContext::BeginDraw ()
{
	@try {
		image = [[NSImage alloc] initWithSize:nsView.frame.size];
		[image lockFocus];
	} @catch (NSException*) {
		
	}
}

void NSImageContext::EndDraw ()
{
	@try {
		[image unlockFocus];
		[image drawInRect: nsView.frame];
	} @catch (NSException*) {
		
	}
}

bool NSImageContext::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void NSImageContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
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

void NSImageContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
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

void NSImageContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	@autoreleasepool {
		@try {
			[CreateColor (pen.GetColor ()) set];
			NSFrameRectWithWidth (CreateRect (nsView, rect), pen.GetThickness ());
		} @catch (NSException*) {
			
		}
	}
}

void NSImageContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	@autoreleasepool {
		@try {
			[CreateColor (color) set];
			NSRectFill (CreateRect (nsView, rect));
		} @catch (NSException*) {
			
		}
	}
}

void NSImageContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
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

void NSImageContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
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

void NSImageContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	@autoreleasepool {
		@try {
			[CreateColor (textColor) set];
			NSString* nsText = StdWStringToNSString (text);
			
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
				textRect.origin.y -= std::floor ((textRect.size.height - textSize.height) / 2.0);
			} else if (vAnchor == NUIE::VerticalAnchor::Bottom) {
				textRect.origin.y -= (textRect.size.height - textSize.height);
			}
			[nsText drawInRect:textRect withAttributes:attributes];
		} @catch (NSException*) {
			
		}
	}
}

NUIE::Size NSImageContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	@autoreleasepool {
		@try {
			NSString* nsText = StdWStringToNSString (text);
			NSDictionary* attributes = @{NSFontAttributeName: GetFont (font)};
			NSSize size = [nsText sizeWithAttributes:attributes];
			return NUIE::Size (size.width * SafetyTextRatio, size.height * SafetyTextRatio);
		} @catch (NSException*) {

		}
	}
	return NUIE::Size ();
}

bool NSImageContext::CanDrawIcon ()
{
	return imageLoader != nullptr;
}

void NSImageContext::DrawIcon (const NUIE::Rect& rect, const NUIE::IconId& iconId)
{
	if (DBGERROR (imageLoader == nullptr)) {
		return;
	}
	@try {
		NSImage* image = imageLoader->LoadNSImage (iconId);
		[image drawInRect:CreateRect (nsView, rect) fromRect:NSZeroRect operation:NSCompositingOperationSourceOver fraction:1.0f];
	} @catch (NSException*) {

	}
}

NSFont* NSImageContext::GetFont (const NUIE::Font& font)
{
	@autoreleasepool {
		@try {
			NUIE::FontCacheKey key (font);
			auto found = fontCache.find (key);
			if (found == fontCache.end ()) {
				NSString* nsFontName = StdWStringToNSString (key.family);
				NSFont* nsFont = [[NSFont fontWithName:nsFontName size:key.size] copy];
				fontCache.insert ({ key, nsFont });
			}
			return fontCache.at (key);
		} @catch (NSException*) {

		}
	}
	return nil;
}

}
