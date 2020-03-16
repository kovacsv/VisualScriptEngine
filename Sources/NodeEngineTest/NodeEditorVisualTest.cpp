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
	SimpleNodeEditorTestEnv (const BasicSkinParams& skinParams) :
		NodeEditorTestEnv (skinParams)
	{
		doubleUpDownNode.reset (new DoubleUpDownNode (L"Number", NUIE::Point (100, 200), 20, 10));
		rangeInputNode.reset (new DoubleIncrementedNode (L"Range", NUIE::Point (300, 400)));
		viewerUINode1.reset (new MultiLineViewerNode (L"Viewer", NUIE::Point (600, 100), 5));
		viewerUINode2.reset (new MultiLineViewerNode (L"Viewer 2", NUIE::Point (600, 400), 5));
		
		nodeEditor.AddNode (doubleUpDownNode);
		nodeEditor.AddNode (rangeInputNode);
		nodeEditor.AddNode (viewerUINode1);
		nodeEditor.AddNode (viewerUINode2); 

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
	SimpleNodeEditorTestEnvWithConnections (const BasicSkinParams& skinParams) :
		SimpleNodeEditorTestEnv (skinParams)
	{
		nodeEditor.ConnectOutputSlotToInputSlot (doubleUpDownNode->GetUIOutputSlot (SlotId ("out")), rangeInputNode->GetUIInputSlot (SlotId ("start")));
		nodeEditor.ConnectOutputSlotToInputSlot (doubleUpDownNode->GetUIOutputSlot (SlotId ("out")), rangeInputNode->GetUIInputSlot (SlotId ("step")));
		nodeEditor.ConnectOutputSlotToInputSlot (doubleUpDownNode->GetUIOutputSlot (SlotId ("out")), viewerUINode1->GetUIInputSlot (SlotId ("in")));
		nodeEditor.ConnectOutputSlotToInputSlot (rangeInputNode->GetUIOutputSlot (SlotId ("out")), viewerUINode2->GetUIInputSlot (SlotId ("in")));

		nodeEditor.Update ();
		RecalcPositions ();
	}
};

TEST (SelectionTest)
{
	SimpleNodeEditorTestEnv env (GetDefaultSkinParams ());
	
	{ // deselect all
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference ("01_Selection_NoSelection.svg"));
	}

	{ // select one node by clicking on the header
		Point doubleInputHeaderPoint = env.doubleInputRect.GetTopCenter () + Point (5.0, 5.0);
		env.Click (doubleInputHeaderPoint);
		ASSERT (env.CheckReference ("01_Selection_DoubleNodeSelected.svg"));
	}

	{ // deselect all nodes by clicking on the empty are
		env.Click (Point (5.0, 5.0));
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
	SimpleNodeEditorTestEnv env (GetDefaultSkinParams ());

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
	SimpleNodeEditorTestEnv env (GetDefaultSkinParams ());
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
		env.nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Right, (int) panStartPoint.GetX (), (int) panStartPoint.GetY ());
		env.nodeEditor.OnMouseMove (EmptyModifierKeys, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
		env.nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Right, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
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
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference ("04_Delete_Basic.svg"));
	env.SetNextCommandName (L"Delete Nodes");
	env.RightClick (env.rangeInputHeaderPoint);
	ASSERT (env.CheckReference ("04_Delete_RangeDeleted.svg"));
}

TEST (CopyPasteTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
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
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference ("06_NodeGrouping_Basic.svg"));

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

	{ // move one node
		Point currentRangeInputHeaderPoint = env.rangeInputNode->GetNodeRect (env.uiEnvironment).GetTopCenter () + Point (5.0, 5.0);
		env.DragDrop (currentRangeInputHeaderPoint, currentRangeInputHeaderPoint + Point (10.0, 10.0), [&] () {
			ASSERT (env.CheckReference ("06_NodeGrouping_RangeInputMoving.svg"));
		});
		ASSERT (env.CheckReference ("06_NodeGrouping_RangeInputMoved.svg"));
	}
}

TEST (UndoTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference ("07_UndoTest_Empty.svg"));

	env.SetNextCommandName (L"Create Number Node");
	env.RightClick (Point (100, 100));
	env.SetNextCommandName (L"Create Number Node");
	env.RightClick (Point (100, 300));
	env.SetNextCommandName (L"Create Addition Node");
	env.RightClick (Point (300, 200));
	env.SetNextCommandName (L"Create Viewer Node");
	env.RightClick (Point (600, 200));
	env.DragDrop (Point (120, 100), Point (240, 200));
	env.DragDrop (Point (120, 300), Point (240, 230));
	env.DragDrop (Point (300, 200), Point (540, 200));
	env.Click (Point (145, 135));
	env.DragDrop (Point (200, 100), Point (800, 400));
	env.DragDrop (Point (300, 160), Point (320, 180));
	env.SetNextCommandName (L"Create New Group");
	env.RightClick (Point (320, 180));
	env.Click (Point (320, 180));
	env.SetNextCommandName (L"Delete Nodes");
	env.RightClick (Point (320, 180));

	ASSERT (env.CheckReference ("07_UndoTest_Initial.svg"));

	for (int i = 1; i <= 11; i++) {
		env.nodeEditor.OnKeyPress (Key (PressedKeyCode::Undo));
		std::string indexString = std::to_string (i);
		while (indexString.length () < 2) {
			indexString = "0" + indexString;
		}
		ASSERT (env.CheckReference ("07_UndoTest_Undo_" + indexString + ".svg"));
	}
	for (int i = 1; i <= 11; i++) {
		env.nodeEditor.OnKeyPress (Key (PressedKeyCode::Redo));
		std::string indexString = std::to_string (i);
		while (indexString.length () < 2) {
			indexString = "0" + indexString;
		}
		ASSERT (env.CheckReference ("07_UndoTest_Redo_" + indexString + ".svg"));
	}
}

TEST (ManualUpdateTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	env.SetNextCommandName (L"Create Integer Node");
	env.RightClick (Point (100, 100));
	env.SetNextCommandName (L"Create Increase Node");
	env.RightClick (Point (300, 200));
	env.SetNextCommandName (L"Create Viewer Node");
	env.RightClick (Point (600, 300));
	env.DragDrop (Point (125, 100), Point (240, 220));
	env.DragDrop (Point (360, 220), Point (530, 300));
	ASSERT (env.CheckReference ("08_ManualUpdateTest_Init.svg"));

	env.Click (Point (145, 140));
	ASSERT (env.CheckReference ("08_ManualUpdateTest_AutoUpdateMode.svg"));
	env.nodeEditor.SetUpdateMode (NodeEditor::UpdateMode::Manual);

	env.Click (Point (145, 140));
	ASSERT (env.CheckReference ("08_ManualUpdateTest_ManualUpdateMode.svg"));
	
	env.nodeEditor.ManualUpdate ();
	ASSERT (env.CheckReference ("08_ManualUpdateTest_ManualUpdateMode_ForceUpdate.svg"));
}

TEST (ManualUpdateTestUndo)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	env.nodeEditor.SetUpdateMode (NodeEditor::UpdateMode::Manual);

	env.SetNextCommandName (L"Create Integer Node");
	env.RightClick (Point (100, 100));
	env.SetNextCommandName (L"Create Increase Node");
	env.RightClick (Point (300, 200));
	env.SetNextCommandName (L"Create Viewer Node");
	env.RightClick (Point (600, 300));
	env.DragDrop (Point (360, 220), Point (530, 300));
	env.DragDrop (Point (125, 100), Point (240, 220));
	ASSERT (env.CheckReference ("09_ManualUpdateTest_Init.svg"));

	env.nodeEditor.ManualUpdate ();
	ASSERT (env.CheckReference ("09_ManualUpdateTest_ManualUpdate.svg"));

	env.nodeEditor.Undo ();
	ASSERT (env.CheckReference ("09_ManualUpdateTest_AfterUndo.svg"));

	env.nodeEditor.ManualUpdate ();
	ASSERT (env.CheckReference ("09_ManualUpdateTest_ManualUpdateAfterUndo.svg"));
}

TEST (FitToWindowTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference ("10_FitToWindow_Basic.svg"));
	env.nodeEditor.FitToWindow ();
	ASSERT (env.CheckReference ("10_FitToWindow_Fit.svg"));
}

TEST (AlignToWindowTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference ("11_AlignToWindow_Basic.svg"));
	env.nodeEditor.AlignToWindow ();
	ASSERT (env.CheckReference ("11_AlignToWindow_Align.svg"));
}

TEST (SlotCirclesTest)
{
	BasicSkinParams mySkinParams (
		/*backgroundColor*/ Color (240, 240, 240),
		/*connectionLinePen*/ Pen (Color (0, 0, 0), 1.0),
		/*nodePadding*/ 5.0,
		/*nodeBorderPen*/ Pen (Color (0, 0, 0), 1.0),
		/*nodeHeaderTextFont*/ Font (L"Arial", 16.0),
		/*nodeHeaderTextColor*/ Color (255, 255, 255),
		/*nodeHeaderErrorTextColor*/ Color (255, 255, 255),
		/*nodeHeaderBackgroundColor*/ Color (100, 100, 100),
		/*nodeHeaderErrorBackgroundColor*/ Color (255, 0, 0),
		/*nodeContentTextFont*/ Font (L"Arial", 16.0),
		/*nodeContentTextColor*/ Color (0, 0, 0),
		/*nodeContentBackgroundColor*/ Color (200, 200, 200),
		/*slotTextColor*/ Color (0, 0, 0),
		/*slotTextBackgroundColor*/ Color (225, 225, 225),
		/*needToDrawSlotCircles*/ true,
		/*slotCircleSize*/ Size (12.0, 12.0),
		/*selectionBlendColor*/ BlendColor (Color (240, 240, 240), 0.5),
		/*disabledBlendColor*/ BlendColor (Color (0, 138, 184), 0.2),
		/*selectionRectPen*/ Pen (Color (0, 138, 184), 1.0),
		/*nodeSelectionRectPen*/ Pen (Color (0, 138, 184), 5.0),
		/*buttonBorderPen*/ Pen (Color (50, 75, 100), 1.0),
		/*buttonBackgroundColor*/ Color (150, 175, 200),
		/*textPanelTextColor*/ Color (0, 0, 0),
		/*textPanelBackgroundColor*/ Color (255, 255, 100),
		/*groupNameFont*/ Font (L"Arial", 18.0),
		/*groupNameColor*/ Color (0, 0, 0),
		/*groupBackgroundColors*/ NamedColorSet ({
			{ L"Blue", Color (160, 200, 240) },
			{ L"Green", Color (160, 239, 160) },
			{ L"Red", Color (239, 189, 160) }
			}),
		/*groupPadding*/ 10.0
	);
	SimpleNodeEditorTestEnvWithConnections env (mySkinParams);
	ASSERT (env.CheckReference ("12_SlotCircles.svg"));
}

}
