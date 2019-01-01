#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestNodes.hpp"

using namespace NE;

namespace NodeConnectionTest
{

class DisabledInputSlot : public InputSlot
{
public:
	DisabledInputSlot (const SlotId& id, const ValuePtr& defaultValue) :
		InputSlot (id, defaultValue, OutputSlotConnectionMode::Disabled)
	{
	}

	~DisabledInputSlot ()
	{

	}
};

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

class InputNode : public SerializableTestNode
{
public:
	InputNode (int val) :
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

class InputNode2 : public SerializableTestNode
{
public:
	InputNode2 () :
		SerializableTestNode ()
	{
	
	}

	virtual void Initialize () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out1"))));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out2"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValueConstPtr (new IntValue (5));
	}
};

class InputNode3 : public SerializableTestNode
{
public:
	InputNode3 () :
		SerializableTestNode ()
	{
	
	}

	virtual void Initialize () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out1"))));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out2"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValueConstPtr (new IntValue (5));
	}
};

class TestInputSlot : public InputSlot
{
public:
	TestInputSlot (const SlotId& id) :
		InputSlot (id, ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Single)
	{

	}
};

class TestOutputSlot : public OutputSlot
{
public:
	TestOutputSlot (const SlotId& id) :
		OutputSlot (id)
	{

	}
};

class AdditionNode : public SerializableTestNode
{
public:
	AdditionNode () :
		SerializableTestNode ()
	{

	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new TestInputSlot (SlotId ("first"))));
		RegisterInputSlot (InputSlotPtr (new TestInputSlot (SlotId ("second"))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr firstResult = EvaluateSingleInputSlot (SlotId ("first"), env);
		ValueConstPtr secondResult = EvaluateSingleInputSlot (SlotId ("second"), env);
		return ValuePtr (new IntValue (IntValue::Get (firstResult) + IntValue::Get (secondResult)));
	}
};

class MultiAdditionNode : public SerializableTestNode
{
public:
	MultiAdditionNode () :
		SerializableTestNode ()
	{

	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new MultiInputSlot (SlotId ("in"), ValuePtr (new IntValue (0)))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ListValueConstPtr result = EvaluateInputSlot (SlotId ("in"), env);
		int sum = 0;
		result->Enumerate ([&] (const ValueConstPtr& val) {
			sum += IntValue::Get (val);
		});
		return ValuePtr (new IntValue (sum));
	}
};

class DisabledAdditionNode : public SerializableTestNode
{
public:
	DisabledAdditionNode () :
		SerializableTestNode ()
	{

	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new DisabledInputSlot (SlotId ("first"), ValuePtr (new IntValue (2)))));
		RegisterInputSlot (InputSlotPtr (new DisabledInputSlot (SlotId ("second"), ValuePtr (new IntValue (3)))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr firstResult = EvaluateSingleInputSlot (SlotId ("first"), env);
		ValueConstPtr secondResult = EvaluateSingleInputSlot (SlotId ("second"), env);
		return ValuePtr (new IntValue (IntValue::Get (firstResult) + IntValue::Get (secondResult)));
	}
};

class AdderInputOutputNode : public SerializableTestNode
{
public:
	AdderInputOutputNode (int toAdd = 1) :
		SerializableTestNode (),
		toAdd (toAdd)
	{

	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new TestInputSlot (SlotId ("in"))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr inputVal = EvaluateSingleInputSlot (SlotId ("in"), env);
		return ValuePtr (new IntValue (IntValue::Get (inputVal) + toAdd));
	}

	void SetToAdd (int newToAdd)
	{
		toAdd = newToAdd;
		InvalidateValue ();
	}

private:
	int toAdd;
};

TEST (SingleInputSlotConnectionTest)
{
	NodeManager manager;

	NodePtr firstNode (new InputNode (5));
	NodePtr secondNode (new InputNode (6));
	NodePtr additionNode (new AdditionNode ());

	manager.AddNode (firstNode);
	manager.AddNode (secondNode);
	manager.AddNode (additionNode);

	ASSERT (manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first"))));
	ASSERT (!manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first"))));
}

TEST (MultiInputSlotConnectionTest)
{
	NodeManager manager;

	NodePtr firstNode (new InputNode (5));
	NodePtr secondNode (new InputNode (6));
	NodePtr additionNode (new MultiAdditionNode ());

	manager.AddNode (firstNode);
	manager.AddNode (secondNode);
	manager.AddNode (additionNode);

	ASSERT (manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));
	ASSERT (!manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));
	ASSERT (!manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));

	ASSERT (manager.HasConnectedInputSlots (firstNode->GetOutputSlot (SlotId ("out"))));
	ASSERT (manager.GetConnectedInputSlotCount (firstNode->GetOutputSlot (SlotId ("out"))) == 1);

	ASSERT (manager.HasConnectedOutputSlots (additionNode->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.GetConnectedOutputSlotCount (additionNode->GetInputSlot (SlotId ("in"))) == 2);
}

TEST (MultiInputSlotEvaluation)
{
	NodeManager manager;

	NodePtr firstNode (new InputNode (5));
	NodePtr secondNode (new InputNode (6));
	NodePtr thirdNode (new InputNode (7));
	NodePtr additionNode (new MultiAdditionNode ());

	manager.AddNode (firstNode);
	manager.AddNode (secondNode);
	manager.AddNode (thirdNode);
	manager.AddNode (additionNode);

	{
		ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 0);
	}

	ASSERT (manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));
	{
		ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 5);
	}

	ASSERT (manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (thirdNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));
	{
		ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 18);
	}
}

TEST (AdditionNodeTest)
{
	NodeManager manager;

	NodePtr firstNode (new InputNode (5));
	NodePtr secondNode (new InputNode (6));
	NodePtr additionNode (new AdditionNode ());

	manager.AddNode (firstNode);
	manager.AddNode (secondNode);
	manager.AddNode (additionNode);

	manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first")));
	manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("second")));

	ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (result != nullptr);
	ASSERT (Value::IsType<IntValue> (result));
	ASSERT (IntValue::Get (result) == 11);
}

TEST (MultipleAdditionNodeTest)
{
	NodeManager manager;

	NodePtr firstNode (new InputNode (5));
	NodePtr secondNode (new InputNode (6));
	NodePtr additionNode (new AdditionNode ());
	NodePtr additionNode2 (new AdditionNode ());

	manager.AddNode (firstNode);
	manager.AddNode (secondNode);
	manager.AddNode (additionNode);
	manager.AddNode (additionNode2);

	manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first")));
	manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("second")));
	manager.ConnectOutputSlotToInputSlot (additionNode->GetOutputSlot (SlotId ("out")), additionNode2->GetInputSlot (SlotId ("first")));
	manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode2->GetInputSlot (SlotId ("second")));

	ValueConstPtr result = additionNode2->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (result != nullptr);
	ASSERT (Value::IsType<IntValue> (result));
	ASSERT (IntValue::Get (result) == 17);
}

TEST (DisabledNodeTest)
{
	NodeManager manager;

	NodePtr firstNode (new InputNode (5));
	NodePtr secondNode (new InputNode (6));
	NodePtr additionNode (new DisabledAdditionNode ());

	manager.AddNode (firstNode);
	manager.AddNode (secondNode);
	manager.AddNode (additionNode);

	ASSERT (!manager.CanConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first"))));
	ASSERT (!manager.CanConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("second"))));
	ASSERT (!manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first"))));
	ASSERT (!manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("second"))));

	ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (result != nullptr);
	ASSERT (Value::IsType<IntValue> (result));
	ASSERT (IntValue::Get (result) == 5);
}

TEST (ConnectSameOutputNodesTest)
{
	NodeManager manager;

	NodePtr firstNode (new InputNode (5));
	NodePtr additionNode (new AdditionNode ());

	manager.AddNode (firstNode);
	manager.AddNode (additionNode);

	manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first")));
	manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("second")));

	ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (result != nullptr);
	ASSERT (Value::IsType<IntValue> (result));
	ASSERT (IntValue::Get (result) == 10);
}

TEST (InputSlotDefaultValueTest)
{
	NodeManager manager;

	NodePtr firstNode (new InputNode (5));
	NodePtr additionNode (new AdditionNode ());

	manager.AddNode (firstNode);
	manager.AddNode (additionNode);

	manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first")));

	ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (result != nullptr);
	ASSERT (Value::IsType<IntValue> (result));
	ASSERT (IntValue::Get (result) == 5);
}

TEST (InputSlotDefaultValueTest2)
{
	NodeManager manager;

	NodePtr firstNode (new InputNode (5));
	NodePtr additionNode (new AdditionNode ());

	manager.AddNode (firstNode);
	manager.AddNode (additionNode);

	ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (result != nullptr);
	ASSERT (Value::IsType<IntValue> (result));
	ASSERT (IntValue::Get (result) == 0);
}

TEST (ChainedNodesTest)
{
	NodeManager manager;

	NodePtr node1 = manager.AddNode (NodePtr (new AdderInputOutputNode ()));
	NodePtr node2 = manager.AddNode (NodePtr (new AdderInputOutputNode ()));
	NodePtr node3 = manager.AddNode (NodePtr (new AdderInputOutputNode ()));
	NodePtr node4 = manager.AddNode (NodePtr (new AdderInputOutputNode ()));
	NodePtr node5 = manager.AddNode (NodePtr (new AdderInputOutputNode ()));

	manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node2->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (node2->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (node3->GetOutputSlot (SlotId ("out")), node4->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (node4->GetOutputSlot (SlotId ("out")), node5->GetInputSlot (SlotId ("in")));

	ValueConstPtr result = node5->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (result != nullptr);
	ASSERT (Value::IsType<IntValue> (result));
	ASSERT (IntValue::Get (result) == 5);
}

TEST (NodeValueChangeTest)
{
	NodeManager manager;

	std::shared_ptr<AdderInputOutputNode> node1 (new AdderInputOutputNode ());
	std::shared_ptr<AdderInputOutputNode> node2 (new AdderInputOutputNode ());
	manager.AddNode (node1);
	manager.AddNode (node2);

	manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node2->GetInputSlot (SlotId ("in")));

	{
		ValueConstPtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 2);
	}

	{
		node1->SetToAdd (2);
		ValueConstPtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 3);
	}
}

TEST (MultipleOutputSlotsTests)
{
	NodeManager manager;

	NodePtr node1 = manager.AddNode (NodePtr (new InputNode2 ()));
	NodePtr node2 = manager.AddNode (NodePtr (new AdderInputOutputNode (0)));
	NodePtr node3 = manager.AddNode (NodePtr (new AdderInputOutputNode (0)));

	manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out1")), node2->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out2")), node3->GetInputSlot (SlotId ("in")));

	{
		ValueConstPtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 5);
	}

	{
		ValueConstPtr result = node3->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 5);
	}
}

TEST (SlotConnectionFailedTest)
{
	NodeManager manager;

	NodePtr node1 = manager.AddNode (NodePtr (new InputNode3 ()));
	NodePtr node2 = manager.AddNode (NodePtr (new AdderInputOutputNode ()));

	ASSERT (manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out1")), node2->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out2")), node2->GetInputSlot (SlotId ("in"))));
}

TEST (SlotConnectionFailedTest2)
{
	NodeManager manager;

	NodePtr node1 = manager.AddNode (NodePtr (new InputNode3 ()));
	NodePtr node2 = manager.AddNode (NodePtr (new AdderInputOutputNode ()));

	ASSERT (manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out1")), node2->GetInputSlot (SlotId ("in"))));
	ASSERT (!manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out1")), node2->GetInputSlot (SlotId ("in"))));
}

TEST (DeleteNodeTest)
{
	NodeManager manager;

	NodePtr firstNode = manager.AddNode (NodePtr (new InputNode (5)));
	NodePtr secondNode = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));
	NodePtr secondNodeValueNode = manager.AddNode (NodePtr (new InputNode (6)));
	NodePtr additionNode = manager.AddNode (NodePtr (new AdditionNode ()));

	manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first")));
	manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("second")));
	manager.ConnectOutputSlotToInputSlot (secondNodeValueNode->GetOutputSlot (SlotId ("out")), secondNode->GetInputSlot (SlotId ("in")));

	{
		ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 12);
	}

	ASSERT (manager.DeleteNode (secondNode));
	ASSERT (secondNode->GetId () == NullNodeId);
	{
		ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 5);
	}
}

TEST (DeleteNodeTest2)
{
	NodeManager manager;

	NodePtr firstNode = manager.AddNode (NodePtr (new InputNode (5)));
	NodePtr secondNode = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));
	NodePtr secondNodeValueNode = manager.AddNode (NodePtr (new InputNode (6)));
	NodePtr additionNode = manager.AddNode (NodePtr (new AdditionNode ()));

	manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("first")));
	manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("second")));
	manager.ConnectOutputSlotToInputSlot (secondNodeValueNode->GetOutputSlot (SlotId ("out")), secondNode->GetInputSlot (SlotId ("in")));
	ASSERT (manager.GetNodeCount () == 4);

	{
		ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 12);
	}

	ASSERT (manager.DeleteNode (secondNodeValueNode));
	ASSERT (manager.GetNodeCount () == 3);
	{
		ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 6);
	}

	ASSERT (manager.DeleteNode (secondNode->GetId ()));
	ASSERT (manager.GetNodeCount () == 2);
	{
		ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 5);
	}

	ASSERT (manager.DeleteNode (firstNode));
	ASSERT (manager.GetNodeCount () == 1);
	{
		ValueConstPtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 0);
	}
}

TEST (DeleteConnectionTest)
{
	NodeManager manager;

	std::shared_ptr<AdderInputOutputNode> node1 (new AdderInputOutputNode ());
	std::shared_ptr<AdderInputOutputNode> node2 (new AdderInputOutputNode ());
	manager.AddNode (node1);
	manager.AddNode (node2);

	manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node2->GetInputSlot (SlotId ("in")));
	{
		ValueConstPtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 2);
	}

	manager.DisconnectOutputSlotFromInputSlot (node1->GetOutputSlot (SlotId ("out")), node2->GetInputSlot (SlotId ("in")));
	{
		ValueConstPtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 1);
	}
}

TEST (CycleDetectionTest1)
{
	NodeManager manager;
	NodePtr node = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));
	ASSERT (!manager.ConnectOutputSlotToInputSlot (node->GetOutputSlot (SlotId ("out")), node->GetInputSlot (SlotId ("in"))));
}

TEST (CycleDetectionTest2)
{
	NodeManager manager;

	NodePtr node1 = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));
	NodePtr node2 = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));

	ASSERT (manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node2->GetInputSlot (SlotId ("in"))));
	ASSERT (!manager.ConnectOutputSlotToInputSlot (node2->GetOutputSlot (SlotId ("out")), node1->GetInputSlot (SlotId ("in"))));
}

TEST (CycleDetectionTest3)
{
	NodeManager manager;

	NodePtr node1 = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));
	NodePtr node2 = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));
	NodePtr node3 = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));
																		   
	ASSERT (manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node2->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (node2->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("in"))));
	ASSERT (!manager.ConnectOutputSlotToInputSlot (node3->GetOutputSlot (SlotId ("out")), node1->GetInputSlot (SlotId ("in"))));
}

TEST (HasConnectionTest)
{
	NodeManager manager;

	NodePtr node1 = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));
	NodePtr node2 = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));
	NodePtr node3 = manager.AddNode (NodePtr (new AdderInputOutputNode (1)));

	ASSERT (manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node2->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (node2->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("in"))));

	ASSERT (manager.IsOutputSlotConnectedToInputSlot (node1->GetOutputSlot (SlotId ("out")), node2->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.IsOutputSlotConnectedToInputSlot (node2->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("in"))));
	ASSERT (!manager.IsOutputSlotConnectedToInputSlot (node1->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("in"))));
}

}
