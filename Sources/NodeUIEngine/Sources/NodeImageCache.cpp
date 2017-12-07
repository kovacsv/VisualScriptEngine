#include "NodeImageCache.hpp"

namespace NUIE
{

NodeImageCachedContext::NodeImageCachedContext (const NE::Checksum& checksum, const IntRect& nodeRect) :
	checksum (checksum),
	nodeRect (nodeRect),
	isUpToDate (false)
{

}

bool NodeImageCachedContext::IsUpToDate () const
{
	return isUpToDate;
}

bool NodeImageCachedContext::IsUpToDate (const NE::Checksum& newChecksum) const
{
	return checksum == newChecksum;
}

void NodeImageCachedContext::SetUpToDate (bool newIsUpToDate)
{
	isUpToDate = newIsUpToDate;
}

const IntRect& NodeImageCachedContext::GetRect () const
{
	return nodeRect;
}

void NodeImageCachedContext::UpdateRect (const IntRect& rect)
{
	nodeRect = rect;
	isUpToDate = true;
}

void NodeImageCachedContext::Resize (int newWidth, int newHeight)
{
	return GetContext ().Resize (newWidth, newHeight);
}

double NodeImageCachedContext::GetWidth () const
{
	return GetContext ().GetWidth ();
}

double NodeImageCachedContext::GetHeight () const
{
	return GetContext ().GetHeight ();
}

void NodeImageCachedContext::BeginDraw ()
{
	GetContext ().BeginDraw ();
}

void NodeImageCachedContext::EndDraw ()
{
	GetContext ().EndDraw ();
}

void NodeImageCachedContext::DrawLine (const Point& beg, const Point& end, const Pen& pen)
{
	GetContext ().DrawLine (GetOffsettedPoint (beg), GetOffsettedPoint (end), pen);
}

void NodeImageCachedContext::DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen)
{
	GetContext ().DrawBezier (GetOffsettedPoint (p1), GetOffsettedPoint (p2), GetOffsettedPoint (p3), GetOffsettedPoint (p4), pen);
}

void NodeImageCachedContext::DrawRect (const Rect& rect, const Pen& pen)
{
	GetContext ().DrawRect (GetOffsettedRect (rect), pen);
}

void NodeImageCachedContext::FillRect (const Rect& rect, const Color& color)
{
	GetContext ().FillRect (GetOffsettedRect (rect), color);
}

void NodeImageCachedContext::DrawEllipse (const Rect& rect, const Pen& pen)
{
	GetContext ().DrawEllipse (GetOffsettedRect (rect), pen);
}

void NodeImageCachedContext::FillEllipse (const Rect& rect, const Color& color)
{
	GetContext ().FillEllipse (GetOffsettedRect (rect), color);
}

void NodeImageCachedContext::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color)
{
	GetContext ().DrawFormattedText (GetOffsettedRect (rect), font, text, hAnchor, vAnchor, color);
}

Size NodeImageCachedContext::MeasureText (const Font& font, const std::wstring& text)
{
	return GetContext ().MeasureText (font, text);
}

Point NodeImageCachedContext::GetOffsettedPoint (const Point& point)
{
	return point - Point (nodeRect.GetX (), nodeRect.GetY ());
}

Rect NodeImageCachedContext::GetOffsettedRect (const Rect& rect)
{
	return rect.Offset (-Point (nodeRect.GetX (), nodeRect.GetY ()));
}

}
