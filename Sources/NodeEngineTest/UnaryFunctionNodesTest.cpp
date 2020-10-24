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

static double GetUnaryFunctionResult (double a, const std::function<UINodePtr ()>& nodeCreator)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value"), Point (0, 0), a, 1.0)), EmptyEvaluationEnv);
	UINodePtr op = uiManager.AddNode (nodeCreator (), EmptyEvaluationEnv);
	uiManager.ConnectOutputSlotToInputSlot (val->GetUIOutputSlot (SlotId ("out")), op->GetUIInputSlot (SlotId ("a")));

	ValueConstPtr result = op->Evaluate (EmptyEvaluationEnv);
	return DoubleValue::Get (result);
}

TEST (TestAbsoluteNode)
{
	double result = GetUnaryFunctionResult (-2.0, [&] () {
		return UINodePtr (new AbsoluteNode (LocString (L"AbsoluteValue"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 2.0));
}

TEST (TestAbsoluteNodeWithList)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value"), Point (0, 0), -5.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr listVal = uiManager.AddNode (UINodePtr (new DoubleIncrementedNode (LocString (L"Value"), Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr op = uiManager.AddNode (UINodePtr (new AbsoluteNode (LocString (L"AbsoluteValue"), Point (0, 0))), EmptyEvaluationEnv);
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

TEST (TestFloorNode)
{
	double result = GetUnaryFunctionResult (2.3, [&] () {
		return UINodePtr (new FloorNode (LocString (L"Floor"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 2.0));
}

TEST (TestCeilNode)
{
	double result = GetUnaryFunctionResult (2.3, [&] () {
		return UINodePtr (new CeilNode (LocString (L"Ceil"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 3.0));
}

}
