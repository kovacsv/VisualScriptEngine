#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestNodes.hpp"

using namespace NE;

namespace ListValueNodeTest
{

class MultiInputSlot : public InputSlot
{
public:
	MultiInputSlot (const SlotId& id, const ValuePtr& defaultValue) :
		InputSlot (id, defaultValue, OutputSlotConnectionMode::Multiple)
	{
	}

	~MultiInputSlot ()
	{

	}
};

class IntegerInputNode : public SerializableTestNode
{
public:
	IntegerInputNode (int val) :
		SerializableTestNode (),
		val (val)
	{
	
	}

	virtual void Initialize () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}
	
	virtual ValuePtr Calculate (NE::EvaluationEnv&) const
	{
		return ValuePtr (new IntValue (val));
	}

	int val;
};

class ListMakerNode : public SerializableTestNode
{
public:
	ListMakerNode () :
		SerializableTestNode ()
	{
	
	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new MultiInputSlot (SlotId ("in"), ValuePtr (new IntValue (0)))));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}
	
	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const
	{
		std::vector<int> result;
		return EvaluateInputSlot (SlotId ("in"), env);
	}
};

class ListSummer : public SerializableTestNode
{
public:
	ListSummer () :
		SerializableTestNode ()
	{
	
	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("in"), ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Single)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}
	
	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const
	{
		NE::ValuePtr val = EvaluateSingleInputSlot (SlotId ("in"), env);
		ListValue* input = Value::Cast<ListValue> (val.get ());
		int result = 0;
		input->Enumerate ([&] (const ValuePtr& value) {
			result += IntValue::Get (value);
		});
		return ValuePtr (new IntValue (result));
	}
};

TEST (ListValueNodeTest)
{
	NodeManager manager;

	NodePtr inputNode1 = manager.AddNode (NodePtr (new IntegerInputNode (1)));
	NodePtr inputNode2 = manager.AddNode (NodePtr (new IntegerInputNode (2)));
	NodePtr inputNode3 = manager.AddNode (NodePtr (new IntegerInputNode (3)));
	NodePtr listMakerNode = manager.AddNode (NodePtr (new ListMakerNode ()));
	NodePtr listSummerNode = manager.AddNode (NodePtr (new ListSummer ()));

	ASSERT (manager.ConnectOutputSlotToInputSlot (inputNode1->GetOutputSlot (SlotId ("out")), listMakerNode->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (inputNode2->GetOutputSlot (SlotId ("out")), listMakerNode->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (inputNode3->GetOutputSlot (SlotId ("out")), listMakerNode->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (listMakerNode->GetOutputSlot (SlotId ("out")), listSummerNode->GetInputSlot (SlotId ("in"))));

	{
		ValuePtr result = listMakerNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (Value::IsType<ListValue> (result));
	}

	{
		ValuePtr result = listSummerNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 6);
	}
}

}
