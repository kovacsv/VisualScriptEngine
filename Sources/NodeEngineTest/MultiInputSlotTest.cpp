#include "SimpleTest.hpp"
#include "TestNodes.hpp"
#include "NE_NodeManager.hpp"
#include "NE_SingleValues.hpp"

using namespace NE;

namespace MultiInputSlotTest
{

class IntegerNode : public SerializableTestNode
{
public:
	IntegerNode (int val) :
		SerializableTestNode (),
		val (val)
	{

	}

	virtual void Initialize () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValueConstPtr (new IntValue (val));
	}

private:
	int val;
};

class IntegerListNode : public SerializableTestNode
{
public:
	IntegerListNode (const std::vector<int>& arr) :
		SerializableTestNode (),
		arr (arr)
	{

	}

	virtual void Initialize () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
	{
		ListValuePtr result (new ListValue ());
		for (int val : arr) {
			result->Push (ValuePtr (new IntValue (val)));
		}
		return result;
	}

private:
	std::vector<int> arr;
};

class FlattenerNode : public SerializableTestNode
{
public:
	FlattenerNode () :
		SerializableTestNode ()
	{

	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("in"), nullptr, OutputSlotConnectionMode::Multiple)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr val = EvaluateInputSlot (SlotId ("in"), env);
		ListValuePtr result (new ListValue ());
		FlatEnumerate (val, [&] (const ValueConstPtr& flatVal) {
			result->Push (flatVal->Clone ());
		});
		return result;
	}
};

TEST (FlattenerNodeTest_SingleValue)
{
	NodeManager manager;

	NodePtr integerNode (new IntegerNode (1));
	NodePtr flattenerNode (new FlattenerNode ());

	manager.AddNode (integerNode);
	manager.AddNode (flattenerNode);
	manager.ConnectOutputSlotToInputSlot (integerNode->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));

	ValueConstPtr val = flattenerNode->Evaluate (EmptyEvaluationEnv);
	ListValueConstPtr listVal = Value::Cast<ListValue> (val);
	ASSERT (listVal->GetSize () == 1);
	ASSERT (NE::IntValue::Get (listVal->GetValue (0)) == 1);
}

TEST (FlattenerNodeTest_MultiValue)
{
	NodeManager manager;

	NodePtr integerNode1 (new IntegerNode (1));
	NodePtr integerNode2 (new IntegerNode (2));
	NodePtr flattenerNode (new FlattenerNode ());

	manager.AddNode (integerNode1);
	manager.AddNode (integerNode2);
	manager.AddNode (flattenerNode);
	manager.ConnectOutputSlotToInputSlot (integerNode1->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (integerNode2->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));

	ValueConstPtr val = flattenerNode->Evaluate (EmptyEvaluationEnv);
	ListValueConstPtr listVal = Value::Cast<ListValue> (val);
	ASSERT (listVal->GetSize () == 2);
	ASSERT (NE::IntValue::Get (listVal->GetValue (0)) == 1);
	ASSERT (NE::IntValue::Get (listVal->GetValue (1)) == 2);
}

TEST (FlattenerNodeTest_SingleListValue)
{
	NodeManager manager;

	NodePtr integerListNode (new IntegerListNode ({1, 2}));
	NodePtr flattenerNode (new FlattenerNode ());

	manager.AddNode (integerListNode);
	manager.AddNode (flattenerNode);
	manager.ConnectOutputSlotToInputSlot (integerListNode->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));

	ValueConstPtr val = flattenerNode->Evaluate (EmptyEvaluationEnv);
	ListValueConstPtr listVal = Value::Cast<ListValue> (val);
	ASSERT (listVal->GetSize () == 2);
	ASSERT (NE::IntValue::Get (listVal->GetValue (0)) == 1);
	ASSERT (NE::IntValue::Get (listVal->GetValue (1)) == 2);
}

TEST (FlattenerNodeTest_MultiListValue)
{
	NodeManager manager;

	NodePtr integerListNode1 (new IntegerListNode ({1, 2}));
	NodePtr integerListNode2 (new IntegerListNode ({3, 4}));
	NodePtr flattenerNode (new FlattenerNode ());

	manager.AddNode (integerListNode1);
	manager.AddNode (integerListNode2);
	manager.AddNode (flattenerNode);
	manager.ConnectOutputSlotToInputSlot (integerListNode1->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (integerListNode2->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));

	ValueConstPtr val = flattenerNode->Evaluate (EmptyEvaluationEnv);
	ListValueConstPtr listVal = Value::Cast<ListValue> (val);
	ASSERT (listVal->GetSize () == 4);
	ASSERT (NE::IntValue::Get (listVal->GetValue (0)) == 1);
	ASSERT (NE::IntValue::Get (listVal->GetValue (1)) == 2);
	ASSERT (NE::IntValue::Get (listVal->GetValue (2)) == 3);
	ASSERT (NE::IntValue::Get (listVal->GetValue (3)) == 4);
}

TEST (FlattenerNodeTest_MixedSingleAndListValue)
{
	NodeManager manager;

	NodePtr integerNode1 (new IntegerNode (1));
	NodePtr integerNode2 (new IntegerNode (2));
	NodePtr integerListNode1 (new IntegerListNode ({3, 4}));
	NodePtr integerListNode2 (new IntegerListNode ({5, 6}));
	NodePtr flattenerNode (new FlattenerNode ());

	manager.AddNode (integerNode1);
	manager.AddNode (integerNode2);
	manager.AddNode (integerListNode1);
	manager.AddNode (integerListNode2);
	manager.AddNode (flattenerNode);
	manager.ConnectOutputSlotToInputSlot (integerListNode1->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (integerNode1->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (integerListNode2->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (integerNode2->GetOutputSlot (SlotId ("out")), flattenerNode->GetInputSlot (SlotId ("in")));

	ValueConstPtr val = flattenerNode->Evaluate (EmptyEvaluationEnv);
	ListValueConstPtr listVal = Value::Cast<ListValue> (val);
	ASSERT (listVal->GetSize () == 6);
	ASSERT (NE::IntValue::Get (listVal->GetValue (0)) == 3);
	ASSERT (NE::IntValue::Get (listVal->GetValue (1)) == 4);
	ASSERT (NE::IntValue::Get (listVal->GetValue (2)) == 1);
	ASSERT (NE::IntValue::Get (listVal->GetValue (3)) == 5);
	ASSERT (NE::IntValue::Get (listVal->GetValue (4)) == 6);
	ASSERT (NE::IntValue::Get (listVal->GetValue (5)) == 2);
}

}
