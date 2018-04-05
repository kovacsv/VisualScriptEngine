#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestNodes.hpp"

using namespace NE;

namespace NodeRecalculationTest
{

class TestNode : public SerializableTestNode
{
public:
	TestNode () :
		SerializableTestNode ()
	{
	
	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("in"), ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Single)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		calculationCounter++;
		ValuePtr in = EvaluateSingleInputSlot (SlotId ("in"), env);
		return ValuePtr (new IntValue (IntValue::Get (in) + 1));
	}

	mutable int calculationCounter = 0;
};

TEST (RecalculationTest)
{
	NodeManager manager;

	std::vector<std::shared_ptr<TestNode>> nodes = {
		std::shared_ptr<TestNode> (new TestNode ()),
		std::shared_ptr<TestNode> (new TestNode ()),
		std::shared_ptr<TestNode> (new TestNode ()),
		std::shared_ptr<TestNode> (new TestNode ()),
		std::shared_ptr<TestNode> (new TestNode ())
	};

	for (std::shared_ptr<TestNode> node : nodes) {
		manager.AddNode (node);
	}
	for (size_t i = 0; i < nodes.size () - 1; ++i) {
		manager.ConnectOutputSlotToInputSlot (nodes[i]->GetOutputSlot (SlotId ("out")), nodes[i + 1]->GetInputSlot (SlotId ("in")));
	}

	{
		for (std::shared_ptr<TestNode> node : nodes) {
			ASSERT (node->calculationCounter == 0);
		}
		manager.EvaluateAllNodes (EmptyEvaluationEnv);
		for (std::shared_ptr<TestNode> node : nodes) {
			ASSERT (node->calculationCounter == 1);
			ASSERT (node->ValueIsCalculated ());
		}
		ValuePtr lastVal = nodes[4]->Evaluate (EmptyEvaluationEnv);
		ASSERT (Value::IsType<IntValue> (lastVal));
		ASSERT (IntValue::Get (lastVal) == 5);
		for (std::shared_ptr<TestNode> node : nodes) {
			ASSERT (node->calculationCounter == 1);
			ASSERT (node->ValueIsCalculated ());
		}
	}

	{
		nodes[2]->InvalidateValue ();
		for (size_t i = 0; i < nodes.size (); ++i) {
			ASSERT (nodes[i]->calculationCounter == 1);
			if (i < 2) {
				ASSERT (nodes[i]->ValueIsCalculated ());
			} else {
				ASSERT (!nodes[i]->ValueIsCalculated ());
			}
		}
		manager.EvaluateAllNodes (EmptyEvaluationEnv);
		for (size_t i = 0; i < nodes.size (); ++i) {
			if (i < 2) {
				ASSERT (nodes[i]->calculationCounter == 1);
			} else {
				ASSERT (nodes[i]->calculationCounter == 2);
			}
			ASSERT (nodes[i]->ValueIsCalculated ());
		}
	}

	{
		manager.DisconnectOutputSlotFromInputSlot (nodes[1]->GetOutputSlot (SlotId ("out")), nodes[2]->GetInputSlot (SlotId ("in")));
		for (size_t i = 0; i < nodes.size (); ++i) {
			if (i < 2) {
				ASSERT (nodes[i]->calculationCounter == 1);
			} else {
				ASSERT (nodes[i]->calculationCounter == 2);
			}
			if (i < 2) {
				ASSERT (nodes[i]->ValueIsCalculated ());
			} else {
				ASSERT (!nodes[i]->ValueIsCalculated ());
			}
		}
		manager.EvaluateAllNodes (EmptyEvaluationEnv);
		for (size_t i = 0; i < nodes.size (); ++i) {
			if (i < 2) {
				ASSERT (nodes[i]->calculationCounter == 1);
			} else {
				ASSERT (nodes[i]->calculationCounter == 3);
			}
			ASSERT (nodes[i]->ValueIsCalculated ());
		}
	}
}

}
