#include "SimpleTest.hpp"
#include "NE_MemoryStream.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ArithmeticUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"
#include "TestReference.hpp"

#include <fstream>

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace CompatibilityTest
{

static void WriteTestFile ()
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	UINodePtr booleanNode (new BooleanNode (LocString (L"Boolean \u03c0"), Point (100, 100), false));
	UINodePtr integerNode (new IntegerUpDownNode (LocString (L"Integer"), Point (100, 220), 1, 1));
	UINodePtr doubleNode (new DoubleUpDownNode (LocString (L"Double"), Point (100, 340), 1.0, 1.0));
	UINodePtr integerIncremented (new IntegerIncrementedNode (LocString (L"Integer Inc"), Point (300, 100)));
	UINodePtr doubleIncremented (new DoubleIncrementedNode (LocString (L"Double Inc"), Point (300, 240)));
	UINodePtr doubleDistributed (new DoubleDistributedNode (LocString (L"Double Dist"), Point (300, 380)));
	UINodePtr listBuilder (new ListBuilderNode (LocString (L"List Builder"), Point (300, 520)));
	UINodePtr addition (new AdditionNode (LocString (L"Add"), Point (500, 100)));
	UINodePtr subtraction (new SubtractionNode (LocString (L"Sub"), Point (500, 220)));
	UINodePtr multiplication (new MultiplicationNode (LocString (L"Mul"), Point (500, 340)));
	UINodePtr division (new DivisionNode (LocString (L"Div"), Point (500, 460)));
	UINodePtr viewer (new ViewerNode (LocString (L"Viewer"), Point (700, 100)));
	UINodePtr multilineViewer (new MultiLineViewerNode (LocString (L"Viewer"), Point (690, 300), 5));
	env.nodeEditor.AddNode (booleanNode);
	env.nodeEditor.AddNode (integerNode);
	env.nodeEditor.AddNode (doubleNode);
	env.nodeEditor.AddNode (integerIncremented);
	env.nodeEditor.AddNode (doubleIncremented);
	env.nodeEditor.AddNode (doubleDistributed);
	env.nodeEditor.AddNode (listBuilder);
	env.nodeEditor.AddNode (subtraction);
	env.nodeEditor.AddNode (multiplication);
	env.nodeEditor.AddNode (division);
	env.nodeEditor.AddNode (addition);
	env.nodeEditor.AddNode (viewer);
	env.nodeEditor.AddNode (multilineViewer);
	env.nodeEditor.ConnectOutputSlotToInputSlot (integerNode->GetUIOutputSlot (SlotId ("out")), integerIncremented->GetUIInputSlot (SlotId ("start")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (doubleNode->GetUIOutputSlot (SlotId ("out")), doubleIncremented->GetUIInputSlot (SlotId ("start")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (doubleNode->GetUIOutputSlot (SlotId ("out")), listBuilder->GetUIInputSlot (SlotId ("in")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (doubleNode->GetUIOutputSlot (SlotId ("out")), division->GetUIInputSlot (SlotId ("b")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (integerIncremented->GetUIOutputSlot (SlotId ("out")), addition->GetUIInputSlot (SlotId ("a")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (integerIncremented->GetUIOutputSlot (SlotId ("out")), subtraction->GetUIInputSlot (SlotId ("a")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (doubleIncremented->GetUIOutputSlot (SlotId ("out")), subtraction->GetUIInputSlot (SlotId ("b")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (doubleDistributed->GetUIOutputSlot (SlotId ("out")), multiplication->GetUIInputSlot (SlotId ("a")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (doubleDistributed->GetUIOutputSlot (SlotId ("out")), multiplication->GetUIInputSlot (SlotId ("b")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (listBuilder->GetUIOutputSlot (SlotId ("out")), division->GetUIInputSlot (SlotId ("a")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (booleanNode->GetUIOutputSlot (SlotId ("out")), viewer->GetUIInputSlot (SlotId ("in")));
	env.nodeEditor.ConnectOutputSlotToInputSlot (multiplication->GetUIOutputSlot (SlotId ("result")), multilineViewer->GetUIInputSlot (SlotId ("in")));

	std::wstring fileName = GetTestFilesPath () + L"Current_CompatibilityTest.ne";
	env.nodeEditor.Save (fileName);
}

TEST (CompatibilityTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());

	std::wstring fileName = GetTestFilesPath () + L"CompatibilityTest.ne";
	bool readSuccess = env.nodeEditor.Open (fileName);
	if (DBGERROR (!readSuccess)) {
		WriteTestFile ();
	}

	ASSERT (env.CheckReference (L"Compatibility_AfterRead.svg"));
}

}
