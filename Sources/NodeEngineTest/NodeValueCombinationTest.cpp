#include "SimpleTest.hpp"
#include "NE_ValueCombination.hpp"
#include "NE_SingleValues.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "TestNodes.hpp"

using namespace NE;

namespace NodeValueCombinationTest
{

class TestNode : public SerializableTestNode
{
public:
	TestNode (NE::ValueCombinationMode combinationMode, ValuePtr aValue, ValuePtr bValue) :
		SerializableTestNode (),
		combinationMode (combinationMode),
		aValue (aValue),
		bValue (bValue)
	{

	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("a"), aValue, OutputSlotConnectionMode::Single)));
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("b"), bValue, OutputSlotConnectionMode::Single)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		NE::ValueConstPtr a = EvaluateInputSlot (NE::SlotId ("a"), env);
		NE::ValueConstPtr b = EvaluateInputSlot (NE::SlotId ("b"), env);
		if (!NE::IsComplexType<NE::IntValue> (a) || !NE::IsComplexType<NE::IntValue> (b)) {
			return nullptr;
		}
		NE::ListValuePtr result (new NE::ListValue ());
		NE::CombineValues (combinationMode, { a, b }, [&] (const NE::ValueCombination& combination) {
			result->Push (NE::ValuePtr (new IntValue (NE::IntValue::Get (combination.GetValue (0)) + NE::IntValue::Get (combination.GetValue (1)))));
			return true;
		});
		return result;
	}

private:
	NE::ValueCombinationMode	combinationMode;
	ValuePtr					aValue;
	ValuePtr					bValue;
};

TEST (TestNodeValueCombination_Shortest)
{
	NodeManager manager;

	{
		NodePtr simpleIntValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::Shortest, ValuePtr (new IntValue (1)), ValuePtr (new IntValue (2)))));
		ValueConstPtr result = simpleIntValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 1);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 3);
	}

	{
		ListValuePtr listValue (new ListValue ());
		listValue->Push (ValuePtr (new IntValue (2)));
		NodePtr intAndIntListValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::Shortest, ValuePtr (new IntValue (1)), listValue)));
		ValueConstPtr result = intAndIntListValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 1);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 3);
	}

	{
		ListValuePtr listValue (new ListValue ());
		listValue->Push (ValuePtr (new IntValue (2)));
		listValue->Push (ValuePtr (new IntValue (3)));
		NodePtr intAndIntListValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::Shortest, ValuePtr (new IntValue (1)), listValue)));
		ValueConstPtr result = intAndIntListValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 1);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 3);
	}

	{
		ListValuePtr listValue1 (new ListValue ());
		listValue1->Push (ValuePtr (new IntValue (1)));
		listValue1->Push (ValuePtr (new IntValue (2)));
		ListValuePtr listValue2 (new ListValue ());
		listValue2->Push (ValuePtr (new IntValue (3)));
		listValue2->Push (ValuePtr (new IntValue (4)));
		listValue2->Push (ValuePtr (new IntValue (5)));
		NodePtr twoListValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::Shortest, listValue1, listValue2)));
		ValueConstPtr result = twoListValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 2);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 4);
		ASSERT (IntValue::Get (resultListValue->GetValue (1)) == 6);
	}
}

TEST (TestNodeValueCombination_Longest)
{
	NodeManager manager;

	{
		NodePtr simpleIntValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::Longest, ValuePtr (new IntValue (1)), ValuePtr (new IntValue (2)))));
		ValueConstPtr result = simpleIntValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 1);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 3);
	}

	{
		ListValuePtr listValue (new ListValue ());
		listValue->Push (ValuePtr (new IntValue (2)));
		NodePtr intAndIntListValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::Longest, ValuePtr (new IntValue (1)), listValue)));
		ValueConstPtr result = intAndIntListValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 1);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 3);
	}

	{
		ListValuePtr listValue (new ListValue ());
		listValue->Push (ValuePtr (new IntValue (2)));
		listValue->Push (ValuePtr (new IntValue (3)));
		NodePtr intAndIntListValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::Longest, ValuePtr (new IntValue (1)), listValue)));
		ValueConstPtr result = intAndIntListValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 2);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 3);
		ASSERT (IntValue::Get (resultListValue->GetValue (1)) == 4);
	}

	{
		ListValuePtr listValue1 (new ListValue ());
		listValue1->Push (ValuePtr (new IntValue (1)));
		listValue1->Push (ValuePtr (new IntValue (2)));
		ListValuePtr listValue2 (new ListValue ());
		listValue2->Push (ValuePtr (new IntValue (3)));
		listValue2->Push (ValuePtr (new IntValue (4)));
		listValue2->Push (ValuePtr (new IntValue (5)));
		NodePtr twoListValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::Longest, listValue1, listValue2)));
		ValueConstPtr result = twoListValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 3);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 4);
		ASSERT (IntValue::Get (resultListValue->GetValue (1)) == 6);
		ASSERT (IntValue::Get (resultListValue->GetValue (2)) == 7);
	}
}

TEST (TestNodeValueCombination_CrossProduct)
{
	NodeManager manager;

	{
		NodePtr nullInputNode = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::CrossProduct, nullptr, nullptr)));
		ASSERT (nullInputNode->Evaluate (EmptyEvaluationEnv) == nullptr);
	}

	{
		NodePtr notIntegerInputNode = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::CrossProduct, ValuePtr (new IntValue (1)), ValuePtr (new DoubleValue (1.2)))));
		ASSERT (notIntegerInputNode->Evaluate (EmptyEvaluationEnv) == nullptr);
	}

	{
		ListValuePtr listValue (new ListValue ());
		listValue->Push (ValuePtr (new DoubleValue (1.2)));
		NodePtr notIntegerListInputNode = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::CrossProduct, ValuePtr (new IntValue (1)), listValue)));
		ASSERT (notIntegerListInputNode->Evaluate (EmptyEvaluationEnv) == nullptr);
	}

	{
		ListValuePtr listValue (new ListValue ());
		NodePtr intAndEmptyListValue = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::CrossProduct, ValuePtr (new IntValue (1)), listValue)));
		ASSERT (intAndEmptyListValue->Evaluate (EmptyEvaluationEnv) == nullptr);
	}

	{
		NodePtr simpleIntValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::CrossProduct, ValuePtr (new IntValue (1)), ValuePtr (new IntValue (2)))));
		ValueConstPtr result = simpleIntValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 1);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 3);
	}

	{
		ListValuePtr listValue (new ListValue ());
		listValue->Push (ValuePtr (new IntValue (2)));
		NodePtr intAndIntListValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::CrossProduct, ValuePtr (new IntValue (1)), listValue)));
		ValueConstPtr result = intAndIntListValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 1);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 3);
	}

	{
		ListValuePtr listValue (new ListValue ());
		listValue->Push (ValuePtr (new IntValue (2)));
		listValue->Push (ValuePtr (new IntValue (3)));
		NodePtr intAndIntListValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::CrossProduct, ValuePtr (new IntValue (1)), listValue)));
		ValueConstPtr result = intAndIntListValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 2);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 3);
		ASSERT (IntValue::Get (resultListValue->GetValue (1)) == 4);
	}

	{
		ListValuePtr listValue1 (new ListValue ());
		listValue1->Push (ValuePtr (new IntValue (1)));
		listValue1->Push (ValuePtr (new IntValue (2)));
		ListValuePtr listValue2 (new ListValue ());
		listValue2->Push (ValuePtr (new IntValue (3)));
		listValue2->Push (ValuePtr (new IntValue (4)));
		listValue2->Push (ValuePtr (new IntValue (5)));
		NodePtr twoListValues = manager.AddNode (NodePtr (new TestNode (NE::ValueCombinationMode::CrossProduct, listValue1, listValue2)));
		ValueConstPtr result = twoListValues->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
		ListValueConstPtr resultListValue = Value::Cast<ListValue> (result);
		ASSERT (resultListValue->GetSize () == 6);
		ASSERT (IntValue::Get (resultListValue->GetValue (0)) == 4);
		ASSERT (IntValue::Get (resultListValue->GetValue (1)) == 5);
		ASSERT (IntValue::Get (resultListValue->GetValue (2)) == 6);
		ASSERT (IntValue::Get (resultListValue->GetValue (3)) == 5);
		ASSERT (IntValue::Get (resultListValue->GetValue (4)) == 6);
		ASSERT (IntValue::Get (resultListValue->GetValue (5)) == 7);
	}
}

}
