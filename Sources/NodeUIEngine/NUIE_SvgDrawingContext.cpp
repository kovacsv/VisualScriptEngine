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

std::wstring SvgBuilder::ToString (double val)
{
	return std::to_wstring ((int) ceil (val));
}

std::wstring SvgBuilder::BegToString (double val)
{
	return std::to_wstring ((int) floor (val));
}

std::wstring SvgBuilder::EndToString (double val)
{
	return std::to_wstring ((int) ceil (val));
}

std::wstring SvgBuilder::PenToStrokeStyle (const Pen& pen)
{
	std::wstring style;
	style += L"stroke-width:" + ToString (pen.GetThickness ()) + L"px;";
	style += L"stroke:rgb(" + ToString (pen.GetColor ().GetR ()) + L"," + ToString (pen.GetColor ().GetG ()) + L"," + ToString (pen.GetColor ().GetB ()) + L");";
	return style;
}

std::wstring SvgBuilder::ColorToFillStyle (const Color& color)
{
	std::wstring style;
	style += L"fill:rgb(" + ToString (color.GetR ()) + L"," + ToString (color.GetG ()) + L"," + ToString (color.GetB ()) + L");";
	return style;
}

std::wstring SvgBuilder::FontToFontStyle (const Font& font)
{
	std::wstring style;
	style += L"font-family:" + font.GetFamily () + L";";
	style += L"font-size:" + SvgBuilder::ToString (font.GetSize ()) + L"px;";
	return style;
}

std::wstring SvgBuilder::PointToPath (const Point& point)
{
	return ToString (point.GetX ()) + L"," + ToString (point.GetY ());
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
		{ L"width", SvgBuilder::ToString (GetWidth ()) },
		{ L"height", SvgBuilder::ToString (GetHeight ()) },
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
	svgBuilder.AddTag (L"line", {
		{ L"x1", SvgBuilder::ToString (beg.GetX ()) },
		{ L"y1", SvgBuilder::ToString (beg.GetY ()) },
		{ L"x2", SvgBuilder::ToString (end.GetX ()) },
		{ L"y2", SvgBuilder::ToString (end.GetY ()) },
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
	svgBuilder.AddTag (L"rect", {
		{ L"x", SvgBuilder::BegToString (rect.GetX ()) },
		{ L"y", SvgBuilder::BegToString (rect.GetY ()) },
		{ L"width", SvgBuilder::EndToString (rect.GetWidth ()) },
		{ L"height", SvgBuilder::EndToString (rect.GetHeight ()) },
		{ L"style", L"fill:none;" + SvgBuilder::PenToStrokeStyle (pen) }
	});
}

void SvgDrawingContext::FillRect (const Rect& rect, const Color& color)
{
	svgBuilder.AddTag (L"rect", {
		{ L"x", SvgBuilder::BegToString (rect.GetX ()) },
		{ L"y", SvgBuilder::BegToString (rect.GetY ()) },
		{ L"width", SvgBuilder::EndToString (rect.GetWidth ()) },
		{ L"height", SvgBuilder::EndToString (rect.GetHeight ()) },
		{ L"style", SvgBuilder::ColorToFillStyle (color) }
	});
}
				 
void SvgDrawingContext::DrawEllipse (const Rect& rect, const Pen& pen)
{
	svgBuilder.AddTag (L"ellipse", {
		{ L"cx", SvgBuilder::BegToString (rect.GetCenter ().GetX ()) },
		{ L"cy", SvgBuilder::BegToString (rect.GetCenter ().GetY ()) },
		{ L"rx", SvgBuilder::EndToString (rect.GetWidth () / 2.0) },
		{ L"ry", SvgBuilder::EndToString (rect.GetHeight () / 2.0) },
		{ L"style", L"fill:none;" + SvgBuilder::PenToStrokeStyle (pen) }
	});
}

void SvgDrawingContext::FillEllipse (const Rect& rect, const Color& color)
{
	svgBuilder.AddTag (L"ellipse", {
		{ L"cx", SvgBuilder::BegToString (rect.GetCenter ().GetX ()) },
		{ L"cy", SvgBuilder::BegToString (rect.GetCenter ().GetY ()) },
		{ L"rx", SvgBuilder::EndToString (rect.GetWidth () / 2.0) },
		{ L"ry", SvgBuilder::EndToString (rect.GetHeight () / 2.0) },
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
		{ L"x", SvgBuilder::BegToString (x) },
		{ L"y", SvgBuilder::BegToString (y) },
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
