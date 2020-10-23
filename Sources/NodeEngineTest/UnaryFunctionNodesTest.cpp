#include "SimpleTest.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "BI_UnaryFunctionNodes.hpp"
#include "BI_InputUINodes.hpp"
#include "TestUtils.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace UnaryFunctionNodesTest
{

TEST (TestAbsoluteValueNode)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value"), Point (0, 0), -1.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr op = uiManager.AddNode (UINodePtr (new AbsoluteValueNode (LocString (L"AbsoluteValue"), Point (0, 0))), EmptyEvaluationEnv);
	uiManager.ConnectOutputSlotToInputSlot (val->GetUIOutputSlot (SlotId ("out")), op->GetUIInputSlot (SlotId ("a")));

	ValueConstPtr value = op->Evaluate (EmptyEvaluationEnv);
	ASSERT (IsSingleType<DoubleValue> (value));
	ASSERT (IsEqual (DoubleValue::Get (value), 1.0));
}

TEST (TestAbsoluteValueNodeWithList)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value"), Point (0, 0), -5.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr listVal = uiManager.AddNode (UINodePtr (new DoubleIncrementedNode (LocString (L"Value"), Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr op = uiManager.AddNode (UINodePtr (new AbsoluteValueNode (LocString (L"AbsoluteValue"), Point (0, 0))), EmptyEvaluationEnv);
	uiManager.ConnectOutputSlotToInputSlot (val->GetUIOutputSlot (SlotId ("out")), listVal->GetUIInputSlot (SlotId ("start")));
	uiManager.ConnectOutputSlotToInputSlot (listVal->GetUIOutputSlot (SlotId ("out")), op->GetUIInputSlot (SlotId ("a")));

	ValueConstPtr value = op->Evaluate (EmptyEvaluationEnv);
	ASSERT (Value::IsType<ListValue> (value));
	std::vector<double> values;
	FlatEnumerate (value, [&] (const ValueConstPtr& v) {
		values.push_back (NumberValue::ToDouble (v));
		return true;
	});
	ASSERT (values == std::vector<double> ({ 5.0, 4.0, 3.0, 2.0, 1.0, 0.0, 1.0, 2.0, 3.0, 4.0 }));
}

}
