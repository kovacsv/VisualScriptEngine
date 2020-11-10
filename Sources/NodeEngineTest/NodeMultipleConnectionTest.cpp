#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestNodes.hpp"

using namespace NE;

namespace NodeMultipleConnectionTest
{

class TestOutputSlotList : public OutputSlotList
{
public:
	TestOutputSlotList (const std::vector<OutputSlotConstPtr>& slots) :
		slots (slots)
	{

	}

	virtual ~TestOutputSlotList ()
	{

	}

	virtual size_t GetSize () const override
	{
		return slots.size ();
	}

	virtual void Enumerate (const std::function<bool (const OutputSlotConstPtr&)>& processor) const override
	{
		for (const OutputSlotConstPtr& slot : slots) {
			if (!processor (slot)) {
				break;
			}
		}
	}

private:
	std::vector<OutputSlotConstPtr> slots;
};

class TestInputSlotList : public InputSlotList
{
public:
	TestInputSlotList (const std::vector<InputSlotConstPtr>& slots) :
		slots (slots)
	{

	}

	virtual ~TestInputSlotList ()
	{

	}

	virtual size_t GetSize () const override
	{
		return slots.size ();
	}

	virtual void Enumerate (const std::function<bool (const InputSlotConstPtr&)>& processor) const override
	{
		for (const InputSlotConstPtr& slot : slots) {
			if (!processor (slot)) {
				break;
			}
		}
	}

private:
	std::vector<InputSlotConstPtr> slots;
};

class SlotTypesTestNode : public SerializableTestNode
{
	DYNAMIC_SERIALIZABLE (SlotTypesTestNode);

public:
	SlotTypesTestNode () :
		SlotTypesTestNode (0)
	{

	}

	SlotTypesTestNode (int myValue) :
		SerializableTestNode (),
		myValue (myValue)
	{

	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("disabled"), ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Disabled)));
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("single"), ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Single)));
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("multiple"), ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Multiple)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr disabledValue = EvaluateInputSlot (SlotId ("disabled"), env);
		ValueConstPtr singleValue = EvaluateInputSlot (SlotId ("single"), env);
		ValueConstPtr multipleValue = EvaluateInputSlot (SlotId ("multiple"), env);
		int result = myValue;
		FlatEnumerate (disabledValue, [&] (const ValueConstPtr& val) {
			DBGASSERT (Value::IsType<IntValue> (val));
			result += IntValue::Get (val);
			return true;
		});
		FlatEnumerate (singleValue, [&] (const ValueConstPtr& val) {
			DBGASSERT (Value::IsType<IntValue> (val));
			result += IntValue::Get (val);
			return true;
		});
		FlatEnumerate (multipleValue, [&] (const ValueConstPtr& val) {
			DBGASSERT (Value::IsType<IntValue> (val));
			result += IntValue::Get (val);
			return true;
		});
		return ValuePtr (new IntValue (result));
	}

	virtual NE::Stream::Status Read (NE::InputStream& inputStream) override
	{
		SerializableTestNode::Read (inputStream);
		return NE::Stream::Status::NoError;
	}

	virtual NE::Stream::Status Write (NE::OutputStream& outputStream) const override
	{
		SerializableTestNode::Write (outputStream);
		return NE::Stream::Status::NoError;
	}

private:
	int myValue;
};

DYNAMIC_SERIALIZATION_INFO (SlotTypesTestNode, 1, "{50CCFA76-48EE-44D7-92CD-805D52A8687C}");

static bool CheckNodeValue (const NodePtr& node, int expected)
{
	NE::ValueConstPtr result = node->Evaluate (EmptyEvaluationEnv);
	if (!Value::IsType<IntValue> (result)) {
		return false;
	}
	return IntValue::Get (result) == expected;
}

TEST (MultipleConnectionTest_NoConnection)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	ASSERT (CheckNodeValue (node, 1));
}

TEST (MultipleOutputConnectionTest_SingleConnections)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	NodePtr outputNode1 = manager.AddNode (NodePtr (new SlotTypesTestNode (2)));
	NodePtr outputNode2 = manager.AddNode (NodePtr (new SlotTypesTestNode (3)));
	
	ASSERT (!manager.CanConnectOutputSlotToInputSlot (outputNode1->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("disabled"))));
	
	ASSERT (manager.CanConnectOutputSlotToInputSlot (outputNode1->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("single"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (outputNode1->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("single"))));
	ASSERT (CheckNodeValue (node, 3));

	ASSERT (!manager.CanConnectOutputSlotToInputSlot (outputNode1->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("single"))));
	ASSERT (manager.CanConnectOutputSlotToInputSlot (outputNode2->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("single"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (outputNode2->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("single"))));
	ASSERT (CheckNodeValue (node, 4));

	ASSERT (manager.CanConnectOutputSlotToInputSlot (outputNode1->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (outputNode1->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 6));

	ASSERT (!manager.CanConnectOutputSlotToInputSlot (outputNode1->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.CanConnectOutputSlotToInputSlot (outputNode2->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (outputNode2->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 9));
}

TEST (MultipleOutputConnectionTest_MultipleConnections)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	NodePtr outputNode1 = manager.AddNode (NodePtr (new SlotTypesTestNode (2)));
	NodePtr outputNode2 = manager.AddNode (NodePtr (new SlotTypesTestNode (3)));

	TestOutputSlotList outputSlotsSingle1 ({
		outputNode1->GetOutputSlot (SlotId ("out"))
	});
	TestOutputSlotList outputSlotsSingle2 ({
		outputNode2->GetOutputSlot (SlotId ("out"))
	});
	TestOutputSlotList outputSlotsMultiple ({
		outputNode1->GetOutputSlot (SlotId ("out")),
		outputNode2->GetOutputSlot (SlotId ("out"))
	});

	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("disabled"))));
	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsMultiple, node->GetInputSlot (SlotId ("disabled"))));
	
	ASSERT (manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("single"))));
	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsMultiple, node->GetInputSlot (SlotId ("single"))));
	ASSERT (manager.ConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("single"))));
	ASSERT (CheckNodeValue (node, 3));

	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("single"))));
	ASSERT (manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle2, node->GetInputSlot (SlotId ("single"))));
	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsMultiple, node->GetInputSlot (SlotId ("single"))));
	ASSERT (manager.ConnectOutputSlotsToInputSlot (outputSlotsSingle2, node->GetInputSlot (SlotId ("single"))));
	ASSERT (CheckNodeValue (node, 4));

	ASSERT (manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle2, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.CanConnectOutputSlotsToInputSlot (outputSlotsMultiple, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.ConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 6));

	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle2, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsMultiple, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.ConnectOutputSlotsToInputSlot (outputSlotsSingle2, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 9));

	ASSERT (manager.DisconnectAllOutputSlotsFromInputSlot (node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 4));

	ASSERT (manager.CanConnectOutputSlotsToInputSlot (outputSlotsMultiple, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.ConnectOutputSlotsToInputSlot (outputSlotsMultiple, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 9));
}

TEST (MultipleOutputConnectionTest_MultipleConnectionsCycle)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	NodePtr outputNode1 = manager.AddNode (NodePtr (new SlotTypesTestNode (2)));
	NodePtr outputNode2 = manager.AddNode (NodePtr (new SlotTypesTestNode (3)));

	TestOutputSlotList outputSlotsSingle1 ({
		outputNode1->GetOutputSlot (SlotId ("out"))
	});
	TestOutputSlotList outputSlotsSingle2 ({
		outputNode2->GetOutputSlot (SlotId ("out"))
	});
	TestOutputSlotList outputSlotsMultiple ({
		outputNode1->GetOutputSlot (SlotId ("out")),
		outputNode2->GetOutputSlot (SlotId ("out"))
	});

	ASSERT (manager.ConnectOutputSlotToInputSlot (node->GetOutputSlot (SlotId ("out")), outputNode2->GetInputSlot (SlotId ("single"))));
	ASSERT (CheckNodeValue (node, 1));

	ASSERT (manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle2, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsMultiple, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (manager.ConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle1, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsSingle2, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsMultiple, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 3));
}

TEST (MultipleOutputConnectionTest_MultipleConnectionsDuplicated)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	NodePtr outputNode1 = manager.AddNode (NodePtr (new SlotTypesTestNode (2)));
	NodePtr outputNode2 = manager.AddNode (NodePtr (new SlotTypesTestNode (3)));

	TestOutputSlotList outputSlotsDuplicated ({
		outputNode1->GetOutputSlot (SlotId ("out")),
		outputNode1->GetOutputSlot (SlotId ("out"))
	});

	ASSERT (!manager.CanConnectOutputSlotsToInputSlot (outputSlotsDuplicated, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 1));
}

TEST (MultipleOutputConnectionTest_MultipleDisconnect)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	NodePtr outputNode1 = manager.AddNode (NodePtr (new SlotTypesTestNode (2)));
	NodePtr outputNode2 = manager.AddNode (NodePtr (new SlotTypesTestNode (3)));

	TestOutputSlotList outputSlots ({
		outputNode1->GetOutputSlot (SlotId ("out")),
		outputNode2->GetOutputSlot (SlotId ("out"))
	});

	ASSERT (manager.ConnectOutputSlotsToInputSlot (outputSlots, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 6));

	ASSERT (manager.DisconnectOutputSlotsFromInputSlot (outputSlots, node->GetInputSlot (SlotId ("multiple"))));
	ASSERT (CheckNodeValue (node, 1));
}

TEST (MultipleInputConnectionTest_MultipleConnections)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	NodePtr inputNode1 = manager.AddNode (NodePtr (new SlotTypesTestNode (2)));
	NodePtr inputNode2 = manager.AddNode (NodePtr (new SlotTypesTestNode (3)));

	TestInputSlotList inputSlotsSingle1 ({
		inputNode1->GetInputSlot (SlotId ("multiple"))
	});
	TestInputSlotList inputSlotsSingle2 ({
		inputNode2->GetInputSlot (SlotId ("multiple"))
	});
	TestInputSlotList inputSlotsMultiple ({
		inputNode1->GetInputSlot (SlotId ("multiple")),
		inputNode2->GetInputSlot (SlotId ("multiple"))
	});

	ASSERT (manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle1));
	ASSERT (manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle2));
	ASSERT (manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsMultiple));
	ASSERT (manager.ConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle1));
	ASSERT (CheckNodeValue (inputNode1, 3));

	ASSERT (!manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle1));
	ASSERT (manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle2));
	ASSERT (!manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsMultiple));

	ASSERT (manager.DisconnectAllInputSlotsFromOutputSlot (node->GetOutputSlot (SlotId ("out"))));

	ASSERT (manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsMultiple));
	ASSERT (manager.ConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsMultiple));
	ASSERT (CheckNodeValue (inputNode1, 3));
	ASSERT (CheckNodeValue (inputNode2, 4));
}

TEST (MultipleInputConnectionTest_MultipleConnectionsCycle)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	NodePtr inputNode1 = manager.AddNode (NodePtr (new SlotTypesTestNode (2)));
	NodePtr inputNode2 = manager.AddNode (NodePtr (new SlotTypesTestNode (3)));

	TestInputSlotList inputSlotsSingle1 ({
		inputNode1->GetInputSlot (SlotId ("multiple"))
	});
	TestInputSlotList inputSlotsSingle2 ({
		inputNode2->GetInputSlot (SlotId ("multiple"))
	});
	TestInputSlotList inputSlotsMultiple ({
		inputNode1->GetInputSlot (SlotId ("multiple")),
		inputNode2->GetInputSlot (SlotId ("multiple"))
	});

	ASSERT (manager.ConnectOutputSlotToInputSlot (inputNode2->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("single"))));
	ASSERT (manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle1));
	ASSERT (!manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle2));
	ASSERT (!manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsMultiple));
	ASSERT (manager.ConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle1));

	ASSERT (!manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle1));
	ASSERT (!manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsSingle2));
	ASSERT (!manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsMultiple));
}

TEST (MultipleInputConnectionTest_MultipleConnectionsDuplicated)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	NodePtr inputNode1 = manager.AddNode (NodePtr (new SlotTypesTestNode (2)));
	NodePtr inputNode2 = manager.AddNode (NodePtr (new SlotTypesTestNode (3)));

	TestInputSlotList inputSlotsDuplicated ({
		inputNode1->GetInputSlot (SlotId ("multiple")),
		inputNode1->GetInputSlot (SlotId ("multiple"))
	});

	ASSERT (!manager.CanConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlotsDuplicated));
}

TEST (MultipleInputConnectionTest_MultipleDisconnect)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new SlotTypesTestNode (1)));
	NodePtr inputNode1 = manager.AddNode (NodePtr (new SlotTypesTestNode (2)));
	NodePtr inputNode2 = manager.AddNode (NodePtr (new SlotTypesTestNode (3)));

	TestInputSlotList inputSlots ({
		inputNode1->GetInputSlot (SlotId ("multiple")),
		inputNode2->GetInputSlot (SlotId ("multiple"))
	});

	ASSERT (manager.ConnectOutputSlotToInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlots));
	ASSERT (CheckNodeValue (inputNode1, 3));
	ASSERT (CheckNodeValue (inputNode2, 4));

	ASSERT (manager.DisconnectOutputSlotFromInputSlots (node->GetOutputSlot (SlotId ("out")), inputSlots));
	ASSERT (CheckNodeValue (inputNode1, 2));
	ASSERT (CheckNodeValue (inputNode2, 3));
}

}
