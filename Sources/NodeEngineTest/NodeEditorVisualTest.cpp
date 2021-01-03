#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "NUIE_NodeCommonMenuCommands.hpp"
#include "NUIE_FeatureToggle.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "TestEnvironment.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorVisualTest
{

TEST (SelectionTest)
{
	SimpleNodeEditorTestEnv env (GetDefaultSkinParams ());
	
	{ // deselect all
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference (L"Selection_NoSelection.svg"));
	}

	{ // select one node by clicking on the header
		Point doubleInputHeaderPoint = env.doubleInputRect.GetTopCenter () + Point (5.0, 5.0);
		env.Click (doubleInputHeaderPoint);
		ASSERT (env.CheckReference (L"Selection_DoubleNodeSelected.svg"));
	}

	{ // deselect all nodes by clicking on the empty are
		env.Click (Point (5.0, 5.0));
		ASSERT (env.CheckReference (L"Selection_NoSelection.svg"));
	}
	
	{ // select node with selection rect
		Point rectSelectStart = env.rangeInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd (env.viewer2InputRect.GetCenter ().GetX (), env.rangeInputRect.GetBottom () + 10.0);
		env.DragDrop (rectSelectStart, rectSelectEnd, [&] () {
			ASSERT (env.CheckReference (L"Selection_SelectionRect.svg"));
		});
		ASSERT (env.CheckReference (L"Selection_RangeNodeSelected.svg"));
	}

	{ // deselect all
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference (L"Selection_NoSelection.svg"));
	}

	{ // select two nodes with selection rect
		Point rectSelectStart = env.rangeInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd (env.viewer2InputRect.GetRight () + 10.0, env.rangeInputRect.GetBottom () + 10.0);
		env.DragDrop (rectSelectStart, rectSelectEnd, [&] () {
			ASSERT (env.CheckReference (L"Selection_SelectionRect2.svg"));
		});
		ASSERT (env.CheckReference (L"Selection_RangeAndViewer2Selected.svg"));
	}

	{ // select another node by clicking on the header
		Point doubleInputHeaderPoint = env.doubleInputRect.GetTopCenter () + Point (5.0, 5.0);
		env.Click (doubleInputHeaderPoint);
		ASSERT (env.CheckReference (L"Selection_DoubleNodeSelected.svg"));
	}

	{ // append another node to selection
		Point viewer1InputHeaderPoint = env.viewer1InputRect.GetTopCenter () + Point (5.0, 5.0);
		env.CtrlClick (viewer1InputHeaderPoint);
		ASSERT (env.CheckReference (L"Selection_DoubleAndViewer1Selected.svg"));
	}

	{ // remove the other node from selection
		Point viewer1InputHeaderPoint = env.viewer1InputRect.GetTopCenter () + Point (5.0, 5.0);
		env.CtrlClick (viewer1InputHeaderPoint);
		ASSERT (env.CheckReference (L"Selection_DoubleNodeSelected.svg"));
	}

	{ // deselect all
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference (L"Selection_NoSelection.svg"));
	}

	{ // select two nodes with selection rect
		Point rectSelectStart = env.rangeInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd (env.viewer2InputRect.GetRight () + 10.0, env.rangeInputRect.GetBottom () + 10.0);
		env.DragDrop (rectSelectStart, rectSelectEnd);
		ASSERT (env.CheckReference (L"Selection_RangeAndViewer2Selected.svg"));
	}

	{ // select nodes with selection rect
		Point rectSelectStart = env.doubleInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd = env.doubleInputRect.GetBottomRight () + Point (10.0, 10.0);
		env.nodeEditor.OnMouseDown (ModifierKeys ({ ModifierKeyCode::Command }), MouseButton::Left, (int) rectSelectStart.GetX (), (int) rectSelectStart.GetY ());
		env.nodeEditor.OnMouseMove (ModifierKeys ({ ModifierKeyCode::Command }), (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		env.nodeEditor.OnMouseUp (ModifierKeys ({ ModifierKeyCode::Command }), MouseButton::Left, (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		ASSERT (env.CheckReference (L"Selection_DoubleRangeAndViewer2Selected.svg"));
	}
}

TEST (MoveNodesTest)
{
	SimpleNodeEditorTestEnv env (GetDefaultSkinParams ());
	env.nodeEditor.ConnectOutputSlotToInputSlot (env.doubleUpDownNode->GetUIOutputSlot (SlotId ("out")), env.rangeInputNode->GetUIInputSlot (SlotId ("start")));

	{ // select nodes with selection rect
		Point rectSelectStart = env.doubleInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd = env.rangeInputRect.GetBottomRight () + Point (10.0, 10.0);
		env.DragDrop (rectSelectStart, rectSelectEnd);
		ASSERT (env.CheckReference (L"Move_DoubleAndRangeSelected.svg"));
	}

	{ // move the three nodes together
		Point targetPoint = env.rangeInputHeaderPoint + Point (50.0, 40.0);
		env.DragDrop (env.rangeInputHeaderPoint, targetPoint);
		ASSERT (env.CheckReference (L"Move_DoubleAndRangeMoved.svg"));
	}
}

TEST (MoveCopyNodesTest)
{
	SimpleNodeEditorTestEnv env (GetDefaultSkinParams ());
	env.nodeEditor.ConnectOutputSlotToInputSlot (env.doubleUpDownNode->GetUIOutputSlot (SlotId ("out")), env.rangeInputNode->GetUIInputSlot (SlotId ("start")));

	{ // select nodes with selection rect
		Point rectSelectStart = env.doubleInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd = env.rangeInputRect.GetBottomRight () + Point (10.0, 10.0);
		env.DragDrop (rectSelectStart, rectSelectEnd);
		ASSERT (env.CheckReference (L"MoveCopy_DoubleAndRangeSelected.svg"));
	}

	{ // duplicate the three nodes together
		Point targetPoint = env.rangeInputHeaderPoint + Point (50.0, -60.0);
		env.CtrlDragDrop (env.rangeInputHeaderPoint, targetPoint, [&] () {
			ASSERT (env.CheckReference (L"MoveCopy_DoubleAndRangeDuringDuplicate.svg"));
		});
		ASSERT (env.CheckReference (L"MoveCopy_DoubleAndRangeDuplicated.svg"));
	}
}

TEST (DeleteNodeTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"Delete_Basic.svg"));
	env.SetNextCommandName (L"Delete Nodes");
	env.RightClick (env.rangeInputHeaderPoint);
	ASSERT (env.CheckReference (L"Delete_RangeDeleted.svg"));
}

TEST (CopyPasteTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"CopyPaste_Basic.svg"));
	env.Click (env.rangeInputHeaderPoint);
	env.CtrlClick (env.doubleInputHeaderPoint);
	ASSERT (env.CheckReference (L"CopyPaste_TwoNodesSelected.svg"));
	env.SetNextCommandName (L"Copy Nodes");
	env.RightClick (env.doubleInputHeaderPoint);
	Point targetPoint = env.doubleInputHeaderPoint + Point (120, 20);
	env.SetNextCommandName (L"Paste Nodes");
	env.RightClick (targetPoint);
	ASSERT (env.CheckReference (L"CopyPaste_TwoNodesPasted.svg"));
}

TEST (NodeGroupingTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"NodeGrouping_Basic.svg"));

	{ // select nodes with selection rect
		Point rectSelectStart = env.doubleInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd = env.rangeInputRect.GetBottomRight () + Point (10.0, 10.0);
		env.nodeEditor.OnMouseDown (NUIE::EmptyModifierKeys, MouseButton::Left, (int) rectSelectStart.GetX (), (int) rectSelectStart.GetY ());
		env.nodeEditor.OnMouseMove (NUIE::EmptyModifierKeys, (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		env.nodeEditor.OnMouseUp (NUIE::EmptyModifierKeys, MouseButton::Left, (int) rectSelectEnd.GetX (), (int) rectSelectEnd.GetY ());
		ASSERT (env.CheckReference (L"NodeGrouping_DoubleAndRangeSelected.svg"));
	}

	{ // group the selected nodes, and deselect all
		env.SetNextCommandName (L"Create New Group");
		env.RightClick (env.rangeInputHeaderPoint);
		env.Click (env.pointInBackground);
		ASSERT (env.CheckReference (L"NodeGrouping_DoubleAndRangeGrouped.svg"));
	}

	{ // move the group
		env.DragDrop (env.doubleInputRect.GetTopRight () + Point (10.0, 0.0), env.doubleInputRect.GetTopRight () + Point (20.0, 10.0));
		ASSERT (env.CheckReference (L"NodeGrouping_GroupMoved.svg"));
	}

	{ // move one node
		Point currentRangeInputHeaderPoint = env.rangeInputNode->GetRect (env.uiEnvironment).GetTopCenter () + Point (5.0, 5.0);
		env.DragDrop (currentRangeInputHeaderPoint, currentRangeInputHeaderPoint + Point (10.0, 10.0), [&] () {
			ASSERT (env.CheckReference (L"NodeGrouping_RangeInputMoving.svg"));
		});
		ASSERT (env.CheckReference (L"NodeGrouping_RangeInputMoved.svg"));
	}
}

TEST (UndoTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"UndoTest_Empty.svg"));

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

	ASSERT (env.CheckReference (L"UndoTest_Initial.svg"));

	for (int i = 1; i <= 11; i++) {
		env.nodeEditor.ExecuteCommand (CommandCode::Undo);
		std::wstring indexString = std::to_wstring (i);
		while (indexString.length () < 2) {
			indexString = L"0" + indexString;
		}
		ASSERT (env.CheckReference (L"UndoTest_Undo_" + indexString + L".svg"));
	}
	for (int i = 1; i <= 11; i++) {
		env.nodeEditor.ExecuteCommand (CommandCode::Redo);
		std::wstring indexString = std::to_wstring (i);
		while (indexString.length () < 2) {
			indexString = L"0" + indexString;
		}
		ASSERT (env.CheckReference (L"UndoTest_Redo_" + indexString + L".svg"));
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
	ASSERT (env.CheckReference (L"ManualUpdateTest_Init.svg"));

	env.Click (Point (145, 140));
	ASSERT (env.CheckReference (L"ManualUpdateTest_AutoUpdateMode.svg"));
	env.nodeEditor.SetUpdateMode (NodeEditor::UpdateMode::Manual);

	env.Click (Point (145, 140));
	ASSERT (env.CheckReference (L"ManualUpdateTest_ManualUpdateMode.svg"));
	
	env.nodeEditor.ManualUpdate ();
	ASSERT (env.CheckReference (L"ManualUpdateTest_ManualUpdateMode_ForceUpdate.svg"));
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
	ASSERT (env.CheckReference (L"ManualUpdateTestUndo_Init.svg"));

	env.nodeEditor.ManualUpdate ();
	ASSERT (env.CheckReference (L"ManualUpdateTestUndo_ManualUpdate.svg"));

	env.nodeEditor.Undo ();
	ASSERT (env.CheckReference (L"ManualUpdateTestUndo_AfterUndo.svg"));

	env.nodeEditor.ManualUpdate ();
	ASSERT (env.CheckReference (L"ManualUpdateTestUndo_ManualUpdateAfterUndo.svg"));
}

TEST (SkinParamsTest)
{
	BasicSkinParams skinParams (
		/*backgroundColor*/ NUIE::Color (250, 250, 250),
		/*connectionLinePen*/ NUIE::Pen (NUIE::Color (0, 0, 200), 5.0),
		/*connectionMarker*/ NUIE::SkinParams::ConnectionMarker::Circle,
		/*connectionMarkerSize*/ NUIE::Size (18.0, 18.0),
		/*nodePadding*/ 10.0,
		/*nodeBorderPen*/ NUIE::Pen (NUIE::Color (0, 200, 0), 5.0),
		/*nodeHeaderTextFont*/ NUIE::Font (L"Arial", 16.0),
		/*nodeHeaderTextColor*/ NUIE::Color (200, 0, 0),
		/*nodeHeaderErrorTextColor*/ NUIE::Color (200, 0, 0),
		/*nodeHeaderBackgroundColor*/ NUIE::Color (180, 180, 200),
		/*nodeHeaderErrorBackgroundColor*/ NUIE::Color (250, 100, 100),
		/*nodeContentTextFont*/ NUIE::Font (L"Arial", 14.0),
		/*nodeContentTextColor*/ NUIE::Color (0, 100, 100),
		/*nodeContentBackgroundColor*/ NUIE::Color (180, 200, 180),
		/*slotTextColor*/ NUIE::Color (100, 100, 0),
		/*slotTextBackgroundColor*/ NUIE::Color (180, 250, 250),
		/*slotMarker*/ NUIE::SkinParams::SlotMarker::Circle,
		/*hiddenSlotMarker*/NUIE::SkinParams::HiddenSlotMarker::Arrow,
		/*slotMarkerSize*/ NUIE::Size (18.0, 18.0),
		/*selectionBlendColor*/ NUIE::BlendColor (NUIE::Color (0, 0, 255), 0.25),
		/*disabledBlendColor*/ NUIE::BlendColor (NUIE::Color (0, 138, 184), 0.2),
		/*selectionRectPen*/ NUIE::Pen (NUIE::Color (0, 0, 255), 5.0),
		/*nodeSelectionRectPen*/ NUIE::Pen (NUIE::Color (0, 0, 255), 8.0),
		/*buttonBorderPen*/ NUIE::Pen (NUIE::Color (255, 0, 0), 3.0),
		/*buttonBackgroundColor*/ NUIE::Color (250, 250, 180),
		/*textPanelTextColor*/ NUIE::Color (0, 100, 0),
		/*textPanelBackgroundColor*/ NUIE::Color (250, 150, 150),
		/*groupNameFont*/ NUIE::Font (L"Arial", 16.0),
		/*groupNameColor*/ NUIE::Color (0, 0, 0),
		/*groupBackgroundColors*/ NUIE::NamedColorSet ({
			{ NE::LocalizeString (L"Blue"), NUIE::Color (160, 200, 240) },
			{ NE::LocalizeString (L"Green"), NUIE::Color (160, 239, 160) },
			{ NE::LocalizeString (L"Red"), NUIE::Color (239, 189, 160) }
			}),
		/*groupPadding*/ 12.0
	);

	SimpleNodeEditorTestEnvWithConnections env (skinParams);
	{ // select one node by clicking on the header
		env.Click (env.rangeInputHeaderPoint);
	}

	ASSERT (env.CheckReference (L"SkinParams_Basic.svg"));
}

TEST (AlignNodesTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());

	{ // select three nodes with selection rect
		Point rectSelectStart = env.doubleInputRect.GetTopLeft () - Point (10.0, 10.0);
		Point rectSelectEnd = env.viewer2InputRect.GetBottomRight () + Point (10.0, 10.0);
		env.DragDrop (rectSelectStart, rectSelectEnd);
		ASSERT (env.CheckReference (L"AlignNodesTest_NodesSelected.svg"));
	}

	{ // align selected nodes to bottom
		env.SetNextCommandName (L"Vertical Bottom");
		env.RightClick (env.viewer2HeaderPoint);
		ASSERT (env.CheckReference (L"AlignNodesTest_AlignedToBottom.svg"));
	}
}

TEST (NodeOrderTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	Point padding (10.0, 10.0);

	{ // create two nodes
		env.nodeEditor.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Number 1"), Point (100.0, 100.0), 0.0, 1.0)));
		env.nodeEditor.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Number 2"), Point (150.0, 100.0), 0.0, 1.0)));
		ASSERT (env.CheckReference (L"NodeOrderTest_Initial.svg"));
	}

	{ // select top node
		env.Click (env.GetNodeRect (env.GetNode (L"Number 2")).GetTopLeft () + padding);
		ASSERT (env.CheckReference (L"NodeOrderTest_Number2Selected.svg"));
	}

	{ // delete bottom node
		env.ExecuteCommand (CommandCode::Escape);
		env.SetNextCommandName (L"Delete Nodes");
		env.RightClick (env.GetNodeRect (env.GetNode (L"Number 1")).GetTopLeft () + padding);
		ASSERT (env.CheckReference (L"NodeOrderTest_Number1Deleted.svg"));
	}

	{ // undo delete
		env.ExecuteCommand (CommandCode::Undo);
		ASSERT (env.CheckReference (L"NodeOrderTest_Number1Recreated.svg"));
	}

	{ // select top node
		env.Click (env.GetNodeRect (env.GetNode (L"Number 2")).GetTopLeft () + padding);
		ASSERT (env.CheckReference (L"NodeOrderTest_Number2SelectedAfterUndo.svg"));
	}
}

TEST (MultiMouseMoveHandlerTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	Point padding (10.0, 10.0);

	{ // create two nodes
		env.nodeEditor.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Number 1"), Point (100.0, 100.0), 0.0, 1.0)));
		env.nodeEditor.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Number 2"), Point (250.0, 250.0), 0.0, 1.0)));
		ASSERT (env.CheckReference (L"MultiMouseMoveHandlerTest_Initial.svg"));
	}

	Point mousePosition;
	{ // start dragging number 2
		mousePosition = env.GetNodeRect (env.GetNode (L"Number 2")).GetTopLeft () + padding;
		env.nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Left, (int) mousePosition.GetX (), (int) mousePosition.GetY ());
		mousePosition = mousePosition + Point (100, 0);
		env.nodeEditor.OnMouseMove (EmptyModifierKeys, (int) mousePosition.GetX (), (int) mousePosition.GetY ());
		ASSERT (env.CheckReference (L"MultiMouseMoveHandlerTest_Number2Dragging.svg"));
	}

	{ // start panning the canvas
		env.nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Right, (int) mousePosition.GetX (), (int) mousePosition.GetY ());
		mousePosition = mousePosition + Point (0, 50);
		env.nodeEditor.OnMouseMove (EmptyModifierKeys, (int) mousePosition.GetX (), (int) mousePosition.GetY ());
		ASSERT (env.CheckReference (L"MultiMouseMoveHandlerTest_Number2DraggingAndPanning.svg"));
	}

	{ // release mouse buttons
		env.nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Right, (int) mousePosition.GetX (), (int) mousePosition.GetY ());
		env.nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Left, (int) mousePosition.GetX (), (int) mousePosition.GetY ());
		ASSERT (env.CheckReference (L"MultiMouseMoveHandlerTest_Final.svg"));
	}
}

}
