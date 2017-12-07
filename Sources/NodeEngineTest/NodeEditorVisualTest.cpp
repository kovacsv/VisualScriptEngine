#include "SimpleTest.hpp"
#include "NodeEditor.hpp"
#include "InputUINodes.hpp"
#include "ViewerUINodes.hpp"

#include <fstream>
#include <sstream>
#include <cmath>

using namespace NE;
using namespace NUIE;

namespace NodeEditorVisualTest
{

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

class SVGBuilder
{
public:
	SVGBuilder ()
	{
	
	}

	void AddTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
	{
		AddTag (tag, L"", attributes);
	}

	void AddTag (const std::wstring& tag, const std::wstring& content, const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
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

	void AddOpenTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
	{
		svgContent << L"<" << tag;
		AddAttributes (attributes);
		svgContent << L">" << std::endl;
	}

	void AddCloseTag (const std::wstring& tag)
	{
		svgContent << L"</" << tag << L">" << std::endl;
	}

	void Clear ()
	{
		svgContent.str (std::wstring ());
	}

	void WriteToFile (const std::string& fileName) const
	{
		std::wofstream file;
		file.open (fileName);
		file << svgContent.str ();
		file.close ();
	}

	std::wstring GetAsString () const
	{
		return svgContent.str ();
	}

	static std::wstring ToString (double val)
	{
		return std::to_wstring ((int) ceil (val));
	}

	static std::wstring BegToString (double val)
	{
		return std::to_wstring ((int) floor (val));
	}

	static std::wstring EndToString (double val)
	{
		return std::to_wstring ((int) ceil (val));
	}

	static std::wstring PenToStrokeStyle (const Pen& pen)
	{
		std::wstring style = L"";
		style += L"stroke-width:" + ToString (pen.GetThickness ()) + L"px;";
		style += L"stroke:rgb(" + ToString (pen.GetColor ().GetR ()) + L"," + ToString (pen.GetColor ().GetG ()) + L"," + ToString (pen.GetColor ().GetB ()) + L");";
		return style;
	}

	static std::wstring ColorToFillStyle (const Color& color)
	{
		std::wstring style = L"";
		style += L"fill:rgb(" + ToString (color.GetR ()) + L"," + ToString (color.GetG ()) + L"," + ToString (color.GetB ()) + L");";
		return style;
	}

	static std::wstring FontToFontStyle (const Font& font)
	{
		std::wstring style = L"";
		style += L"font-family:" + font.GetFamily () + L";";
		style += L"font-size:" + SVGBuilder::ToString (font.GetSize ()) + L"px;";
		return style;
	}

	static std::wstring PointToPath (const Point& point)
	{
		return ToString (point.GetX ()) + L"," + ToString (point.GetY ());
	}

private:
	void AddAttributes (const std::vector<std::pair<std::wstring, std::wstring>>& attributes)
	{
		for (const auto& it : attributes) {
			svgContent << L" " << it.first << L"=\"" << it.second << L"\"";
		}
	}

	std::wostringstream svgContent;
};

class SVGDrawingContext : public NodeDrawingContext
{
public:
	SVGDrawingContext (double width, double height) :
		width (width),
		height (height)
	{
	
	}

	void WriteToFile (const std::string& fileName) const
	{
		svgBuilder.WriteToFile (fileName);
	}

	std::wstring GetAsString () const
	{
		return svgBuilder.GetAsString ();
	}

	virtual void Resize (int, int) override
	{
	
	}

	virtual double GetWidth () const override
	{
		return width;
	}

	virtual double GetHeight () const override
	{
		return height;
	}
	
	virtual void BeginDraw () override
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

	virtual void EndDraw () override
	{
		svgBuilder.AddCloseTag (L"svg");
	}
				 
	virtual void DrawLine (const Point&, const Point&, const Pen&) override
	{
		DBGBREAK ();
	}

	virtual void DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override
	{
		svgBuilder.AddTag (L"path", {
			{ L"d", L"M" + SVGBuilder::PointToPath (p1) + L" C" + SVGBuilder::PointToPath (p2) + L" " + SVGBuilder::PointToPath (p3) + L" " + SVGBuilder::PointToPath (p4) },
			{ L"style", L"fill:none;" + SVGBuilder::PenToStrokeStyle (pen) }
		});
	}
				 
	virtual void DrawRect (const Rect& rect, const Pen& pen) override
	{
		svgBuilder.AddTag (L"rect", {
			{ L"x", SVGBuilder::BegToString (rect.GetX ()) },
			{ L"y", SVGBuilder::BegToString (rect.GetY ()) },
			{ L"width", SVGBuilder::EndToString (rect.GetWidth ()) },
			{ L"height", SVGBuilder::EndToString (rect.GetHeight ()) },
			{ L"style", L"fill:none;" + SVGBuilder::PenToStrokeStyle (pen) }
		});
	}

	virtual void FillRect (const Rect& rect, const Color& color) override
	{
		svgBuilder.AddTag (L"rect", {
			{ L"x", SVGBuilder::BegToString (rect.GetX ()) },
			{ L"y", SVGBuilder::BegToString (rect.GetY ()) },
			{ L"width", SVGBuilder::EndToString (rect.GetWidth ()) },
			{ L"height", SVGBuilder::EndToString (rect.GetHeight ()) },
			{ L"style", SVGBuilder::ColorToFillStyle (color) }
		});
	}
				 
	virtual void DrawEllipse (const Rect&, const Pen&) override
	{
		DBGBREAK ();
	}

	virtual void FillEllipse (const Rect&, const Color&) override
	{
		DBGBREAK ();
	}
				 
	virtual void DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color) override
	{
		std::wstring textAnchor;
		std::wstring dominantBaseline;
		double x = 0.0;
		double y = 0.0;

		switch (hAnchor) {
		case NUIE::HorizontalAnchor::Left:
			textAnchor = L"start";
			x = rect.GetLeft ();
			break;
		case NUIE::HorizontalAnchor::Center:
			textAnchor = L"middle";
			x = rect.GetCenter ().GetX ();
			break;
		case NUIE::HorizontalAnchor::Right:
			textAnchor = L"end";
			x = rect.GetRight ();
			break;
		}

		switch (vAnchor) {
		case NUIE::VerticalAnchor::Top:
			dominantBaseline = L"text-before-edge";
			y = rect.GetTop ();
			break;
		case NUIE::VerticalAnchor::Center:
			dominantBaseline = L"central";
			y = rect.GetCenter ().GetY ();
			break;
		case NUIE::VerticalAnchor::Bottom:
			dominantBaseline = L"text-after-edge";
			y = rect.GetBottom ();
			break;
		}

		svgBuilder.AddTag (L"text", text, {
			{ L"x", SVGBuilder::BegToString (x) },
			{ L"y", SVGBuilder::BegToString (y) },
			{ L"dominant-baseline", dominantBaseline },
			{ L"text-anchor", textAnchor },
			{ L"style", SVGBuilder::ColorToFillStyle (color) + SVGBuilder::FontToFontStyle (font) }
		});
	}

	virtual Size MeasureText (const Font& font, const std::wstring& text) override
	{
		return Size (text.length () * font.GetSize (), font.GetSize () * 1.5);
	}

	void OnNodeDrawingBegin (const NodeId&, const NE::Checksum&, const NUIE::Rect&) override
	{
		
	}

	virtual  void OnNodeDrawingEnd (const NodeId&) override
	{
		
	}

private:
	SVGBuilder	svgBuilder;
	double		width;
	double		height;
};

class TestEventHandlers : public EventHandlers
{
public:
	TestEventHandlers ()
	{
	
	}

	virtual void OnRecalculate () override
	{
	
	}

	virtual void OnRedraw () override
	{
	
	}

	virtual NUIE::CommandPtr OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const CommandStructure&) override
	{
		return nullptr;
	}

	virtual NUIE::CommandPtr OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodePtr&, const CommandStructure&) override
	{
		return nullptr;
	}

	virtual NUIE::CommandPtr OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::OutputSlotPtr&, const CommandStructure&) override
	{
		return nullptr;
	}

	virtual NUIE::CommandPtr OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::InputSlotPtr&, const CommandStructure&) override
	{
		return nullptr;
	}
};


class TestNodeEditorInterface : public NodeEditorInterface
{
public:
	TestNodeEditorInterface () :
		drawingContext (800, 600),
		eventHandlers (),
		evaluationEnv (nullptr)
	{
	
	}

	const SVGDrawingContext& GetSVGDrawingContext () const
	{
		return drawingContext;
	}

	virtual NodeDrawingContext& GetDrawingContext () override
	{
		return drawingContext;
	}

	virtual EventHandlers& GetEventHandlers () override
	{
		return eventHandlers;
	}

	virtual EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
	}

private:
	SVGDrawingContext	drawingContext;
	TestEventHandlers	eventHandlers;
	EvaluationEnv		evaluationEnv;
};

class NodeEditorTestEnv
{
public:
	NodeEditorTestEnv () :
		nodeEditorInterface (),
		nodeEditor (nodeEditorInterface)
	{
	}

	bool CheckReference (const std::string& referenceFileName)
	{
		const SVGDrawingContext& context = nodeEditorInterface.GetSVGDrawingContext ();

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
			context.WriteToFile (testFilesPath + "Current_" + referenceFileName);
			return false;
		}
		return true;
	}

	void Click (const Point& point)
	{
		nodeEditor.OnMouseDown (EmptyKeySet, MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
		nodeEditor.OnMouseUp (EmptyKeySet, MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
	}

	void CtrlClick (const Point& point)
	{
		nodeEditor.OnMouseDown (KeySet ({ KeyCode::Control }), MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
		nodeEditor.OnMouseUp (KeySet ({ KeyCode::Control }), MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
	}

	void Wheel (MouseWheelRotation rotation, const Point& point)
	{
		nodeEditor.OnMouseWheel (EmptyKeySet, rotation, (int) point.GetX (), (int) point.GetY ());
	}

	void DragDrop (const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp = nullptr)
	{
		nodeEditor.OnMouseDown (EmptyKeySet, MouseButton::Left, (int) from.GetX (), (int) from.GetY ());
		nodeEditor.OnMouseMove (EmptyKeySet, (int) to.GetX (), (int) to.GetY ());
		if (beforeMouseUp != nullptr) {
			beforeMouseUp ();
		}
		nodeEditor.OnMouseUp (EmptyKeySet, MouseButton::Left, (int) to.GetX (), (int) to.GetY ());
	}

	TestNodeEditorInterface		nodeEditorInterface;
	NodeEditor					nodeEditor;
};

class SimpleNodeEditorTestEnv : public NodeEditorTestEnv
{
public:
	SimpleNodeEditorTestEnv () :
		NodeEditorTestEnv ()
	{
		NodeUIManager& uiManager = nodeEditor.GetNodeUIManager ();

		integerInputNode = uiManager.AddNode (NUIE::UINodePtr (new IntegerUpDownUINode (L"Integer", NUIE::Point (100, 200), 20, 10)), nodeEditorInterface.GetEvaluationEnv ());
		rangeInputNode = uiManager.AddNode (NUIE::UINodePtr (new IntegerRangeNode (L"Range", NUIE::Point (300, 400))), nodeEditorInterface.GetEvaluationEnv ());
		viewerUINode1 = uiManager.AddNode (NUIE::UINodePtr (new MultiLineViewerUINode (L"Viewer", NUIE::Point (600, 100), 5)), nodeEditorInterface.GetEvaluationEnv ());
		viewerUINode2 = uiManager.AddNode (NUIE::UINodePtr (new MultiLineViewerUINode (L"Viewer 2", NUIE::Point (600, 400), 5)), nodeEditorInterface.GetEvaluationEnv ());
		nodeEditor.Update ();

		pointInBackground = Point (5.0, 5.0);
		integerInputRect = integerInputNode->GetNodeRect (nodeEditor.GetNodeUIEnvironment ());
		rangeInputRect = rangeInputNode->GetNodeRect (nodeEditor.GetNodeUIEnvironment ());
		viewer1InputRect = viewerUINode1->GetNodeRect (nodeEditor.GetNodeUIEnvironment ());
		viewer2InputRect = viewerUINode2->GetNodeRect (nodeEditor.GetNodeUIEnvironment ());
	}

	UINodePtr	integerInputNode;
	UINodePtr	rangeInputNode;
	UINodePtr	viewerUINode1;
	UINodePtr	viewerUINode2;

	Point		pointInBackground;
	Rect		integerInputRect;
	Rect		rangeInputRect;
	Rect		viewer1InputRect;
	Rect		viewer2InputRect;
};

TEST (SelectionTest)
{
	SimpleNodeEditorTestEnv env;
	
	{ // deselect all
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference ("01_Selection_NoSelection.svg"));
	}

	{ // select one node by clicking on the header
		Point integerInputHeaderPoint = env.integerInputRect.GetTopCenter () + Point (5.0, 5.0);
		env.Click (integerInputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_IntegerNodeSelected.svg"));
	}

	{ // deselect one node by clicking on the header again
		Point integerInputHeaderPoint = env.integerInputRect.GetTopCenter () + Point (5.0, 5.0);
		env.Click (integerInputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_NoSelection.svg"));
	}
	
	{ // select node with selection rect
		Point rectSelectStart = env.rangeInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd (env.viewer2InputRect.GetCenter ().GetX (), env.rangeInputRect.GetBottom () + 10.0);
		env.DragDrop (rectSelectStart, rectSelectEnd, [&] () {
			ASSERT (env.CheckReference ("01_Selection_SelectionRect.svg"));
		});
		ASSERT (env.CheckReference ("01_Selection_RangeNodeSelected.svg"));
	}

	{ // deselect all
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference ("01_Selection_NoSelection.svg"));
	}

	{ // select two nodes with selection rect
		Point rectSelectStart = env.rangeInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd (env.viewer2InputRect.GetRight () + 10.0, env.rangeInputRect.GetBottom () + 10.0);
		env.DragDrop (rectSelectStart, rectSelectEnd, [&] () {
			ASSERT (env.CheckReference ("01_Selection_SelectionRect2.svg"));
		});
		ASSERT (env.CheckReference ("01_Selection_RangeAndViewer2Selected.svg"));
	}

	{ // select another node by clicking on the header
		Point integerInputHeaderPoint = env.integerInputRect.GetTopCenter () + Point (5.0, 5.0);
		env.Click (integerInputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_IntegerNodeSelected.svg"));
	}

	{ // append another node to selection
		Point viewer1InputHeaderPoint = env.viewer1InputRect.GetTopCenter () + Point (5.0, 5.0);
		env.CtrlClick (viewer1InputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_IntegerAndViewer1Selected.svg"));
	}

	{ // remove the other node from selection
		Point viewer1InputHeaderPoint = env.viewer1InputRect.GetTopCenter () + Point (5.0, 5.0);
		env.CtrlClick (viewer1InputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_IntegerNodeSelected.svg"));
	}

	{ // deselect all
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference ("01_Selection_NoSelection.svg"));
	}

	{ // select two nodes with selection rect
		Point rectSelectStart = env.rangeInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd (env.viewer2InputRect.GetRight () + 10.0, env.rangeInputRect.GetBottom () + 10.0);
		env.DragDrop (rectSelectStart, rectSelectEnd);
		ASSERT (env.CheckReference ("01_Selection_RangeAndViewer2Selected.svg"));
	}

	{ // select node with selection rect
		Point rectSelectStart = env.integerInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd = env.integerInputRect.GetBottomRight () + Point (10.0, 10.0);
		env.nodeEditor.OnMouseDown (KeySet ({ KeyCode::Control }), MouseButton::Left, (int) rectSelectStart.GetX (), (int) rectSelectStart.GetY ());
		env.nodeEditor.OnMouseMove (KeySet ({ KeyCode::Control }), (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		env.nodeEditor.OnMouseUp (KeySet ({ KeyCode::Control }), MouseButton::Left, (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		ASSERT (env.CheckReference ("01_Selection_IntegerRangeAndViewer2Selected.svg"));
	}

	{ // move the three nodes together
		Point rangeInputHeaderPoint = env.rangeInputRect.GetTopCenter () + Point (5.0, 5.0);
		Point targetPoint = rangeInputHeaderPoint + Point (50.0, 70.0);
		env.DragDrop (rangeInputHeaderPoint, targetPoint);
		ASSERT (env.CheckReference ("01_Selection_IntegerRangeAndViewer2Moved.svg"));
	}
}

TEST (SlotConnectionTest)
{
	SimpleNodeEditorTestEnv env;

	Point integerOutputSlotPosition = env.integerInputNode->GetOutputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("out")).GetCenter ();
	Point rangeOutputSlotPosition = env.rangeInputNode->GetOutputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("out")).GetCenter ();
	Rect viewer1InputSlotRect = env.viewerUINode1->GetInputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("in"));
	Point viewer1InputSlotPosition = viewer1InputSlotRect.GetCenter ();
	Point viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("in")).GetCenter ();
	Point rangeStartInputSlotPosition = env.rangeInputNode->GetInputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("start")).GetCenter ();

	ASSERT (env.CheckReference ("02_SlotConnection_Basic.svg"));

	{ // start connecting integer output slot without target
		Point targetPosition = integerOutputSlotPosition + Point (200.0, -100.0);
		env.DragDrop (integerOutputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_DraggingIntegerOutput.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_Basic.svg"));
	}

	{ // start connecting viewer1 input slot without target
		Point targetPosition = viewer1InputSlotPosition - Point (200.0, -100.0);
		env.DragDrop (viewer1InputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_DraggingViewer1Input.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_Basic.svg"));
	}

	{ // connect integer output slot to viewer1 input slot
		Point targetPos = viewer1InputSlotRect.GetLeftCenter () - Point (5.0, 0.0);
		env.DragDrop (integerOutputSlotPosition, targetPos, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_ConnectingIntegerToViewer1.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_IntegerConnectedToViewer1.svg"));
	}

	{ // start connecting integer output slot without target again
		Point targetPosition = integerOutputSlotPosition + Point (200.0, -100.0);
		env.DragDrop (integerOutputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_DraggingConnectedViewer1Input.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_IntegerConnectedToViewer1.svg"));
	}

	{ // connect viewer2 input slot to range output slot
		env.DragDrop (viewer2InputSlotPosition, rangeOutputSlotPosition, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_ConnectingViewer2ToRange.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_AllViewersConnected.svg"));
	}

	{ // connect integer output slot to range start slot
		env.DragDrop (integerOutputSlotPosition, rangeStartInputSlotPosition);
		ASSERT (env.CheckReference ("02_SlotConnection_AllConnected.svg"));
	}

	viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("in")).GetCenter ();
	{ // connect integer output slot to viewer2 input slot
		env.DragDrop (integerOutputSlotPosition, viewer2InputSlotPosition, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_ConnectingIntegerToViewer2.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_IntegerToViewer2Connected.svg"));
	}
}

TEST (PanAndZoomTest)
{
	SimpleNodeEditorTestEnv env;
	ASSERT (env.CheckReference ("03_PanAndZoom_Basic.svg"));

	Point integerOutputSlotPosition = env.integerInputNode->GetOutputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("out")).GetCenter ();
	Point rangeStartInputSlotPosition = env.rangeInputNode->GetInputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("start")).GetCenter ();
	Point rangeOutputSlotPosition = env.rangeInputNode->GetOutputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("out")).GetCenter ();
	Point viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.nodeEditor.GetNodeUIEnvironment (), SlotId ("in")).GetCenter ();
	{
		env.DragDrop (integerOutputSlotPosition, rangeStartInputSlotPosition);	
		env.DragDrop (rangeOutputSlotPosition, viewer2InputSlotPosition);
		ASSERT (env.CheckReference ("03_PanAndZoom_Connections.svg"));
	}

	Point panStartPoint = env.rangeInputNode->GetNodeRect (env.nodeEditor.GetNodeUIEnvironment ()).GetTopLeft ();
	Point panEndPoint = panStartPoint + Point (50.0, -50.0);

	{
		env.nodeEditor.OnMouseDown (EmptyKeySet, MouseButton::Middle, (int) panStartPoint.GetX (), (int) panStartPoint.GetY ());
		env.nodeEditor.OnMouseMove (EmptyKeySet, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
		env.nodeEditor.OnMouseUp (EmptyKeySet, MouseButton::Middle, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
		ASSERT (env.CheckReference ("03_PanAndZoom_Panned.svg"));
	}

	for (size_t i = 0; i < 5; i++) {
		env.nodeEditor.OnMouseWheel (EmptyKeySet, MouseWheelRotation::Forward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference ("03_PanAndZoom_ZoomedIn.svg"));

	for (size_t i = 0; i < 10; i++) {
		env.nodeEditor.OnMouseWheel (EmptyKeySet, MouseWheelRotation::Forward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference ("03_PanAndZoom_ZoomedIn2.svg"));

	for (size_t i = 0; i < 20; i++) {
		env.nodeEditor.OnMouseWheel (EmptyKeySet, MouseWheelRotation::Backward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference ("03_PanAndZoom_ZoomedOut.svg"));

	for (size_t i = 0; i < 10; i++) {
		env.nodeEditor.OnMouseWheel (EmptyKeySet, MouseWheelRotation::Backward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference ("03_PanAndZoom_ZoomedOut2.svg"));
}

}
