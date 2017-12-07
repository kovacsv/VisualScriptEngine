#include "NodeBitmapContextGdi.hpp"
#include "Debug.hpp"

NodeBitmapContextGdi::NodeBitmapContextGdi (int width, int height) :
	bitmapContext (width, height),
	currentNodeCacheData (nullptr)
{
}

NodeBitmapContextGdi::~NodeBitmapContextGdi ()
{
}

void NodeBitmapContextGdi::Resize (int newWidth, int newHeight)
{
	bitmapContext.Resize (newWidth, newHeight);
}

double NodeBitmapContextGdi::GetWidth () const
{
	return bitmapContext.GetWidth ();
}

double NodeBitmapContextGdi::GetHeight () const
{
	return bitmapContext.GetHeight ();
}

void NodeBitmapContextGdi::BeginDraw ()
{

}

void NodeBitmapContextGdi::EndDraw ()
{

}

void NodeBitmapContextGdi::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	ChooseContext ()->DrawLine (beg, end, pen);
}

void NodeBitmapContextGdi::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	ChooseContext ()->DrawBezier (p1, p2, p3, p4, pen);
}

void NodeBitmapContextGdi::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	ChooseContext ()->DrawRect (rect, pen);
}

void NodeBitmapContextGdi::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	ChooseContext ()->FillRect (rect, color);
}

void NodeBitmapContextGdi::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	ChooseContext ()->DrawEllipse (rect, pen);
}

void NodeBitmapContextGdi::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	ChooseContext ()->FillEllipse (rect, color);
}

void NodeBitmapContextGdi::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& color)
{
	ChooseContext ()->DrawFormattedText (rect, font, text, hAnchor, vAnchor, color);
}

NUIE::Size NodeBitmapContextGdi::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	return bitmapContext.MeasureText (font, text);
}

void NodeBitmapContextGdi::OnNodeDrawingBegin (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::Rect& rect)
{
	currentNodeCacheData = nodeImageCache.GetImageCacheData (nodeId, checksum, rect);
}

void NodeBitmapContextGdi::OnNodeDrawingEnd (const NE::NodeId& /*nodeId*/)
{
	if (currentNodeCacheData != nullptr) {
		currentNodeCacheData->SetUpToDate (true);
		const NUIE::IntRect& nodeRect = currentNodeCacheData->GetRect ();
		currentNodeCacheData->GetTypedContext ()->DrawToContext (bitmapContext, nodeRect.GetX (), nodeRect.GetY ());
	}
	currentNodeCacheData = nullptr;
}

void NodeBitmapContextGdi::DrawToHDC (HDC hdc, int x, int y)
{
	bitmapContext.DrawToHDC (hdc, x, y);
}

NUIE::DrawingContext* NodeBitmapContextGdi::ChooseContext ()
{
	if (currentNodeCacheData != nullptr) {
		if (currentNodeCacheData->IsUpToDate ()) {
			static NUIE::NullDrawingContext nullContext;
			return &nullContext;
		}
		return currentNodeCacheData;
	}
	return &bitmapContext;
}
