#include "SimpleTest.hpp"
#include "NodeEditor.hpp"
#include "InputUINodes.hpp"
#include "ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;

namespace NodeEditorVisualTest
{

class SimpleNodeEditorTestEnv : public NodeEditorTestEnv
{
public:
	SimpleNodeEditorTestEnv () :
		NodeEditorTestEnv ()
	{
		NodeUIManager& uiManager = nodeEditor.GetNodeUIManager ();

		integerInputNode = uiManager.AddNode (NUIE::UINodePtr (new IntegerUpDownUINode (L"Integer", NUIE::Point (100, 200), 20, 10)), uiEnvironment.GetEvaluationEnv ());
		rangeInputNode = uiManager.AddNode (NUIE::UINodePtr (new IntegerRangeNode (L"Range", NUIE::Point (300, 400))), uiEnvironment.GetEvaluationEnv ());
		viewerUINode1 = uiManager.AddNode (NUIE::UINodePtr (new MultiLineViewerUINode (L"Viewer", NUIE::Point (600, 100), 5)), uiEnvironment.GetEvaluationEnv ());
		viewerUINode2 = uiManager.AddNode (NUIE::UINodePtr (new MultiLineViewerUINode (L"Viewer 2", NUIE::Point (600, 400), 5)), uiEnvironment.GetEvaluationEnv ());
		nodeEditor.Update ();

		pointInBackground = Point (5.0, 5.0);
		integerInputRect = integerInputNode->GetNodeRect (uiEnvironment);
		rangeInputRect = rangeInputNode->GetNodeRect (uiEnvironment);
		viewer1InputRect = viewerUINode1->GetNodeRect (uiEnvironment);
		viewer2InputRect = viewerUINode2->GetNodeRect (uiEnvironment);
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

	Point integerOutputSlotPosition = env.integerInputNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point rangeOutputSlotPosition = env.rangeInputNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Rect viewer1InputSlotRect = env.viewerUINode1->GetInputSlotRect (env.uiEnvironment, SlotId ("in"));
	Point viewer1InputSlotPosition = viewer1InputSlotRect.GetCenter ();
	Point viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
	Point rangeStartInputSlotPosition = env.rangeInputNode->GetInputSlotRect (env.uiEnvironment, SlotId ("start")).GetCenter ();

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

	viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
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

	Point integerOutputSlotPosition = env.integerInputNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point rangeStartInputSlotPosition = env.rangeInputNode->GetInputSlotRect (env.uiEnvironment, SlotId ("start")).GetCenter ();
	Point rangeOutputSlotPosition = env.rangeInputNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
	{
		env.DragDrop (integerOutputSlotPosition, rangeStartInputSlotPosition);	
		env.DragDrop (rangeOutputSlotPosition, viewer2InputSlotPosition);
		ASSERT (env.CheckReference ("03_PanAndZoom_Connections.svg"));
	}

	Point panStartPoint = env.rangeInputNode->GetNodeRect (env.uiEnvironment).GetTopLeft ();
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
