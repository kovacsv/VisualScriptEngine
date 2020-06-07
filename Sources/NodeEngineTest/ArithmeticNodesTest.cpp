#include "SimpleTest.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "BI_ArithmeticUINodes.hpp"
#include "BI_InputUINodes.hpp"
#include "TestUtils.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace ArithmeticNodesTest
{

TEST (TestAllArithmeticNodes)
{
	TestDrawingEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val1 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value1"), Point (0, 0), 1.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr val2 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value1"), Point (0, 0), 2.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr val3 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value3"), Point (0, 0), 3.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr add = uiManager.AddNode (UINodePtr (new AdditionNode (LocString (L"Addition"), Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr sub = uiManager.AddNode (UINodePtr (new SubtractionNode (LocString (L"Subtracion"), Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr mul = uiManager.AddNode (UINodePtr (new MultiplicationNode (LocString (L"Multiplication"), Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr div = uiManager.AddNode (UINodePtr (new DivisionNode (LocString (L"Division"), Point (0, 0))), EmptyEvaluationEnv);

	uiManager.ConnectOutputSlotToInputSlot (val1->GetUIOutputSlot (SlotId ("out")), add->GetUIInputSlot (SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (val2->GetUIOutputSlot (SlotId ("out")), add->GetUIInputSlot (SlotId ("b")));
	uiManager.ConnectOutputSlotToInputSlot (val1->GetUIOutputSlot (SlotId ("out")), sub->GetUIInputSlot (SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (val2->GetUIOutputSlot (SlotId ("out")), sub->GetUIInputSlot (SlotId ("b")));
	uiManager.ConnectOutputSlotToInputSlot (add->GetUIOutputSlot (SlotId ("result")), mul->GetUIInputSlot (SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (sub->GetUIOutputSlot (SlotId ("result")), mul->GetUIInputSlot (SlotId ("b")));
	uiManager.ConnectOutputSlotToInputSlot (mul->GetUIOutputSlot (SlotId ("result")), div->GetUIInputSlot (SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (val3->GetUIOutputSlot (SlotId ("out")), div->GetUIInputSlot (SlotId ("b")));

	ValueConstPtr val = div->Evaluate (EmptyEvaluationEnv);
	ASSERT (IsSingleType<DoubleValue> (val));
	ASSERT (IsEqual (DoubleValue::Get (CreateSingleValue (val)), -1.0));
}

}
