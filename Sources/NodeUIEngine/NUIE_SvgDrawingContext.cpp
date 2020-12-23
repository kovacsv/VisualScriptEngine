#include "NUIE_SvgDrawingContext.hpp"
#include "NE_StringUtils.hpp"

#include <cmath>

namespace NUIE
{

SvgBuilder::SvgBuilder ()
{
	
}

void SvgBuilder::AddTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
{
	AddTag (tag, std::wstring (), attributes);
}

void SvgBuilder::AddTag (const std::wstring& tag, const std::wstring& content, const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
{
	svgContent << L"<" << tag;
	AddAttributes (attributes);
	if (content.empty ()) {
		svgContent << L"/>" << std::endl;
	} else {
		std::wstring contentToWrite = content;
		contentToWrite = NE::ReplaceAll (contentToWrite, L"<", L"&lt;");
		contentToWrite = NE::ReplaceAll (contentToWrite, L">", L"&gt;");
		svgContent << L">" << contentToWrite;
		AddCloseTag (tag);
	}
}

void SvgBuilder::AddOpenTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
{
	svgContent << L"<" << tag;
	AddAttributes (attributes);
	svgContent << L">" << std::endl;
}

void SvgBuilder::AddCloseTag (const std::wstring& tag)
{
	svgContent << L"</" << tag << L">" << std::endl;
}

void SvgBuilder::Clear ()
{
	svgContent.str (std::wstring ());
}

std::wstring SvgBuilder::GetAsString () const
{
	return svgContent.str ();
}

std::wstring SvgBuilder::NumberToString (double num)
{
	return std::to_wstring ((int) std::floor (num));
}

std::wstring SvgBuilder::PenToStrokeStyle (const Pen& pen)
{
	std::wstring style;
	const Color& color = pen.GetColor ();
	style += L"stroke-width:" + NumberToString (pen.GetThickness ()) + L"px;";
	style += L"stroke:rgb(" + std::to_wstring (color.GetR ()) + L"," + std::to_wstring (color.GetG ()) + L"," + std::to_wstring (color.GetB ()) + L");";
	return style;
}

std::wstring SvgBuilder::ColorToFillStyle (const Color& color)
{
	std::wstring style;
	style += L"fill:rgb(" + std::to_wstring (color.GetR ()) + L"," + std::to_wstring (color.GetG ()) + L"," + std::to_wstring (color.GetB ()) + L");";
	return style;
}

std::wstring SvgBuilder::FontToFontStyle (const Font& font)
{
	std::wstring style;
	style += L"font-family:" + font.GetFamily () + L";";
	style += L"font-size:" + NumberToString (font.GetSize ()) + L"px;";
	return style;
}

std::wstring SvgBuilder::PointToPath (const Point& point)
{
	IntPoint intPoint (point);
	return std::to_wstring (intPoint.GetX ()) + L"," + std::to_wstring (intPoint.GetY ());
}

void SvgBuilder::AddAttributes (const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
{
	for (const auto& it : attributes) {
		svgContent << L" " << it.first << L"=\"" << it.second << L"\"";
	}
}

SvgDrawingContext::SvgDrawingContext (int width, int height) :
	width (width),
	height (height)
{
	
}

std::wstring SvgDrawingContext::GetAsString () const
{
	return svgBuilder.GetAsString ();
}

void SvgDrawingContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}

int SvgDrawingContext::GetWidth () const
{
	return width;
}

int SvgDrawingContext::GetHeight () const
{
	return height;
}
	
void SvgDrawingContext::BeginDraw ()
{
	svgBuilder.Clear ();
	svgBuilder.AddOpenTag (L"svg", {
		{ L"version", L"1.1" },
		{ L"xmlns", L"http://www.w3.org/2000/svg" },
		{ L"width", std::to_wstring (GetWidth ()) },
		{ L"height", std::to_wstring (GetHeight ()) },
		{ L"shape-rendering", L"crispEdges" }
	});
}

void SvgDrawingContext::EndDraw ()
{
	svgBuilder.AddCloseTag (L"svg");
}
			
bool SvgDrawingContext::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void SvgDrawingContext::DrawLine (const Point& beg, const Point& end, const Pen& pen)
{
	IntPoint intBeg (beg);
	IntPoint intEnd (end);
	svgBuilder.AddTag (L"line", {
		{ L"x1", std::to_wstring (intBeg.GetX ()) },
		{ L"y1", std::to_wstring (intBeg.GetY ()) },
		{ L"x2", std::to_wstring (intEnd.GetX ()) },
		{ L"y2", std::to_wstring (intEnd.GetY ()) },
		{ L"style", L"fill:none;" + SvgBuilder::PenToStrokeStyle (pen) }
	});
}

void SvgDrawingContext::DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen)
{
	svgBuilder.AddTag (L"path", {
		{ L"d", L"M" + SvgBuilder::PointToPath (p1) + L" C" + SvgBuilder::PointToPath (p2) + L" " + SvgBuilder::PointToPath (p3) + L" " + SvgBuilder::PointToPath (p4) },
		{ L"style", L"fill:none;" + SvgBuilder::PenToStrokeStyle (pen) }
	});
}
				 
void SvgDrawingContext::DrawRect (const Rect& rect, const Pen& pen)
{
	IntRect intRect (rect);
	svgBuilder.AddTag (L"rect", {
		{ L"x", std::to_wstring (intRect.GetLeft ()) },
		{ L"y", std::to_wstring (intRect.GetTop ()) },
		{ L"width", std::to_wstring (intRect.GetWidth ()) },
		{ L"height", std::to_wstring (intRect.GetHeight ()) },
		{ L"style", L"fill:none;" + SvgBuilder::PenToStrokeStyle (pen) }
	});
}

void SvgDrawingContext::FillRect (const Rect& rect, const Color& color)
{
	IntRect intRect (rect);
	svgBuilder.AddTag (L"rect", {
		{ L"x", std::to_wstring (intRect.GetLeft ()) },
		{ L"y", std::to_wstring (intRect.GetTop ()) },
		{ L"width", std::to_wstring (intRect.GetWidth ()) },
		{ L"height", std::to_wstring (intRect.GetHeight ()) },
		{ L"style", SvgBuilder::ColorToFillStyle (color) }
	});
}
				 
void SvgDrawingContext::DrawEllipse (const Rect& rect, const Pen& pen)
{
	IntRect intRect (rect);
	svgBuilder.AddTag (L"ellipse", {
		{ L"cx", std::to_wstring (intRect.GetCenter ().GetX ()) },
		{ L"cy", std::to_wstring (intRect.GetCenter ().GetY ()) },
		{ L"rx", std::to_wstring (intRect.GetWidth () / 2) },
		{ L"ry", std::to_wstring (intRect.GetHeight () / 2) },
		{ L"style", L"fill:none;" + SvgBuilder::PenToStrokeStyle (pen) }
	});
}

void SvgDrawingContext::FillEllipse (const Rect& rect, const Color& color)
{
	IntRect intRect (rect);
	svgBuilder.AddTag (L"ellipse", {
		{ L"cx", std::to_wstring (intRect.GetCenter ().GetX ()) },
		{ L"cy", std::to_wstring (intRect.GetCenter ().GetY ()) },
		{ L"rx", std::to_wstring (intRect.GetWidth () / 2) },
		{ L"ry", std::to_wstring (intRect.GetHeight () / 2) },
		{ L"style", SvgBuilder::ColorToFillStyle (color) }
	});
}
				 
void SvgDrawingContext::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& textColor)
{
	std::wstring textAnchor;
	std::wstring dominantBaseline;
	double x = 0.0;
	double y = 0.0;

	switch (hAnchor) {
	case HorizontalAnchor::Left:
		textAnchor = L"start";
		x = rect.GetLeft ();
		break;
	case HorizontalAnchor::Center:
		textAnchor = L"middle";
		x = rect.GetCenter ().GetX ();
		break;
	case HorizontalAnchor::Right:
		textAnchor = L"end";
		x = rect.GetRight ();
		break;
	}

	switch (vAnchor) {
	case VerticalAnchor::Top:
		dominantBaseline = L"text-before-edge";
		y = rect.GetTop ();
		break;
	case VerticalAnchor::Center:
		dominantBaseline = L"central";
		y = rect.GetCenter ().GetY ();
		break;
	case VerticalAnchor::Bottom:
		dominantBaseline = L"text-after-edge";
		y = rect.GetBottom ();
		break;
	}

	svgBuilder.AddTag (L"text", text, {
		{ L"x", SvgBuilder::NumberToString (x) },
		{ L"y", SvgBuilder::NumberToString (y) },
		{ L"dominant-baseline", dominantBaseline },
		{ L"text-anchor", textAnchor },
		{ L"style", SvgBuilder::ColorToFillStyle (textColor) + SvgBuilder::FontToFontStyle (font) }
	});
}

Size SvgDrawingContext::MeasureText (const Font& font, const std::wstring& text)
{
	return Size (text.length () * font.GetSize (), font.GetSize () * 1.5);
}

bool SvgDrawingContext::CanDrawIcon ()
{
	return false;
}

void SvgDrawingContext::DrawIcon (const Rect&, const IconId&)
{
	DBGBREAK ();
}

}
