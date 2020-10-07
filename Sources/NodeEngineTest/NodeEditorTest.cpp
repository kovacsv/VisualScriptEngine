#include "SimpleTest.hpp"
#include "NE_MemoryStream.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_BuiltInNodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorTest
{

TEST (NodeEditorNeedToSaveTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	ASSERT (!env.nodeEditor.NeedToSave ());

	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (LocString (L"Integer"), Point (0.0, 0.0), 0, 1)));
	ASSERT (env.nodeEditor.NeedToSave ());
	
	MemoryOutputStream outputStream;
	ASSERT (env.nodeEditor.Save (outputStream));
	ASSERT (!env.nodeEditor.NeedToSave ());
	
	env.nodeEditor.New ();
	ASSERT (!env.nodeEditor.NeedToSave ());
	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (LocString (L"Integer"), Point (0.0, 0.0), 0, 1)));
	ASSERT (env.nodeEditor.NeedToSave ());

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	ASSERT (env.nodeEditor.Open (inputStream));
	ASSERT (!env.nodeEditor.NeedToSave ());	
	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (LocString (L"Integer"), Point (0.0, 0.0), 0, 1)));
	ASSERT (env.nodeEditor.NeedToSave ());
}

TEST (NodeEditorGetInfoTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());

	ViewBox testViewBox (Point (50, 50), 2.0);
	env.nodeEditor.SetViewBox (testViewBox);

	UINodePtr intNode (new IntegerUpDownNode (LocString (L"Integer"), Point (100.0, 100.0), 0, 1));
	UINodePtr viewerNode1 (new MultiLineViewerNode (LocString (L"Viewer1"), Point (200.0, 200.0), 5));
	UINodePtr viewerNode2 (new MultiLineViewerNode (LocString (L"Viewer2"), Point (200.0, 400.0), 5));
	env.nodeEditor.AddNode (intNode);
	env.nodeEditor.AddNode (viewerNode1);
	env.nodeEditor.AddNode (viewerNode2);
	env.nodeEditor.ConnectOutputSlotToInputSlot (intNode->GetUIOutputSlot (SlotId ("out")), viewerNode1->GetUIInputSlot (SlotId ("in")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (intNode->GetUIOutputSlot (SlotId ("out")), viewerNode2->GetUIInputSlot (SlotId ("in")));
	Selection selection;
	selection.SetNodes (NE::NodeCollection ({ viewerNode1->GetId (), viewerNode2->GetId () }));
	env.nodeEditor.SetSelection (selection);
	env.nodeEditor.ExecuteCommand (CommandCode::Group);
	
	NodeEditorInfo info = env.nodeEditor.GetInfo ();

	ASSERT (info.view.width == 800);
	ASSERT (info.view.height == 600);

	ASSERT (info.nodes.size () == 3);
	ASSERT (info.nodes[0].id == NE::NodeId (1));
	ASSERT (info.nodes[0].name == L"Integer");
	ASSERT (info.nodes[0].inputSlots.size () == 0);
	ASSERT (info.nodes[0].outputSlots.size () == 1);

	ASSERT (info.connections.size () == 2);
	ASSERT (info.connections[0].fromNodeId == NE::NodeId (1));
	ASSERT (info.connections[0].fromSlotId == SlotId ("out"));
	ASSERT (info.connections[0].toNodeId == NE::NodeId (2));
	ASSERT (info.connections[0].toSlotId == SlotId ("in"));

	ASSERT (info.groups.size () == 1);
	ASSERT (info.groups[0].id == NE::NodeGroupId (1));
	ASSERT (info.groups[0].name == L"Group");
	ASSERT (info.groups[0].nodesInGroup.size () == 2);
	ASSERT (info.groups[0].nodesInGroup[0] == NE::NodeId (2));
	ASSERT (info.groups[0].nodesInGroup[1] == NE::NodeId (3));
}

}
