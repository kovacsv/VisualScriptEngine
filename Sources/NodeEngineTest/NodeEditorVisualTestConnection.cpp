#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "TestEnvironment.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorVisualTestConnection
{

class ReconnectionTestEnv : public NodeEditorTestEnv
{
public:
	ReconnectionTestEnv (const BasicSkinParams& skinParams) :
		NodeEditorTestEnv (skinParams)
	{
		integerUpDownNode.reset (new BI::IntegerUpDownNode (LocString (L"Integer"), NUIE::Point (100, 100), 1, 1));
		doubleUpDownNode.reset (new BI::DoubleUpDownNode (LocString (L"Number"), NUIE::Point (100, 300), 2.0, 1.0));
		listBuilder1Node.reset (new BI::ListBuilderNode (LocString (L"List"), NUIE::Point (350, 200)));
		listBuilder2Node.reset (new BI::ListBuilderNode (LocString (L"List"), NUIE::Point (350, 400)));
		viewer1Node.reset (new BI::MultiLineViewerNode (LocString (L"Viewer"), NUIE::Point (600, 100), 5));
		viewer2Node.reset (new BI::MultiLineViewerNode (LocString (L"Viewer"), NUIE::Point (600, 300), 5));
		viewer3Node.reset (new BI::MultiLineViewerNode (LocString (L"Viewer"), NUIE::Point (600, 500), 5));

		nodeEditor.AddNode (integerUpDownNode);
		nodeEditor.AddNode (doubleUpDownNode);
		nodeEditor.AddNode (listBuilder1Node);
		nodeEditor.AddNode (listBuilder2Node);
		nodeEditor.AddNode (viewer1Node);
		nodeEditor.AddNode (viewer2Node);
		nodeEditor.AddNode (viewer3Node);

		nodeEditor.ConnectOutputSlotToInputSlot (integerUpDownNode->GetUIOutputSlot (SlotId ("out")), listBuilder1Node->GetUIInputSlot (SlotId ("in")));
		nodeEditor.ConnectOutputSlotToInputSlot (doubleUpDownNode->GetUIOutputSlot (SlotId ("out")), listBuilder1Node->GetUIInputSlot (SlotId ("in")));

		nodeEditor.ConnectOutputSlotToInputSlot (listBuilder1Node->GetUIOutputSlot (SlotId ("out")), viewer1Node->GetUIInputSlot (SlotId ("in")));
		nodeEditor.ConnectOutputSlotToInputSlot (listBuilder1Node->GetUIOutputSlot (SlotId ("out")), viewer2Node->GetUIInputSlot (SlotId ("in")));

		nodeEditor.ConnectOutputSlotToInputSlot (listBuilder2Node->GetUIOutputSlot (SlotId ("out")), viewer3Node->GetUIInputSlot (SlotId ("in")));

		nodeEditor.Update ();
		RecalcPositions ();
	}

	void RecalcPositions ()
	{
		listBuilder1InputSlotPosition = listBuilder1Node->GetInputSlotRect (uiEnvironment, SlotId ("in")).GetCenter ();
		listBuilder2InputSlotPosition = listBuilder2Node->GetInputSlotRect (uiEnvironment, SlotId ("in")).GetCenter ();
		listBuilder1OutputSlotPosition = listBuilder1Node->GetOutputSlotRect (uiEnvironment, SlotId ("out")).GetCenter ();
		listBuilder2OutputSlotPosition = listBuilder2Node->GetOutputSlotRect (uiEnvironment, SlotId ("out")).GetCenter ();
	}

	UINodePtr	integerUpDownNode;
	UINodePtr	doubleUpDownNode;
	UINodePtr	listBuilder1Node;
	UINodePtr	listBuilder2Node;
	UINodePtr	viewer1Node;
	UINodePtr	viewer2Node;
	UINodePtr	viewer3Node;

	Point		listBuilder1InputSlotPosition;
	Point		listBuilder2InputSlotPosition;
	Point		listBuilder1OutputSlotPosition;
	Point		listBuilder2OutputSlotPosition;
};

TEST (SlotConnectionTest)
{
	SimpleNodeEditorTestEnv env (GetDefaultSkinParams ());

	Point doubleOutputSlotPosition = env.doubleUpDownNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point rangeOutputSlotPosition = env.rangeInputNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Rect viewer1InputSlotRect = env.viewerUINode1->GetInputSlotRect (env.uiEnvironment, SlotId ("in"));
	Point viewer1InputSlotPosition = viewer1InputSlotRect.GetCenter ();
	Point viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
	Point rangeStartInputSlotPosition = env.rangeInputNode->GetInputSlotRect (env.uiEnvironment, SlotId ("start")).GetCenter ();

	ASSERT (env.CheckReference (L"SlotConnection_Basic.svg"));

	{ // start connecting double output slot without target
		Point targetPosition = doubleOutputSlotPosition + Point (200.0, -100.0);
		env.DragDrop (doubleOutputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference (L"SlotConnection_DraggingDoubleOutput.svg"));
		});
		ASSERT (env.CheckReference (L"SlotConnection_Basic.svg"));
	}

	{ // start connecting viewer1 input slot without target
		Point targetPosition = viewer1InputSlotPosition - Point (200.0, -100.0);
		env.DragDrop (viewer1InputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference (L"SlotConnection_DraggingViewer1Input.svg"));
		});
		ASSERT (env.CheckReference (L"SlotConnection_Basic.svg"));
	}

	{ // connect double output slot to viewer1 input slot
		Point targetPos = viewer1InputSlotRect.GetLeftCenter () - Point (5.0, 0.0);
		env.DragDrop (doubleOutputSlotPosition, targetPos, [&] () {
			ASSERT (env.CheckReference (L"SlotConnection_ConnectingDoubleToViewer1.svg"));
		});
		ASSERT (env.CheckReference (L"SlotConnection_DoubleConnectedToViewer1.svg"));
	}

	{ // start connecting double output slot without target again
		Point targetPosition = doubleOutputSlotPosition + Point (200.0, -100.0);
		env.DragDrop (doubleOutputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference (L"SlotConnection_DraggingConnectedViewer1Input.svg"));
		});
		ASSERT (env.CheckReference (L"SlotConnection_DoubleConnectedToViewer1.svg"));
	}

	{ // connect viewer2 input slot to range output slot
		env.DragDrop (viewer2InputSlotPosition, rangeOutputSlotPosition, [&] () {
			ASSERT (env.CheckReference (L"SlotConnection_ConnectingViewer2ToRange.svg"));
		});
		ASSERT (env.CheckReference (L"SlotConnection_AllViewersConnected.svg"));
	}

	{ // connect double output slot to range start slot
		env.DragDrop (doubleOutputSlotPosition, rangeStartInputSlotPosition);
		ASSERT (env.CheckReference (L"SlotConnection_AllConnected.svg"));
	}

	viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
	{ // connect double output slot to viewer2 input slot
		env.DragDrop (doubleOutputSlotPosition, viewer2InputSlotPosition, [&] () {
			ASSERT (env.CheckReference (L"SlotConnection_ConnectingDoubleToViewer2.svg"));
		});
		ASSERT (env.CheckReference (L"SlotConnection_DoubleToViewer2Connected.svg"));
	}
}

TEST (InputSlotReconnectionTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"InputSlotReconnection_Basic.svg"));

	env.CtrlDragDrop (env.viewer1InputSlotRect.GetCenter (), env.viewer2InputSlotRect.GetCenter (), [&] () {
		ASSERT (env.CheckReference (L"InputSlotReconnection_DuringConnection.svg"));
	});

	ASSERT (env.CheckReference (L"InputSlotReconnection_Reconnected.svg"));

	env.nodeEditor.Undo ();
	ASSERT (env.CheckReference (L"InputSlotReconnection_AfterUndo.svg"));

	env.CtrlDragDrop (env.viewer1InputSlotRect.GetCenter (), Point (0.0, 0.0));
	ASSERT (env.CheckReference (L"InputSlotReconnection_AfterDisconnect.svg"));
}

TEST (OutputSlotReconnectionTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"OutputSlotReconnection_Basic.svg"));

	env.CtrlDragDrop (env.rangeOutputSlotSRect.GetCenter (), env.doubleUpDownOutputSlotRect.GetCenter (), [&] () {
		ASSERT (env.CheckReference (L"OutputSlotReconnection_DuringConnection.svg"));
	});

	ASSERT (env.CheckReference (L"OutputSlotReconnection_Reconnected.svg"));

	env.nodeEditor.Undo ();
	ASSERT (env.CheckReference (L"OutputSlotReconnection_AfterUndo.svg"));

	env.CtrlDragDrop (env.rangeOutputSlotSRect.GetCenter (), Point (0.0, 0.0));
	ASSERT (env.CheckReference (L"OutputSlotReconnection_AfterDisconnect.svg"));
}

TEST (ReconnectionTest_ReconnectInputSlot)
{
	ReconnectionTestEnv env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"Reconnection_InputSlotBasic.svg"));

	{
		env.CtrlDragDrop (env.listBuilder1InputSlotPosition, env.listBuilder2InputSlotPosition, [&] () {
			ASSERT (env.CheckReference (L"Reconnection_InputSlotBeforeReconnect.svg"));
		});
		ASSERT (env.CheckReference (L"Reconnection_InputSlotReconnected.svg"));
	}

	{
		Point disconnectPosition = env.listBuilder2InputSlotPosition - Point (0, 100);
		env.CtrlDragDrop (env.listBuilder2InputSlotPosition, disconnectPosition, [&] () {
			ASSERT (env.CheckReference (L"Reconnection_InputSlotBeforeDisconnect.svg"));
		});
		ASSERT (env.CheckReference (L"Reconnection_InputSlotDisconnected.svg"));
	}
}

TEST (ReconnectionTest_ReconnectOutputSlot)
{
	ReconnectionTestEnv env (GetDefaultSkinParams ());

	env.CtrlDragDrop (env.listBuilder1InputSlotPosition, env.listBuilder2InputSlotPosition);
	ASSERT (env.CheckReference (L"Reconnection_OutputSlotBasic.svg"));

	{
		env.CtrlDragDrop (env.listBuilder1OutputSlotPosition, env.listBuilder2OutputSlotPosition, [&] () {
			ASSERT (env.CheckReference (L"Reconnection_OutputSlotBeforeReconnect1.svg"));
		});
		ASSERT (env.CheckReference (L"Reconnection_OutputSlotReconnected1.svg"));
	}

	{
		env.CtrlDragDrop (env.listBuilder2OutputSlotPosition, env.listBuilder1OutputSlotPosition, [&] () {
			ASSERT (env.CheckReference (L"Reconnection_OutputSlotBeforeReconnect2.svg"));
		});
		ASSERT (env.CheckReference (L"Reconnection_OutputSlotReconnected2.svg"));
	}

	{
		Point disconnectPosition = env.listBuilder1OutputSlotPosition + Point (50, 100);
		env.CtrlDragDrop (env.listBuilder1OutputSlotPosition, disconnectPosition, [&] () {
			ASSERT (env.CheckReference (L"Reconnection_OutputSlotBeforeDisconnect.svg"));
		});
		ASSERT (env.CheckReference (L"Reconnection_OutputSlotDisconnected.svg"));
	}
}

}
