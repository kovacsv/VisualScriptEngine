#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorVisualTestReconnection
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
