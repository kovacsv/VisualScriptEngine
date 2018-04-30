#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorVisualTest
{

class SimpleNodeEditorTestEnv : public NodeEditorTestEnv
{
public:
	SimpleNodeEditorTestEnv () :
		NodeEditorTestEnv ()
	{
		NodeUIManager& uiManager = nodeEditor.GetNodeUIManager ();

		doubleUpDownNode = uiManager.AddNode (NUIE::UINodePtr (new DoubleUpDownNode (L"Number", NUIE::Point (100, 200), 20, 10)), uiEnvironment.GetEvaluationEnv ());
		rangeInputNode = uiManager.AddNode (NUIE::UINodePtr (new DoubleRangeNode (L"Range", NUIE::Point (300, 400))), uiEnvironment.GetEvaluationEnv ());
		viewerUINode1 = uiManager.AddNode (NUIE::UINodePtr (new MultiLineViewerNode (L"Viewer", NUIE::Point (600, 100), 5)), uiEnvironment.GetEvaluationEnv ());
		viewerUINode2 = uiManager.AddNode (NUIE::UINodePtr (new MultiLineViewerNode (L"Viewer 2", NUIE::Point (600, 400), 5)), uiEnvironment.GetEvaluationEnv ());

		nodeEditor.Update ();
		RecalcPositions ();
	}

	void RecalcPositions ()
	{
		pointInBackground = Point (5.0, 5.0);
		doubleInputRect = doubleUpDownNode->GetNodeRect (uiEnvironment);
		rangeInputRect = rangeInputNode->GetNodeRect (uiEnvironment);
		viewer1InputRect = viewerUINode1->GetNodeRect (uiEnvironment);
		viewer2InputRect = viewerUINode2->GetNodeRect (uiEnvironment);

		doubleInputHeaderPoint = doubleInputRect.GetTopCenter () + Point (5.0, 5.0);
		rangeInputHeaderPoint = rangeInputRect.GetTopCenter () + Point (5.0, 5.0);	
	}

	UINodePtr	doubleUpDownNode;
	UINodePtr	rangeInputNode;
	UINodePtr	viewerUINode1;
	UINodePtr	viewerUINode2;

	Point		pointInBackground;
	Rect		doubleInputRect;
	Rect		rangeInputRect;
	Rect		viewer1InputRect;
	Rect		viewer2InputRect;

	Point		doubleInputHeaderPoint;
	Point		rangeInputHeaderPoint;
};

class SimpleNodeEditorTestEnvWithConnections : public SimpleNodeEditorTestEnv
{
public:
	SimpleNodeEditorTestEnvWithConnections () :
		SimpleNodeEditorTestEnv ()
	{
		NodeUIManager& uiManager = nodeEditor.GetNodeUIManager ();

		uiManager.ConnectOutputSlotToInputSlot (doubleUpDownNode->GetUIOutputSlot (SlotId ("out")), rangeInputNode->GetUIInputSlot (SlotId ("start")));
		uiManager.ConnectOutputSlotToInputSlot (doubleUpDownNode->GetUIOutputSlot (SlotId ("out")), rangeInputNode->GetUIInputSlot (SlotId ("step")));
		uiManager.ConnectOutputSlotToInputSlot (doubleUpDownNode->GetUIOutputSlot (SlotId ("out")), viewerUINode1->GetUIInputSlot (SlotId ("in")));
		uiManager.ConnectOutputSlotToInputSlot (rangeInputNode->GetUIOutputSlot (SlotId ("out")), viewerUINode2->GetUIInputSlot (SlotId ("in")));

		nodeEditor.Update ();
		RecalcPositions ();
	}
};

TEST (SelectionTest)
{
	SimpleNodeEditorTestEnv env;
	
	{ // deselect all
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference ("01_Selection_NoSelection.svg"));
	}

	{ // select one node by clicking on the header
		Point doubleInputHeaderPoint = env.doubleInputRect.GetTopCenter () + Point (5.0, 5.0);
		env.Click (doubleInputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_DoubleNodeSelected.svg"));
	}

	{ // deselect one node by clicking on the header again
		Point doubleInputHeaderPoint = env.doubleInputRect.GetTopCenter () + Point (5.0, 5.0);
		env.Click (doubleInputHeaderPoint);
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
		Point doubleInputHeaderPoint = env.doubleInputRect.GetTopCenter () + Point (5.0, 5.0);
		env.Click (doubleInputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_DoubleNodeSelected.svg"));
	}

	{ // append another node to selection
		Point viewer1InputHeaderPoint = env.viewer1InputRect.GetTopCenter () + Point (5.0, 5.0);
		env.CtrlClick (viewer1InputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_DoubleAndViewer1Selected.svg"));
	}

	{ // remove the other node from selection
		Point viewer1InputHeaderPoint = env.viewer1InputRect.GetTopCenter () + Point (5.0, 5.0);
		env.CtrlClick (viewer1InputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_DoubleNodeSelected.svg"));
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
		Point rectSelectStart = env.doubleInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd = env.doubleInputRect.GetBottomRight () + Point (10.0, 10.0);
		env.nodeEditor.OnMouseDown (ModifierKeys ({ ModifierKeyCode::Control }), MouseButton::Left, (int) rectSelectStart.GetX (), (int) rectSelectStart.GetY ());
		env.nodeEditor.OnMouseMove (ModifierKeys ({ ModifierKeyCode::Control }), (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		env.nodeEditor.OnMouseUp (ModifierKeys ({ ModifierKeyCode::Control }), MouseButton::Left, (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		ASSERT (env.CheckReference ("01_Selection_DoubleRangeAndViewer2Selected.svg"));
	}

	{ // move the three nodes together
		Point targetPoint = env.rangeInputHeaderPoint + Point (50.0, 70.0);
		env.DragDrop (env.rangeInputHeaderPoint, targetPoint);
		ASSERT (env.CheckReference ("01_Selection_DoubleRangeAndViewer2Moved.svg"));
	}
}

TEST (SlotConnectionTest)
{
	SimpleNodeEditorTestEnv env;

	Point doubleOutputSlotPosition = env.doubleUpDownNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point rangeOutputSlotPosition = env.rangeInputNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Rect viewer1InputSlotRect = env.viewerUINode1->GetInputSlotRect (env.uiEnvironment, SlotId ("in"));
	Point viewer1InputSlotPosition = viewer1InputSlotRect.GetCenter ();
	Point viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
	Point rangeStartInputSlotPosition = env.rangeInputNode->GetInputSlotRect (env.uiEnvironment, SlotId ("start")).GetCenter ();

	ASSERT (env.CheckReference ("02_SlotConnection_Basic.svg"));

	{ // start connecting double output slot without target
		Point targetPosition = doubleOutputSlotPosition + Point (200.0, -100.0);
		env.DragDrop (doubleOutputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_DraggingDoubleOutput.svg"));
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

	{ // connect double output slot to viewer1 input slot
		Point targetPos = viewer1InputSlotRect.GetLeftCenter () - Point (5.0, 0.0);
		env.DragDrop (doubleOutputSlotPosition, targetPos, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_ConnectingDoubleToViewer1.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_DoubleConnectedToViewer1.svg"));
	}

	{ // start connecting double output slot without target again
		Point targetPosition = doubleOutputSlotPosition + Point (200.0, -100.0);
		env.DragDrop (doubleOutputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_DraggingConnectedViewer1Input.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_DoubleConnectedToViewer1.svg"));
	}

	{ // connect viewer2 input slot to range output slot
		env.DragDrop (viewer2InputSlotPosition, rangeOutputSlotPosition, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_ConnectingViewer2ToRange.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_AllViewersConnected.svg"));
	}

	{ // connect double output slot to range start slot
		env.DragDrop (doubleOutputSlotPosition, rangeStartInputSlotPosition);
		ASSERT (env.CheckReference ("02_SlotConnection_AllConnected.svg"));
	}

	viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
	{ // connect double output slot to viewer2 input slot
		env.DragDrop (doubleOutputSlotPosition, viewer2InputSlotPosition, [&] () {
			ASSERT (env.CheckReference ("02_SlotConnection_ConnectingDoubleToViewer2.svg"));
		});
		ASSERT (env.CheckReference ("02_SlotConnection_DoubleToViewer2Connected.svg"));
	}
}

TEST (PanAndZoomTest)
{
	SimpleNodeEditorTestEnv env;
	ASSERT (env.CheckReference ("03_PanAndZoom_Basic.svg"));

	Point doubleOutputSlotPosition = env.doubleUpDownNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point rangeStartInputSlotPosition = env.rangeInputNode->GetInputSlotRect (env.uiEnvironment, SlotId ("start")).GetCenter ();
	Point rangeOutputSlotPosition = env.rangeInputNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
	{
		env.DragDrop (doubleOutputSlotPosition, rangeStartInputSlotPosition);	
		env.DragDrop (rangeOutputSlotPosition, viewer2InputSlotPosition);
		ASSERT (env.CheckReference ("03_PanAndZoom_Connections.svg"));
	}

	Point panStartPoint = env.rangeInputNode->GetNodeRect (env.uiEnvironment).GetTopLeft ();
	Point panEndPoint = panStartPoint + Point (50.0, -50.0);

	{
		env.nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Middle, (int) panStartPoint.GetX (), (int) panStartPoint.GetY ());
		env.nodeEditor.OnMouseMove (EmptyModifierKeys, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
		env.nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Middle, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
		ASSERT (env.CheckReference ("03_PanAndZoom_Panned.svg"));
	}

	for (size_t i = 0; i < 5; i++) {
		env.nodeEditor.OnMouseWheel (EmptyModifierKeys, MouseWheelRotation::Forward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference ("03_PanAndZoom_ZoomedIn.svg"));

	for (size_t i = 0; i < 10; i++) {
		env.nodeEditor.OnMouseWheel (EmptyModifierKeys, MouseWheelRotation::Forward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference ("03_PanAndZoom_ZoomedIn2.svg"));

	for (size_t i = 0; i < 20; i++) {
		env.nodeEditor.OnMouseWheel (EmptyModifierKeys, MouseWheelRotation::Backward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference ("03_PanAndZoom_ZoomedOut.svg"));

	for (size_t i = 0; i < 10; i++) {
		env.nodeEditor.OnMouseWheel (EmptyModifierKeys, MouseWheelRotation::Backward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference ("03_PanAndZoom_ZoomedOut2.svg"));
}

TEST (DeleteNodeTest)
{
	SimpleNodeEditorTestEnvWithConnections env;
	ASSERT (env.CheckReference ("04_Delete_Basic.svg"));
	env.SetNextCommandName (L"Delete Nodes");
	env.RightClick (env.rangeInputHeaderPoint);
	ASSERT (env.CheckReference ("04_Delete_RangeDeleted.svg"));
}

TEST (CopyPasteTest)
{
	SimpleNodeEditorTestEnvWithConnections env;
	ASSERT (env.CheckReference ("05_CopyPaste_Basic.svg"));
	env.Click (env.rangeInputHeaderPoint);
	env.CtrlClick (env.doubleInputHeaderPoint);
	ASSERT (env.CheckReference ("05_CopyPaste_TwoNodesSelected.svg"));
	env.SetNextCommandName (L"Copy Nodes");
	env.RightClick (env.doubleInputHeaderPoint);
	Point targetPoint = env.doubleInputHeaderPoint + Point (120, 20);
	env.SetNextCommandName (L"Paste Nodes");
	env.RightClick (targetPoint);
	ASSERT (env.CheckReference ("05_CopyPaste_TwoNodesPasted.svg"));
}

TEST (NodeGroupingTest)
{
	SimpleNodeEditorTestEnvWithConnections env;
	ASSERT (env.CheckReference ("06_NodeGrouping_Basic.svg"));

	Point doubleOutputSlotPosition = env.doubleUpDownNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point rangeStartInputSlotPosition = env.rangeInputNode->GetInputSlotRect (env.uiEnvironment, SlotId ("start")).GetCenter ();
	Point rangeOutputSlotPosition = env.rangeInputNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
	
	{ // select nodes with selection rect
		Point rectSelectStart = env.doubleInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd = env.rangeInputRect.GetBottomRight () + Point (10.0, 10.0);
		env.nodeEditor.OnMouseDown (NUIE::EmptyModifierKeys, MouseButton::Left, (int) rectSelectStart.GetX (), (int) rectSelectStart.GetY ());
		env.nodeEditor.OnMouseMove (NUIE::EmptyModifierKeys, (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		env.nodeEditor.OnMouseUp (NUIE::EmptyModifierKeys, MouseButton::Left, (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		ASSERT (env.CheckReference ("06_NodeGrouping_DoubleAndRangeSelected.svg"));
	}

	{ // group the selected nodes, and deselect all
		env.SetNextCommandName (L"Create New Group");
		env.RightClick (env.rangeInputHeaderPoint);
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference ("06_NodeGrouping_DoubleAndRangeGrouped.svg"));
	}

	{ // move the group
		env.DragDrop (env.doubleInputRect.GetTopRight () + Point (10.0, 0.0), env.doubleInputRect.GetTopRight () + Point (20.0, 10.0));
		ASSERT (env.CheckReference ("06_NodeGrouping_GroupMoved.svg"));
	}
}

}
