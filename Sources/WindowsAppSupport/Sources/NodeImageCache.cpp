#include "NodeImageCache.hpp"

NodeImageCachedContext::NodeImageCachedContext (const NE::Checksum& checksum, const NUIE::IntRect& nodeRect) :
	checksum (checksum),
	nodeRect (nodeRect),
	isUpToDate (false),
	context (nodeRect.GetWidth (), nodeRect.GetHeight ())
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

BitmapContextGdi* NodeImageCachedContext::GetContext ()
{
	return &context;
}

void NodeImageCachedContext::Resize (int newWidth, int newHeight)
{
	return context.Resize (newWidth, newHeight);
}

double NodeImageCachedContext::GetWidth () const
{
	return context.GetWidth ();
}

double NodeImageCachedContext::GetHeight () const
{
	return context.GetHeight ();
}

void NodeImageCachedContext::BeginDraw ()
{
	context.BeginDraw ();
}

void NodeImageCachedContext::EndDraw ()
{
	context.EndDraw ();
}

void NodeImageCachedContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	context.DrawLine (GetOffsettedPoint (beg), GetOffsettedPoint (end), pen);
}

void NodeImageCachedContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	context.DrawBezier (GetOffsettedPoint (p1), GetOffsettedPoint (p2), GetOffsettedPoint (p3), GetOffsettedPoint (p4), pen);
}

void NodeImageCachedContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	context.DrawRect (GetOffsettedRect (rect), pen);
}

void NodeImageCachedContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	context.FillRect (GetOffsettedRect (rect), color);
}

void NodeImageCachedContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	context.DrawEllipse (GetOffsettedRect (rect), pen);
}

void NodeImageCachedContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	context.FillEllipse (GetOffsettedRect (rect), color);
}

void NodeImageCachedContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& color)
{
	context.DrawFormattedText (GetOffsettedRect (rect), font, text, hAnchor, vAnchor, color);
}

NUIE::Size NodeImageCachedContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	return context.MeasureText (font, text);
}

NUIE::Point NodeImageCachedContext::GetOffsettedPoint (const NUIE::Point& point)
{
	return point - NUIE::Point (nodeRect.GetX (), nodeRect.GetY ());
}

NUIE::Rect NodeImageCachedContext::GetOffsettedRect (const NUIE::Rect& rect)
{
	return rect.Offset (-NUIE::Point (nodeRect.GetX (), nodeRect.GetY ()));
}

NodeImageCache::NodeImageCache ()
{

}

NodeImageCache::~NodeImageCache ()
{

}

NodeImageCachedContext* NodeImageCache::GetImageCacheData (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::IntRect& rect)
{
	auto found = cache.find (nodeId);
	if (found != cache.end ()) {
		ImageCachedContextPtr& cacheData = found->second;
		if (cacheData->IsUpToDate (checksum)) {
			cacheData->UpdateRect (rect);
			return cacheData.get ();
		} else {
			cache.erase (found);
		}
	}
	return Insert (nodeId, checksum, rect);
}

NodeImageCachedContext* NodeImageCache::Insert (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::IntRect& rect)
{
	ImageCachedContextPtr cacheData (new NodeImageCachedContext (checksum, rect));
	cache.insert ({ nodeId, cacheData });
	return cacheData.get ();
}
