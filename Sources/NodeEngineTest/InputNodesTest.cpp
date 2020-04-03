#include "SimpleTest.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "BI_InputUINodes.hpp"
#include "TestUtils.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace InputNodesTest
{

TEST (TestListBuilderNode)
{
	TestDrawingEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr intNode = uiManager.AddNode (UINodePtr (new IntegerUpDownNode (L"Value1", Point (0, 0), 1, 1)), EmptyEvaluationEnv);
	UINodePtr intIncNode = uiManager.AddNode (UINodePtr (new IntegerIncrementedNode (L"Value2", Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr listBuilder = uiManager.AddNode (UINodePtr (new ListBuilderNode (L"List Builder", Point (0, 0))), EmptyEvaluationEnv);

	{
		ValueConstPtr val = listBuilder->Evaluate (EmptyEvaluationEnv);
		ASSERT (val == nullptr);
	}

	uiManager.ConnectOutputSlotToInputSlot (intNode->GetUIOutputSlot (SlotId ("out")), listBuilder->GetUIInputSlot (SlotId ("in")));
	{
		ValueConstPtr val = listBuilder->Evaluate (EmptyEvaluationEnv);
		ASSERT (val != nullptr);
		ASSERT (IsComplexType<IntValue> (val));
		ASSERT (Value::IsType<ListValue> (val));
		ASSERT (Value::Cast<ListValue> (val)->GetSize () == 1);
	}

	uiManager.ConnectOutputSlotToInputSlot (intIncNode->GetUIOutputSlot (SlotId ("out")), listBuilder->GetUIInputSlot (SlotId ("in")));
	{
		ValueConstPtr val = listBuilder->Evaluate (EmptyEvaluationEnv);
		ASSERT (val != nullptr);
		ASSERT (IsComplexType<IntValue> (val));
		ASSERT (Value::IsType<ListValue> (val));
		ASSERT (Value::Cast<ListValue> (val)->GetSize () == 11);
	}
}

}
