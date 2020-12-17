#include "MAS_NSViewOffscreenContext.hpp"
#include "MAS_CocoaAppUtils.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

#include "NE_Debug.hpp"

namespace MAS
{

static const float SafetyTextRatio = 1.05f;

NSViewOffscreenContext::NSViewOffscreenContext (Orientation orientation) :
	NSViewOffscreenContext (orientation, nullptr)
{

}

NSViewOffscreenContext::NSViewOffscreenContext (Orientation orientation, const NSImageLoaderPtr& imageLoader) :
	NUIE::NativeDrawingContext (),
	orientation (orientation),
	width (0),
	height (0),
	imageLoader (imageLoader),
	image (nil),
	fontCache ()
{

}

NSViewOffscreenContext::~NSViewOffscreenContext ()
{
	@autoreleasepool {
		@try {
			for (auto& it : fontCache) {
				[it.second release];
			}
			if (image != nil) {
				[image release];
			}
		} @catch (NSException*) {
			
		}
	}
}

void NSViewOffscreenContext::Init (void* nativeHandle)
{
	NSView* nsView = (NSView*) nativeHandle;
	width = nsView.frame.size.width;
	height = nsView.frame.size.height;
	
	CGSize size = { (CGFloat) width, (CGFloat) height };
	image = [[NSImage alloc] initWithSize:size];
}

void NSViewOffscreenContext::BlitToWindow (void*)
{
	
}

void NSViewOffscreenContext::BlitToContext (void* cgContext)
{
	CGContextRef ctx = (CGContextRef) cgContext;
	if (ctx != nil) {
		if (orientation == Orientation::FlippedVertically) {
			CGContextTranslateCTM (ctx, 1.0, height);
			CGContextScaleCTM (ctx, 1.0, -1.0);
		}
		CGRect imgRect = CGRectMake (0, 0, width, height);
		CGImageRef cgImage = [image CGImageForProposedRect: &imgRect context: [NSGraphicsContext currentContext] hints: nil];
		CGContextDrawImage (ctx, imgRect, cgImage);
		if (orientation == Orientation::FlippedVertically) {
			CGContextScaleCTM (ctx, 1.0, -1.0);
			CGContextTranslateCTM (ctx, 1.0, -height);
		}
	}
}

void NSViewOffscreenContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	CGSize size = { (CGFloat) width, (CGFloat) height };
	image = [[NSImage alloc] initWithSize:size];
}

int NSViewOffscreenContext::GetWidth () const
{
	return width;
}

int NSViewOffscreenContext::GetHeight () const
{
	return height;
}

void NSViewOffscreenContext::BeginDraw ()
{
	@try {
		[image lockFocus];
	} @catch (NSException*) {
		
	}
}

void NSViewOffscreenContext::EndDraw ()
{
	@try {
		[image unlockFocus];
	} @catch (NSException*) {
		
	}
}

bool NSViewOffscreenContext::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void NSViewOffscreenContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	@autoreleasepool {
		@try {
			[CreateColor (pen.GetColor ()) set];
			NSBezierPath* bezierPath = [NSBezierPath bezierPath];
			[bezierPath setLineWidth:pen.GetThickness ()];
			[bezierPath moveToPoint:CreatePoint (height, beg)];
			[bezierPath lineToPoint:CreatePoint (height, end)];
			[bezierPath stroke];
		} @catch (NSException*) {
			
		}
	}
}

void NSViewOffscreenContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	@autoreleasepool {
		@try {
			[CreateColor (pen.GetColor ()) set];
			NSBezierPath* bezierPath = [NSBezierPath bezierPath];
			[bezierPath setLineWidth:pen.GetThickness ()];
			[bezierPath moveToPoint:CreatePoint (height, p1)];
			[bezierPath curveToPoint:CreatePoint (height, p4) controlPoint1:CreatePoint (height, p2) controlPoint2:CreatePoint (height, p3)];
			[bezierPath stroke];
		} @catch (NSException*) {
			
		}
	}
}

void NSViewOffscreenContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	@autoreleasepool {
		@try {
			[CreateColor (pen.GetColor ()) set];
			NSFrameRectWithWidth (CreateRect (height, rect), pen.GetThickness ());
		} @catch (NSException*) {
			
		}
	}
}

void NSViewOffscreenContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	@autoreleasepool {
		@try {
			[CreateColor (color) set];
			NSRectFill (CreateRect (height, rect));
		} @catch (NSException*) {
			
		}
	}
}

void NSViewOffscreenContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	@autoreleasepool {
		@try {
			[CreateColor (pen.GetColor ()) set];
			NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect (height, rect)];
			[bezierPath setLineWidth:pen.GetThickness ()];
			[bezierPath stroke];
		} @catch (NSException*) {
			
		}
	}
}

void NSViewOffscreenContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	@autoreleasepool {
		@try {
			[CreateColor (color) set];
			NSBezierPath* bezierPath = [NSBezierPath bezierPathWithOvalInRect:CreateRect (height, rect)];
			[bezierPath fill];
		} @catch (NSException*) {
			
		}
	}
}

void NSViewOffscreenContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
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
			NSRect textRect = CreateRect (height, rect);
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

NUIE::Size NSViewOffscreenContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
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

bool NSViewOffscreenContext::CanDrawIcon ()
{
	return imageLoader != nullptr;
}

void NSViewOffscreenContext::DrawIcon (const NUIE::Rect& rect, const NUIE::IconId& iconId)
{
	if (DBGERROR (imageLoader == nullptr)) {
		return;
	}
	@try {
		NSImage* iconImage = imageLoader->LoadNSImage (iconId);
		[iconImage drawInRect:CreateRect (height, rect) fromRect:NSZeroRect operation:NSCompositingOperationSourceOver fraction:1.0f];
	} @catch (NSException*) {

	}
}

NSFont* NSViewOffscreenContext::GetFont (const NUIE::Font& font)
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
