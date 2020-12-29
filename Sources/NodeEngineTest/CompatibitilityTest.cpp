#include "SimpleTest.hpp"
#include "NE_MemoryStream.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_BinaryOperationNodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"
#include "TestReference.hpp"

#include <fstream>

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace CompatibilityTest
{

static void InitNodeEditor (NodeEditor& nodeEditor)
{
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

	nodeEditor.AddNode (booleanNode);
	nodeEditor.AddNode (integerNode);
	nodeEditor.AddNode (doubleNode);
	nodeEditor.AddNode (integerIncremented);
	nodeEditor.AddNode (doubleIncremented);
	nodeEditor.AddNode (doubleDistributed);
	nodeEditor.AddNode (listBuilder);
	nodeEditor.AddNode (subtraction);
	nodeEditor.AddNode (multiplication);
	nodeEditor.AddNode (division);
	nodeEditor.AddNode (addition);
	nodeEditor.AddNode (viewer);
	nodeEditor.AddNode (multilineViewer);
	nodeEditor.ConnectOutputSlotToInputSlot (integerNode->GetUIOutputSlot (SlotId ("out")), integerIncremented->GetUIInputSlot (SlotId ("start")));
	nodeEditor.ConnectOutputSlotToInputSlot (doubleNode->GetUIOutputSlot (SlotId ("out")), doubleIncremented->GetUIInputSlot (SlotId ("start")));
	nodeEditor.ConnectOutputSlotToInputSlot (doubleNode->GetUIOutputSlot (SlotId ("out")), listBuilder->GetUIInputSlot (SlotId ("in")));
	nodeEditor.ConnectOutputSlotToInputSlot (doubleNode->GetUIOutputSlot (SlotId ("out")), division->GetUIInputSlot (SlotId ("b")));
	nodeEditor.ConnectOutputSlotToInputSlot (integerIncremented->GetUIOutputSlot (SlotId ("out")), addition->GetUIInputSlot (SlotId ("a")));
	nodeEditor.ConnectOutputSlotToInputSlot (integerIncremented->GetUIOutputSlot (SlotId ("out")), subtraction->GetUIInputSlot (SlotId ("a")));
	nodeEditor.ConnectOutputSlotToInputSlot (doubleIncremented->GetUIOutputSlot (SlotId ("out")), subtraction->GetUIInputSlot (SlotId ("b")));
	nodeEditor.ConnectOutputSlotToInputSlot (doubleDistributed->GetUIOutputSlot (SlotId ("out")), multiplication->GetUIInputSlot (SlotId ("a")));
	nodeEditor.ConnectOutputSlotToInputSlot (doubleDistributed->GetUIOutputSlot (SlotId ("out")), multiplication->GetUIInputSlot (SlotId ("b")));
	nodeEditor.ConnectOutputSlotToInputSlot (listBuilder->GetUIOutputSlot (SlotId ("out")), division->GetUIInputSlot (SlotId ("a")));
	nodeEditor.ConnectOutputSlotToInputSlot (booleanNode->GetUIOutputSlot (SlotId ("out")), viewer->GetUIInputSlot (SlotId ("in")));
	nodeEditor.ConnectOutputSlotToInputSlot (multiplication->GetUIOutputSlot (SlotId ("result")), multilineViewer->GetUIInputSlot (SlotId ("in")));

	Selection selection;
	selection.SetNodes (NE::NodeCollection ({ booleanNode->GetId () }));
	nodeEditor.SetSelection (selection);
	nodeEditor.ExecuteCommand (NUIE::CommandCode::Group);
	nodeEditor.SetSelection (EmptySelection);
}

static void WriteTestFile ()
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	InitNodeEditor (env.nodeEditor);

	std::wstring fileName = GetCompatibilityTestFilesPath () + L"Current_CompatibilityTest.vse";
	env.nodeEditor.Save (fileName);
}

TEST (CompatibilityTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());

	std::vector<std::wstring> fileNames = {
		L"CompatibilityTest_0_3_11.vse",
		L"CompatibilityTest_0_4_6.vse",
		L"CompatibilityTest_0_5_1.vse"
	};
	for (const std::wstring& fileName : fileNames) {
		std::wstring filePath = GetCompatibilityTestFilesPath () + fileName;
		bool readSuccess = env.nodeEditor.Open (filePath);
		if (DBGERROR (!readSuccess)) {
			WriteTestFile ();
		}
		ASSERT (env.CheckReference (L"Compatibility_AfterRead.svg"));
	}
}

TEST (NewestVersionReadWriteTest)
{
	MemoryOutputStream outputStream;
	{
		NodeEditorTestEnv env (GetDefaultSkinParams ());
		InitNodeEditor (env.nodeEditor);
		ASSERT (env.CheckReference (L"NewestVersionReadWrite_Original.svg"));
		env.nodeEditor.Save (outputStream);
	}

	{
		NodeEditorTestEnv env2 (GetDefaultSkinParams ());
		MemoryInputStream inputSream (outputStream.GetBuffer ());
		env2.nodeEditor.Open (inputSream);
		ASSERT (env2.CheckReference (L"NewestVersionReadWrite_AfterSaveAndLoad.svg"));
	}
}

}
