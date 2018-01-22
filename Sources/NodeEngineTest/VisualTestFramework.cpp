#include "VisualTestFramework.hpp"
#include "MouseEventHandler.hpp"
#include "SimpleTest.hpp"

#include <iostream>
#include <fstream>
#include <cmath>

static std::wstring ReplaceAll (const std::wstring& string, const std::wstring& from, const std::wstring& to)
{
	std::wstring result = string;
	size_t found = string.find (from);
	while (found != std::wstring::npos) {
		result.replace (found, from.length (), to);
		found = result.find (from);
	}
	return result;
}

SVGBuilder::SVGBuilder ()
{
	
}

void SVGBuilder::AddTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
{
	AddTag (tag, L"", attributes);
}

void SVGBuilder::AddTag (const std::wstring& tag, const std::wstring& content, const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
{
	svgContent << L"<" << tag;
	AddAttributes (attributes);
	if (content.empty ()) {
		svgContent << L"/>" << std::endl;
	} else {
		std::wstring contentToWrite = content;
		contentToWrite = ReplaceAll (contentToWrite, L"<", L"&lt;");
		contentToWrite = ReplaceAll (contentToWrite, L">", L"&gt;");
		svgContent << L">" << contentToWrite;
		AddCloseTag (tag);
	}
}

void SVGBuilder::AddOpenTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
{
	svgContent << L"<" << tag;
	AddAttributes (attributes);
	svgContent << L">" << std::endl;
}

void SVGBuilder::AddCloseTag (const std::wstring& tag)
{
	svgContent << L"</" << tag << L">" << std::endl;
}

void SVGBuilder::Clear ()
{
	svgContent.str (std::wstring ());
}

void SVGBuilder::WriteToFile (const std::string& fileName) const
{
	std::wofstream file;
	file.open (fileName);
	file << svgContent.str ();
	file.close ();
}

std::wstring SVGBuilder::GetAsString () const
{
	return svgContent.str ();
}

std::wstring SVGBuilder::ToString (double val)
{
	return std::to_wstring ((int) ceil (val));
}

std::wstring SVGBuilder::BegToString (double val)
{
	return std::to_wstring ((int) floor (val));
}

std::wstring SVGBuilder::EndToString (double val)
{
	return std::to_wstring ((int) ceil (val));
}

std::wstring SVGBuilder::PenToStrokeStyle (const Pen& pen)
{
	std::wstring style = L"";
	style += L"stroke-width:" + ToString (pen.GetThickness ()) + L"px;";
	style += L"stroke:rgb(" + ToString (pen.GetColor ().GetR ()) + L"," + ToString (pen.GetColor ().GetG ()) + L"," + ToString (pen.GetColor ().GetB ()) + L");";
	return style;
}

std::wstring SVGBuilder::ColorToFillStyle (const Color& color)
{
	std::wstring style = L"";
	style += L"fill:rgb(" + ToString (color.GetR ()) + L"," + ToString (color.GetG ()) + L"," + ToString (color.GetB ()) + L");";
	return style;
}

std::wstring SVGBuilder::FontToFontStyle (const Font& font)
{
	std::wstring style = L"";
	style += L"font-family:" + font.GetFamily () + L";";
	style += L"font-size:" + SVGBuilder::ToString (font.GetSize ()) + L"px;";
	return style;
}

std::wstring SVGBuilder::PointToPath (const Point& point)
{
	return ToString (point.GetX ()) + L"," + ToString (point.GetY ());
}

void SVGBuilder::AddAttributes (const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
{
	for (const auto& it : attributes) {
		svgContent << L" " << it.first << L"=\"" << it.second << L"\"";
	}
}

SVGDrawingContext::SVGDrawingContext (double width, double height) :
	width (width),
	height (height)
{
	
}

void SVGDrawingContext::WriteToFile (const std::string& fileName) const
{
	svgBuilder.WriteToFile (fileName);
}

std::wstring SVGDrawingContext::GetAsString () const
{
	return svgBuilder.GetAsString ();
}

void SVGDrawingContext::Resize (int, int)
{
	
}

double SVGDrawingContext::GetWidth () const
{
	return width;
}

double SVGDrawingContext::GetHeight () const
{
	return height;
}
	
void SVGDrawingContext::BeginDraw ()
{
	svgBuilder.Clear ();
	svgBuilder.AddOpenTag (L"svg", {
		{ L"version", L"1.1" },
		{ L"xmlns", L"http://www.w3.org/2000/svg" },
		{ L"width", SVGBuilder::ToString (GetWidth ()) },
		{ L"height", SVGBuilder::ToString (GetHeight ()) },
		{ L"shape-rendering", L"crispEdges" }
	});
}

void SVGDrawingContext::EndDraw ()
{
	svgBuilder.AddCloseTag (L"svg");
}
				 
void SVGDrawingContext::DrawLine (const Point&, const Point&, const Pen&)
{
	DBGBREAK ();
}

void SVGDrawingContext::DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen)
{
	svgBuilder.AddTag (L"path", {
		{ L"d", L"M" + SVGBuilder::PointToPath (p1) + L" C" + SVGBuilder::PointToPath (p2) + L" " + SVGBuilder::PointToPath (p3) + L" " + SVGBuilder::PointToPath (p4) },
		{ L"style", L"fill:none;" + SVGBuilder::PenToStrokeStyle (pen) }
	});
}
				 
void SVGDrawingContext::DrawRect (const Rect& rect, const Pen& pen)
{
	svgBuilder.AddTag (L"rect", {
		{ L"x", SVGBuilder::BegToString (rect.GetX ()) },
		{ L"y", SVGBuilder::BegToString (rect.GetY ()) },
		{ L"width", SVGBuilder::EndToString (rect.GetWidth ()) },
		{ L"height", SVGBuilder::EndToString (rect.GetHeight ()) },
		{ L"style", L"fill:none;" + SVGBuilder::PenToStrokeStyle (pen) }
	});
}

void SVGDrawingContext::FillRect (const Rect& rect, const Color& color)
{
	svgBuilder.AddTag (L"rect", {
		{ L"x", SVGBuilder::BegToString (rect.GetX ()) },
		{ L"y", SVGBuilder::BegToString (rect.GetY ()) },
		{ L"width", SVGBuilder::EndToString (rect.GetWidth ()) },
		{ L"height", SVGBuilder::EndToString (rect.GetHeight ()) },
		{ L"style", SVGBuilder::ColorToFillStyle (color) }
	});
}
				 
void SVGDrawingContext::DrawEllipse (const Rect&, const Pen&)
{
	DBGBREAK ();
}

void SVGDrawingContext::FillEllipse (const Rect&, const Color&)
{
	DBGBREAK ();
}
				 
void SVGDrawingContext::DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const NUIE::Color& textColor)
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
		{ L"x", SVGBuilder::BegToString (x) },
		{ L"y", SVGBuilder::BegToString (y) },
		{ L"dominant-baseline", dominantBaseline },
		{ L"text-anchor", textAnchor },
		{ L"style", SVGBuilder::ColorToFillStyle (textColor) + SVGBuilder::FontToFontStyle (font) }
	});
}

Size SVGDrawingContext::MeasureText (const Font& font, const std::wstring& text)
{
	return Size (text.length () * font.GetSize (), font.GetSize () * 1.5);
}

TestEventHandlers::TestEventHandlers ()
{
	
}

CommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const CommandStructure&)
{
	return nullptr;
}

CommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodePtr&, const CommandStructure&)
{
	return nullptr;
}

CommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::OutputSlotPtr&, const CommandStructure&)
{
	return nullptr;
}

CommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::InputSlotPtr&, const CommandStructure&)
{
	return nullptr;
}

TestNodeUIEnvironment::TestNodeUIEnvironment (NodeEditor& nodeEditor) :
	NUIE::NodeUIEnvironment (),
	nodeEditor (nodeEditor),
	drawingContext (800, 600),
	skinParams (),
	eventHandlers (),
	evaluationEnv (nullptr)
{
	
}

DrawingContext& TestNodeUIEnvironment::GetDrawingContext ()
{
	return drawingContext;
}

SkinParams& TestNodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

EvaluationEnv& TestNodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void TestNodeUIEnvironment::OnValuesRecalculated ()
{

}

void TestNodeUIEnvironment::OnRedrawRequest ()
{
	nodeEditor.Draw ();
}

EventHandlers& TestNodeUIEnvironment::GetEventHandlers ()
{
	return eventHandlers;
}

const SVGDrawingContext& TestNodeUIEnvironment::GetSVGDrawingContext () const
{
	return drawingContext;
}

NodeEditorTestEnv::NodeEditorTestEnv () :
	uiEnvironment (nodeEditor),
	nodeEditor (uiEnvironment)
{
}

bool NodeEditorTestEnv::CheckReference (const std::string& referenceFileName)
{
	const SVGDrawingContext& context = uiEnvironment.GetSVGDrawingContext ();

	std::string testFilesPath = SimpleTest::GetAppFolderLocation () + "VisualTestFiles" + PATH_SEPARATOR;
	std::string referenceFilePath = testFilesPath + referenceFileName;
	std::wifstream referenceFile;
	referenceFile.open (referenceFilePath);
	if (!referenceFile.is_open ()) {
		context.WriteToFile (testFilesPath + "Current_" + referenceFileName);
		return false;
	}

	std::wstringstream referenceFileBuffer;
	referenceFileBuffer << referenceFile.rdbuf ();
	std::wstring referenceContent = referenceFileBuffer.str ();
	referenceFile.close ();

	std::wstring currentContent = context.GetAsString ();
	referenceContent = ReplaceAll (referenceContent, L"\r\n", L"\n");
	currentContent = ReplaceAll (currentContent, L"\r\n", L"\n");
	if (referenceContent != currentContent) {
		std::wcout << std::endl << L"=== CURRENT ===" << std::endl;
		std::wcout << currentContent << std::endl;
		std::wcout << L"=== REFERENCE ===" << std::endl;
		std::wcout << referenceContent << std::endl;
		context.WriteToFile (testFilesPath + "Current_" + referenceFileName);
		return false;
	}
	return true;
}

void NodeEditorTestEnv::Click (const Point& point)
{
	nodeEditor.OnMouseDown (EmptyKeySet, MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
	nodeEditor.OnMouseUp (EmptyKeySet, MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::CtrlClick (const Point& point)
{
	nodeEditor.OnMouseDown (KeySet ({ KeyCode::Control }), MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
	nodeEditor.OnMouseUp (KeySet ({ KeyCode::Control }), MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::Wheel (MouseWheelRotation rotation, const Point& point)
{
	nodeEditor.OnMouseWheel (EmptyKeySet, rotation, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::DragDrop (const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp)
{
	nodeEditor.OnMouseDown (EmptyKeySet, MouseButton::Left, (int) from.GetX (), (int) from.GetY ());
	nodeEditor.OnMouseMove (EmptyKeySet, (int) to.GetX (), (int) to.GetY ());
	if (beforeMouseUp != nullptr) {
		beforeMouseUp ();
	}
	nodeEditor.OnMouseUp (EmptyKeySet, MouseButton::Left, (int) to.GetX (), (int) to.GetY ());
}
