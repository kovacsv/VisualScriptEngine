#include "SimpleTest.hpp"
#include "NodeManager.hpp"
#include "Node.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"

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

class InputNode : public Node
{
public:
	InputNode (int val) :
		Node (),
		val (val)
	{
		
	}

	virtual void RegisterSlots () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValuePtr (new IntValue (val));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
	}

private:
	int val;
};

class InputNode2 : public Node
{
public:
	InputNode2 () :
		Node ()
	{
	
	}

	virtual void RegisterSlots () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out1"))));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out2"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValuePtr (new IntValue (5));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
	}
};

class InputNode3 : public Node
{
public:
	InputNode3 () :
		Node ()
	{
	
	}

	virtual void RegisterSlots () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out1"))));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out2"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValuePtr (new IntValue (5));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
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

class AdditionNode : public Node
{
public:
	AdditionNode () :
		Node ()
	{

	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new TestInputSlot (SlotId ("first"))));
		RegisterInputSlot (InputSlotPtr (new TestInputSlot (SlotId ("second"))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValuePtr firstResult = EvaluateSingleInputSlot (SlotId ("first"), env);
		ValuePtr secondResult = EvaluateSingleInputSlot (SlotId ("second"), env);
		return ValuePtr (new IntValue (IntValue::Get (firstResult) + IntValue::Get (secondResult)));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
	}
};

class MultiAdditionNode : public Node
{
public:
	MultiAdditionNode () :
		Node ()
	{

	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new MultiInputSlot (SlotId ("in"), ValuePtr (new IntValue (0)))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		ListValuePtr result = EvaluateInputSlot (SlotId ("in"), env);
		int sum = 0;
		result->Enumerate ([&] (const ValuePtr& val) {
			sum += IntValue::Get (val);
		});
		return ValuePtr (new IntValue (sum));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
	}
};

class DisabledAdditionNode : public Node
{
public:
	DisabledAdditionNode () :
		Node ()
	{

	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new DisabledInputSlot (SlotId ("first"), ValuePtr (new IntValue (2)))));
		RegisterInputSlot (InputSlotPtr (new DisabledInputSlot (SlotId ("second"), ValuePtr (new IntValue (3)))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValuePtr firstResult = EvaluateSingleInputSlot (SlotId ("first"), env);
		ValuePtr secondResult = EvaluateSingleInputSlot (SlotId ("second"), env);
		return ValuePtr (new IntValue (IntValue::Get (firstResult) + IntValue::Get (secondResult)));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
	}
};

class AdderInputOutputNode : public Node
{
public:
	AdderInputOutputNode (int toAdd = 1) :
		Node (),
		toAdd (toAdd)
	{

	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new TestInputSlot (SlotId ("in"))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValuePtr inputVal = EvaluateSingleInputSlot (SlotId ("in"), env);
		return ValuePtr (new IntValue (IntValue::Get (inputVal) + toAdd));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
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
		ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 0);
	}

	ASSERT (manager.ConnectOutputSlotToInputSlot (firstNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));
	{
		ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 5);
	}

	ASSERT (manager.ConnectOutputSlotToInputSlot (secondNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (thirdNode->GetOutputSlot (SlotId ("out")), additionNode->GetInputSlot (SlotId ("in"))));
	{
		ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
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

	{
		int connectionCount = 0;
		manager.EnumerateConnections ([&] (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) {
			ASSERT (outputSlot->GetId () == SlotId ("out"));
			ASSERT (inputSlot->GetId () == SlotId ("first") || inputSlot->GetId () == SlotId ("second"));
			connectionCount++;
		});
		ASSERT (connectionCount == 2);
	}

	{
		int connectionCount = 0;
		manager.EnumerateConnections ([&] (const NodeConstPtr& outputNode, const OutputSlotConstPtr& outputSlot, const NodeConstPtr& inputNode, const InputSlotConstPtr& inputSlot) {
			ASSERT (outputNode == firstNode || outputNode == secondNode);
			ASSERT (inputNode == additionNode);
			ASSERT (outputSlot->GetId () == SlotId ("out"));
			ASSERT (inputSlot->GetId () == SlotId ("first") || inputSlot->GetId () == SlotId ("second"));
			connectionCount++;
		});
		ASSERT (connectionCount == 2);
	}

	ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
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

	ValuePtr result = additionNode2->Evaluate (NE::EmptyEvaluationEnv);
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

	ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
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

	ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
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

	ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
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

	ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
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

	ValuePtr result = node5->Evaluate (NE::EmptyEvaluationEnv);
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
		ValuePtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 2);
	}

	{
		node1->SetToAdd (2);
		ValuePtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
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
		ValuePtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 5);
	}

	{
		ValuePtr result = node3->Evaluate (NE::EmptyEvaluationEnv);
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
		ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 12);
	}

	ASSERT (manager.DeleteNode (secondNode));
	ASSERT (secondNode->GetId () == NullNodeId);
	{
		ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
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
		ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 12);
	}

	ASSERT (manager.DeleteNode (secondNodeValueNode));
	ASSERT (manager.GetNodeCount () == 3);
	{
		ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 6);
	}

	ASSERT (manager.DeleteNode (secondNode->GetId ()));
	ASSERT (manager.GetNodeCount () == 2);
	{
		ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 5);
	}

	ASSERT (manager.DeleteNode (firstNode));
	ASSERT (manager.GetNodeCount () == 1);
	{
		ValuePtr result = additionNode->Evaluate (NE::EmptyEvaluationEnv);
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
		ValuePtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (result != nullptr);
		ASSERT (Value::IsType<IntValue> (result));
		ASSERT (IntValue::Get (result) == 2);
	}

	manager.DisconnectOutputSlotFromInputSlot (node1->GetOutputSlot (SlotId ("out")), node2->GetInputSlot (SlotId ("in")));
	{
		ValuePtr result = node2->Evaluate (NE::EmptyEvaluationEnv);
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
