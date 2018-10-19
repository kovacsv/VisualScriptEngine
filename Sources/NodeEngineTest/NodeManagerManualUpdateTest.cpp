#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestNodes.hpp"

using namespace NE;

namespace NodeManagerManualUpdateTest
{

class ValueNode : public SerializableTestNode
{
public:
	ValueNode (int val) :
		SerializableTestNode (),
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

	void SetValue (int newVal)
	{
		val = newVal;
		InvalidateValue ();
	}

private:
	virtual bool IsForceCalculated () const override
	{
		return true;
	}

	int val;
};

class IncreaseNode : public SerializableTestNode
{
public:
	IncreaseNode () :
		SerializableTestNode ()
	{

	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("in"), nullptr, NE::OutputSlotConnectionMode::Single)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValuePtr in = EvaluateSingleInputSlot (SlotId ("in"), env);
		return ValuePtr (new IntValue (IntValue::Get (in) + 1));
	}
};

TEST (ManualUpdateTest_Automatic)
{
	NodeManager manager;

	NE::NodePtr valueNode = manager.AddNode (NodePtr (new ValueNode (5)));
	NE::NodePtr incNode1 = manager.AddNode (NodePtr (new IncreaseNode ()));
	NE::NodePtr incNode2 = manager.AddNode (NodePtr (new IncreaseNode ()));

	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode1->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode2->GetInputSlot (SlotId ("in")));

	{
		NE::ValuePtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 5);
		NE::ValuePtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode1Val) == 6);
		NE::ValuePtr incNode2Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode2Val) == 6);
	}

	NE::Node::Cast<ValueNode> (valueNode)->SetValue (10);
	ASSERT (!incNode1->HasCalculatedValue ());
	ASSERT (!incNode2->HasCalculatedValue ());

	{
		NE::ValuePtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 10);
		NE::ValuePtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode1Val) == 11);
		NE::ValuePtr incNode2Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode2Val) == 11);
	}
}

TEST (ManualUpdateTest_Manual)
{
	NodeManager manager;
	manager.SetUpdateMode (NodeManager::UpdateMode::Manual);

	NE::NodePtr valueNode = manager.AddNode (NodePtr (new ValueNode (5)));
	NE::NodePtr incNode1 = manager.AddNode (NodePtr (new IncreaseNode ()));
	NE::NodePtr incNode2 = manager.AddNode (NodePtr (new IncreaseNode ()));

	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode1->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode2->GetInputSlot (SlotId ("in")));

	{
		NE::ValuePtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 5);
		NE::ValuePtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode1Val == nullptr);
		NE::ValuePtr incNode2Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode2Val == nullptr);
	}

	{
		NE::ValuePtr incNode1Val = incNode1->ForceEvaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode1Val) == 6);
		NE::ValuePtr incNode2Val = incNode1->ForceEvaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode2Val) == 6);
	}	
}

TEST (ManualUpdateTest_ManualForceAll)
{
	NodeManager manager;
	manager.SetUpdateMode (NodeManager::UpdateMode::Manual);

	NE::NodePtr valueNode = manager.AddNode (NodePtr (new ValueNode (5)));
	NE::NodePtr incNode1 = manager.AddNode (NodePtr (new IncreaseNode ()));
	NE::NodePtr incNode2 = manager.AddNode (NodePtr (new IncreaseNode ()));

	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode1->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode2->GetInputSlot (SlotId ("in")));

	{
		NE::ValuePtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 5);
		NE::ValuePtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode1Val == nullptr);
		NE::ValuePtr incNode2Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode2Val == nullptr);
	}

	manager.ForceEvaluateAllNodes (EmptyEvaluationEnv);

	{
		NE::ValuePtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode1Val) == 6);
		NE::ValuePtr incNode2Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode2Val) == 6);
	}	
}

}
