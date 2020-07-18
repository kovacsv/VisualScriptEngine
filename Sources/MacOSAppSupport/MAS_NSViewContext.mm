#include "MAS_NSViewContext.hpp"
#include "NE_Debug.hpp"

namespace MAS
{

NSViewContext::NSViewContext () :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0)
{

}

NSViewContext::~NSViewContext ()
{

}

void NSViewContext::Init (void* nativeHandle)
{
	// TODO
	#pragma unused (nativeHandle)
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
	// TODO
	#pragma unused (beg)
	#pragma unused (end)
	#pragma unused (pen)
}

void NSViewContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	// TODO
	#pragma unused (p1)
	#pragma unused (p2)
	#pragma unused (p4)
	#pragma unused (p3)
	#pragma unused (pen)
}

void NSViewContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	// TODO
	#pragma unused (rect)
	#pragma unused (pen)
}

void NSViewContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	// TODO
	#pragma unused (rect)
	#pragma unused (color)
}

void NSViewContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	// TODO
	#pragma unused (rect)
	#pragma unused (pen)
}

void NSViewContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	// TODO
	#pragma unused (rect)
	#pragma unused (color)
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

}
