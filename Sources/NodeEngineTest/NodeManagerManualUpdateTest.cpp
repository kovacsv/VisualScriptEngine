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

	virtual void Initialize () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
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

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("in"), nullptr, NE::OutputSlotConnectionMode::Single)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr in = EvaluateSingleInputSlot (SlotId ("in"), env);
		return ValuePtr (new IntValue (IntValue::Get (in) + 1));
	}
};


class IncreaseForceCalculatedNode : public SerializableTestNode
{
public:
	IncreaseForceCalculatedNode () :
		SerializableTestNode ()
	{

	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("in"), nullptr, NE::OutputSlotConnectionMode::Single)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr in = EvaluateSingleInputSlot (SlotId ("in"), env);
		if (in == nullptr) {
			return nullptr;
		}
		return ValuePtr (new IntValue (IntValue::Get (in) + 1));
	}

private:
	virtual bool IsForceCalculated () const override
	{
		return true;
	}
};

TEST (ManualUpdateTest_Automatic)
{
	NodeManager manager;

	NE::NodePtr valueNode = manager.AddNode (NodePtr (new ValueNode (5)));
	NE::NodePtr incNode1 = manager.AddNode (NodePtr (new IncreaseNode ()));
	NE::NodePtr incNode2 = manager.AddNode (NodePtr (new IncreaseNode ()));
	NE::NodePtr incNode3 = manager.AddNode (NodePtr (new IncreaseForceCalculatedNode ()));

	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode1->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode2->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (incNode2->GetOutputSlot (SlotId ("out")), incNode3->GetInputSlot (SlotId ("in")));

	{
		NE::ValueConstPtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 5);
		NE::ValueConstPtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode1Val) == 6);
		NE::ValueConstPtr incNode2Val = incNode2->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode2Val) == 6);
		NE::ValueConstPtr incNode3Val = incNode3->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode3Val) == 7);
	}

	NE::Node::Cast<ValueNode> (valueNode)->SetValue (10);
	ASSERT (!incNode1->HasCalculatedValue ());
	ASSERT (!incNode2->HasCalculatedValue ());

	{
		NE::ValueConstPtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 10);
		NE::ValueConstPtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode1Val) == 11);
		NE::ValueConstPtr incNode2Val = incNode2->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode2Val) == 11);
		NE::ValueConstPtr incNode3Val = incNode3->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode3Val) == 12);
	}
}

TEST (ManualUpdateTest_Manual)
{
	NodeManager manager;
	manager.SetUpdateMode (NodeManager::UpdateMode::Manual);

	NE::NodePtr valueNode = manager.AddNode (NodePtr (new ValueNode (5)));
	NE::NodePtr incNode1 = manager.AddNode (NodePtr (new IncreaseNode ()));
	NE::NodePtr incNode2 = manager.AddNode (NodePtr (new IncreaseNode ()));
	NE::NodePtr incNode3 = manager.AddNode (NodePtr (new IncreaseForceCalculatedNode ()));

	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode1->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode2->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (incNode2->GetOutputSlot (SlotId ("out")), incNode3->GetInputSlot (SlotId ("in")));

	{
		NE::ValueConstPtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 5);
		NE::ValueConstPtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode1Val == nullptr);
		NE::ValueConstPtr incNode2Val = incNode2->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode2Val == nullptr);
		NE::ValueConstPtr incNode3Val = incNode3->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode3Val == nullptr);
	}

	manager.EvaluateAllNodes (EmptyEvaluationEnv);

	{
		NE::ValueConstPtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 5);
		NE::ValueConstPtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode1Val == nullptr);
		NE::ValueConstPtr incNode2Val = incNode2->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode2Val == nullptr);
		NE::ValueConstPtr incNode3Val = incNode3->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode3Val == nullptr);
	}
}

TEST (ManualUpdateTest_ManualForced)
{
	NodeManager manager;
	manager.SetUpdateMode (NodeManager::UpdateMode::Manual);

	NE::NodePtr valueNode = manager.AddNode (NodePtr (new ValueNode (5)));
	NE::NodePtr incNode1 = manager.AddNode (NodePtr (new IncreaseNode ()));
	NE::NodePtr incNode2 = manager.AddNode (NodePtr (new IncreaseNode ()));
	NE::NodePtr incNode3 = manager.AddNode (NodePtr (new IncreaseForceCalculatedNode ()));

	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode1->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode2->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (incNode2->GetOutputSlot (SlotId ("out")), incNode3->GetInputSlot (SlotId ("in")));

	{
		NE::ValueConstPtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 5);
		NE::ValueConstPtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode1Val == nullptr);
		NE::ValueConstPtr incNode2Val = incNode2->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode2Val == nullptr);
		NE::ValueConstPtr incNode3Val = incNode3->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode3Val == nullptr);
	}

	manager.ForceEvaluateAllNodes (EmptyEvaluationEnv);

	{
		NE::ValueConstPtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode1Val) == 6);
		NE::ValueConstPtr incNode2Val = incNode2->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode2Val) == 6);
		NE::ValueConstPtr incNode3Val = incNode3->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode3Val) == 7);
	}

	NE::Node::Cast<ValueNode> (valueNode)->SetValue (10);

	{
		NE::ValueConstPtr valueNodeVal = valueNode->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (valueNodeVal) == 10);
		NE::ValueConstPtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode1Val == nullptr);
		NE::ValueConstPtr incNode2Val = incNode2->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode2Val == nullptr);
		NE::ValueConstPtr incNode3Val = incNode3->Evaluate (EmptyEvaluationEnv);
		ASSERT (incNode3Val == nullptr);
	}

	manager.ForceEvaluateAllNodes (EmptyEvaluationEnv);

	{
		NE::ValueConstPtr incNode1Val = incNode1->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode1Val) == 11);
		NE::ValueConstPtr incNode2Val = incNode2->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode2Val) == 11);
		NE::ValueConstPtr incNode3Val = incNode3->Evaluate (EmptyEvaluationEnv);
		ASSERT (NE::IntValue::Get (incNode3Val) == 12);
	}
}

TEST (ManualUpdateTest_RandomForced)
{
	for (size_t i = 0; i < 100; i++) {
		NodeManager manager;
		manager.SetUpdateMode (NodeManager::UpdateMode::Manual);

		NE::NodePtr valueNode = nullptr;
		NE::NodePtr incNode1 = nullptr;
		NE::NodePtr incNode2 = nullptr;
		NE::NodePtr incNode3 = nullptr;

		std::vector<std::function <void ()>> addOps = {
			[&] () { valueNode = manager.AddNode (NodePtr (new ValueNode (5))); },
			[&] () { incNode1 = manager.AddNode (NodePtr (new IncreaseNode ())); },
			[&] () { incNode2 = manager.AddNode (NodePtr (new IncreaseNode ())); },
			[&] () { incNode3 = manager.AddNode (NodePtr (new IncreaseForceCalculatedNode ())); }
		};

		std::vector<std::function <void ()>> connectOps = {
			[&] () { manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode1->GetInputSlot (SlotId ("in"))); },
			[&] () { manager.ConnectOutputSlotToInputSlot (valueNode->GetOutputSlot (SlotId ("out")), incNode2->GetInputSlot (SlotId ("in"))); },
			[&] () { manager.ConnectOutputSlotToInputSlot (incNode2->GetOutputSlot (SlotId ("out")), incNode3->GetInputSlot (SlotId ("in"))); }
		};

		std::random_shuffle (addOps.begin (), addOps.end ());
		std::random_shuffle (connectOps.begin (), connectOps.end ());

		for (auto& it : addOps) {
			it ();
		}

		for (auto& it : connectOps) {
			it ();
		}

		manager.ForceEvaluateAllNodes (EmptyEvaluationEnv);

		NE::ValueConstPtr incNode1Val = incNode1->GetCalculatedValue ();
		ASSERT (NE::IntValue::Get (incNode1Val) == 6);
		NE::ValueConstPtr incNode2Val = incNode2->GetCalculatedValue ();
		ASSERT (NE::IntValue::Get (incNode2Val) == 6);
		NE::ValueConstPtr incNode3Val = incNode3->GetCalculatedValue ();
		ASSERT (NE::IntValue::Get (incNode3Val) == 7);
	}
}

}
