#include "NodeImageCache.hpp"

NodeImageCachedContext::NodeImageCachedContext (const NE::Checksum& checksum, const NUIE::IntRect& nodeRect) :
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

const NUIE::IntRect& NodeImageCachedContext::GetRect () const
{
	return nodeRect;
}

void NodeImageCachedContext::UpdateRect (const NUIE::IntRect& rect)
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

void NodeImageCachedContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	GetContext ().DrawLine (GetOffsettedPoint (beg), GetOffsettedPoint (end), pen);
}

void NodeImageCachedContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	GetContext ().DrawBezier (GetOffsettedPoint (p1), GetOffsettedPoint (p2), GetOffsettedPoint (p3), GetOffsettedPoint (p4), pen);
}

void NodeImageCachedContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	GetContext ().DrawRect (GetOffsettedRect (rect), pen);
}

void NodeImageCachedContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	GetContext ().FillRect (GetOffsettedRect (rect), color);
}

void NodeImageCachedContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	GetContext ().DrawEllipse (GetOffsettedRect (rect), pen);
}

void NodeImageCachedContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	GetContext ().FillEllipse (GetOffsettedRect (rect), color);
}

void NodeImageCachedContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& color)
{
	GetContext ().DrawFormattedText (GetOffsettedRect (rect), font, text, hAnchor, vAnchor, color);
}

NUIE::Size NodeImageCachedContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	return GetContext ().MeasureText (font, text);
}

NUIE::Point NodeImageCachedContext::GetOffsettedPoint (const NUIE::Point& point)
{
	return point - NUIE::Point (nodeRect.GetX (), nodeRect.GetY ());
}

NUIE::Rect NodeImageCachedContext::GetOffsettedRect (const NUIE::Rect& rect)
{
	return rect.Offset (-NUIE::Point (nodeRect.GetX (), nodeRect.GetY ()));
}
