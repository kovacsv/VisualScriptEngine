#include "SimpleTest.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "BI_BinaryOperationNodes.hpp"
#include "BI_InputUINodes.hpp"
#include "TestUtils.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace BinaryOperationNodesTest
{

static double GetBinaryOperationResult (double a, double b, const std::function<UINodePtr ()>& nodeCreator)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val1 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value1"), Point (0, 0), a, 1.0)), EmptyEvaluationEnv);
	UINodePtr val2 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value1"), Point (0, 0), b, 1.0)), EmptyEvaluationEnv);
	UINodePtr op = uiManager.AddNode (nodeCreator (), EmptyEvaluationEnv);
	uiManager.ConnectOutputSlotToInputSlot (val1->GetUIOutputSlot (SlotId ("out")), op->GetUIInputSlot (SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (val2->GetUIOutputSlot (SlotId ("out")), op->GetUIInputSlot (SlotId ("b")));

	ValueConstPtr result = op->Evaluate (EmptyEvaluationEnv);
	return DoubleValue::Get (result);
}

TEST (TestAdditionNode)
{
	double result = GetBinaryOperationResult (2.0, 3.0, [&] () {
		return UINodePtr (new AdditionNode (LocString (L"Addition"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 5.0));
}

TEST (TestAdditionNodeWithList)
{
	TestUIEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val1 = uiManager.AddNode (UINodePtr (new DoubleIncrementedNode (LocString (L"Value1"), Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr val2 = uiManager.AddNode (UINodePtr (new DoubleIncrementedNode (LocString (L"Value2"), Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr op = uiManager.AddNode (UINodePtr (new AdditionNode (LocString (L"Addition"), Point (0, 0))), EmptyEvaluationEnv);
	uiManager.ConnectOutputSlotToInputSlot (val1->GetUIOutputSlot (SlotId ("out")), op->GetUIInputSlot (SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (val2->GetUIOutputSlot (SlotId ("out")), op->GetUIInputSlot (SlotId ("b")));

	ValueConstPtr val = op->Evaluate (EmptyEvaluationEnv);
	ASSERT (Value::IsType<ListValue> (val));
	ASSERT (IsComplexType<NumberValue> (val));
	std::vector<double> values;
	FlatEnumerate (val, [&] (const ValueConstPtr& v) {
		values.push_back (NumberValue::ToDouble (v));
		return true;
	});
	ASSERT (values == std::vector<double> ({ 0.0, 2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0 }));
}

TEST (TestSubtractionNode)
{
	double result = GetBinaryOperationResult (2.0, 3.0, [&] () {
		return UINodePtr (new SubtractionNode (LocString (L"Subtraction"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, -1.0));
}

TEST (TestMultiplicationNode)
{
	double result = GetBinaryOperationResult (2.0, 3.0, [&] () {
		return UINodePtr (new MultiplicationNode (LocString (L"Multiplication"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 6.0));
}

TEST (TestDivisionNode)
{
	double result = GetBinaryOperationResult (2.0, 3.0, [&] () {
		return UINodePtr (new DivisionNode (LocString (L"Division"), Point (0, 0)));
	});
	ASSERT (IsEqual (result, 2.0 / 3.0));
}

}
