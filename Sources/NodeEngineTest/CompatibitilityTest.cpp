#include "SimpleTest.hpp"
#include "NE_MemoryStream.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ArithmeticUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"

#include <fstream>

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace CompatibilityTest
{

static void WriteTestFile ()
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	UINodePtr booleanNode (new BooleanNode (String (L"Boolean \u03c0"), Point (100, 100), false));
	UINodePtr integerNode (new IntegerUpDownNode (String (L"Integer"), Point (100, 220), 1, 1));
	UINodePtr doubleNode (new DoubleUpDownNode (String (L"Double"), Point (100, 340), 1.0, 1.0));
	UINodePtr integerIncremented (new IntegerIncrementedNode (String (L"Integer Inc"), Point (300, 100)));
	UINodePtr doubleIncremented (new DoubleIncrementedNode (String (L"Double Inc"), Point (300, 240)));
	UINodePtr doubleDistributed (new DoubleDistributedNode (String (L"Double Dist"), Point (300, 380)));
	UINodePtr listBuilder (new ListBuilderNode (String (L"List Builder"), Point (300, 520)));
	UINodePtr addition (new AdditionNode (String (L"Add"), Point (500, 100)));
	UINodePtr subtraction (new SubtractionNode (String (L"Sub"), Point (500, 220)));
	UINodePtr multiplication (new MultiplicationNode (String (L"Mul"), Point (500, 340)));
	UINodePtr division (new DivisionNode (String (L"Div"), Point (500, 460)));
	UINodePtr viewer (new ViewerNode (String (L"Viewer"), Point (700, 100)));
	UINodePtr multilineViewer (new MultiLineViewerNode (String (L"Viewer"), Point (690, 300), 5));
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

	MemoryOutputStream outputStream;
	env.nodeEditor.Save (outputStream);
	std::string fileName = GetTestFilesPath () + "Current_CompatibilityTest.ne";
	std::ofstream file;
	file.open (fileName, std::ios::binary);
	if (file.is_open ()) {
		file.write (outputStream.GetBuffer ().data (), outputStream.GetBuffer ().size ());
		file.close ();
	}
}

TEST (CompatibilityTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());

	bool readSuccess = false;
	std::string fileName = GetTestFilesPath () + "CompatibilityTest.ne";
	std::ifstream file;
	file.open (fileName, std::ios::binary);
	if (file.is_open ()) {
		std::vector<char> buffer;
		buffer.assign (std::istreambuf_iterator<char> (file), std::istreambuf_iterator<char> ());
		MemoryInputStream inputStream (buffer);
		readSuccess = env.nodeEditor.Open (inputStream);
		file.close ();
	}
	ASSERT (readSuccess);
	if (!readSuccess) {
		WriteTestFile ();
	}

	ASSERT (env.CheckReference ("Compatibility_AfterRead.svg"));
}

}
