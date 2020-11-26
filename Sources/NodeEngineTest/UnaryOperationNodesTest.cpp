#include "SimpleTest.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "BI_UnaryOperationNodes.hpp"
#include "BI_InputUINodes.hpp"
#include "TestUtils.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace UnaryOperationNodesTest
{

static double GetUnaryOperationResult (double a, const std::function<UINodePtr ()>& nodeCreator)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value"), Point (0, 0), a, 1.0)));
	UINodePtr op = uiManager.AddNode (nodeCreator ());
	uiManager.ConnectOutputSlotToInputSlot (val->GetUIOutputSlot (SlotId ("out")), op->GetUIInputSlot (SlotId ("a")));

	ValueConstPtr result = op->Evaluate (EmptyEvaluationEnv);
	return DoubleValue::Get (result);
}

TEST (TestAbsoluteNode)
{
	double result = GetUnaryOperationResult (-2.0, [&] () {
		return UINodePtr (new AbsNode (LocString (L"Abs"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 2.0));
}

TEST (TestAbsoluteNodeWithList)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value"), Point (0, 0), -5.0, 1.0)));
	UINodePtr listVal = uiManager.AddNode (UINodePtr (new DoubleIncrementedNode (LocString (L"Value"), Point (0, 0))));
	UINodePtr op = uiManager.AddNode (UINodePtr (new AbsNode (LocString (L"Abs"), Point (0, 0))));
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
	double result = GetUnaryOperationResult (2.3, [&] () {
		return UINodePtr (new FloorNode (LocString (L"Floor"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 2.0));
}

TEST (TestCeilNode)
{
	double result = GetUnaryOperationResult (2.3, [&] () {
		return UINodePtr (new CeilNode (LocString (L"Ceil"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 3.0));
}

TEST (TestNegativeNode)
{
	double result = GetUnaryOperationResult (2.0, [&] () {
		return UINodePtr (new NegativeNode (LocString (L"Negative"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, -2.0));
}

TEST (TestSqrtNode)
{
	double result = GetUnaryOperationResult (9.0, [&] () {
		return UINodePtr (new SqrtNode (LocString (L"Sqrt"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 3.0));
}

}
