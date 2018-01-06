#include "SimpleTest.hpp"
#include "NodeManager.hpp"
#include "Node.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"

using namespace NE;

namespace NodeValueCacheTest
{

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

class InputOutputNode : public Node
{
public:
	InputOutputNode () :
		Node (),
		calculationCount (0),
		calculationPostProcessCount (0)
	{

	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new TestInputSlot (SlotId ("in"))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		calculationCount++;
		ValuePtr in = EvaluateSingleInputSlot (SlotId ("in"), env);
		return ValuePtr (new IntValue (IntValue::Get (in) + 1));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv& env) const override
	{
		Evaluate (env); // should not call calculation again
		calculationPostProcessCount++;
	}

	bool CheckPreAndPostProcessCount () const
	{
		return calculationCount == calculationPostProcessCount;
	}

	mutable int calculationCount;
	mutable int calculationPostProcessCount;
};

class DummyEvaluationData : public NE::EvaluationData
{
public:
	int x = 5;
};

class EnvModifierNode : public Node
{
public:
	EnvModifierNode () :
		Node ()
	{

	}

	virtual void RegisterSlots () override
	{

	}

	virtual ValuePtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValuePtr (new IntValue (5));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv& env) const override
	{
		std::shared_ptr<DummyEvaluationData> dummyData = env.GetData<DummyEvaluationData> ();
		dummyData->x += 1;
	}
};

TEST (SimpleCacheTest)
{
	NodeManager manager;

	std::shared_ptr<InputOutputNode> node (new InputOutputNode ());
	manager.AddNode (node);

	{
		ValuePtr result = node->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 1);
		ASSERT (node->calculationCount == 1);
		ASSERT (node->CheckPreAndPostProcessCount ());
	}
	{
		ValuePtr result = node->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 1);
		ASSERT (node->calculationCount == 1);
		ASSERT (node->CheckPreAndPostProcessCount ());
	}
}

TEST (ConnectedNodesCacheTest)
{
	// 0 -> 1 -> 2 -> 3 -> 4

	NodeManager manager;

	std::vector<std::shared_ptr<InputOutputNode>> nodes;
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));

	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		manager.AddNode (node);
	}

	for (size_t i = 0; i < nodes.size () - 1; ++i) {
		manager.ConnectOutputSlotToInputSlot (nodes[i]->GetOutputSlot (SlotId ("out")), nodes[i + 1]->GetInputSlot (SlotId ("in")));
	}

	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		ASSERT (node->calculationCount == 0);
		ASSERT (node->CheckPreAndPostProcessCount ());
	}

	ASSERT (IntValue::Get (nodes.back ()->Evaluate (NE::EmptyEvaluationEnv)) == 5);
	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		ASSERT (node->calculationCount == 1);
		ASSERT (node->CheckPreAndPostProcessCount ());
	}

	nodes[2]->InvalidateValue ();
	ASSERT (IntValue::Get (nodes.back ()->Evaluate (NE::EmptyEvaluationEnv)) == 5);
	for (size_t i = 0; i < nodes.size () - 1; ++i) {
		if (i < 2) {
			ASSERT (nodes[i]->calculationCount == 1);
		} else {
			ASSERT (nodes[i]->calculationCount == 2);
		}
		ASSERT (nodes[i]->CheckPreAndPostProcessCount ());
	}
}

TEST (MultipleConnectionsTest)
{
	// 0 -> 1 -> 2
	//		|
	//      -> 3 -> 4

	NodeManager manager;

	std::vector<std::shared_ptr<InputOutputNode>> nodes;
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));

	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		manager.AddNode (node);
	}

	manager.ConnectOutputSlotToInputSlot (nodes[0]->GetOutputSlot (SlotId ("out")), nodes[1]->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (nodes[1]->GetOutputSlot (SlotId ("out")), nodes[2]->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (nodes[1]->GetOutputSlot (SlotId ("out")), nodes[3]->GetInputSlot (SlotId ("in")));
	manager.ConnectOutputSlotToInputSlot (nodes[3]->GetOutputSlot (SlotId ("out")), nodes[4]->GetInputSlot (SlotId ("in")));

	nodes[2]->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (nodes[0]->calculationCount == 1);
	ASSERT (nodes[1]->calculationCount == 1);
	ASSERT (nodes[2]->calculationCount == 1);
	ASSERT (nodes[3]->calculationCount == 0);
	ASSERT (nodes[4]->calculationCount == 0);
	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		ASSERT (node->CheckPreAndPostProcessCount ());
	}

	nodes[4]->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (nodes[0]->calculationCount == 1);
	ASSERT (nodes[1]->calculationCount == 1);
	ASSERT (nodes[2]->calculationCount == 1);
	ASSERT (nodes[3]->calculationCount == 1);
	ASSERT (nodes[4]->calculationCount == 1);
	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		ASSERT (node->CheckPreAndPostProcessCount ());
	}

	nodes[1]->InvalidateValue ();
	nodes[2]->Evaluate (NE::EmptyEvaluationEnv);
	nodes[4]->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (nodes[0]->calculationCount == 1);
	ASSERT (nodes[1]->calculationCount == 2);
	ASSERT (nodes[2]->calculationCount == 2);
	ASSERT (nodes[3]->calculationCount == 2);
	ASSERT (nodes[4]->calculationCount == 2);
	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		ASSERT (node->CheckPreAndPostProcessCount ());
	}

	nodes[3]->InvalidateValue ();
	nodes[2]->Evaluate (NE::EmptyEvaluationEnv);
	nodes[4]->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (nodes[0]->calculationCount == 1);
	ASSERT (nodes[1]->calculationCount == 2);
	ASSERT (nodes[2]->calculationCount == 2);
	ASSERT (nodes[3]->calculationCount == 3);
	ASSERT (nodes[4]->calculationCount == 3);
	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		ASSERT (node->CheckPreAndPostProcessCount ());
	}
}

TEST (DeleteInvalidationTest)
{
	// 0 -> 1 -> 2 -> 3 -> 4

	NodeManager manager;

	std::vector<std::shared_ptr<InputOutputNode>> nodes;
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));

	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		manager.AddNode (node);
	}

	for (size_t i = 0; i < nodes.size () - 1; ++i) {
		manager.ConnectOutputSlotToInputSlot (nodes[i]->GetOutputSlot (SlotId ("out")), nodes[i + 1]->GetInputSlot (SlotId ("in")));
	}

	{
		ValuePtr val = nodes.back ()->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (val) == 5);
	}

	manager.DeleteNode (nodes[2]);
	{
		ValuePtr val = nodes.back ()->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (val) == 2);
	}

	manager.ConnectOutputSlotToInputSlot (nodes[1]->GetOutputSlot (SlotId ("out")), nodes[3]->GetInputSlot (SlotId ("in")));
	{
		ValuePtr val = nodes.back ()->Evaluate (NE::EmptyEvaluationEnv);
		ASSERT (IntValue::Get (val) == 4);
	}
}

TEST (EnumerateDependentNodesTest)
{
	// 0 -> 1 -> 2 -> 3 -> 4

	NodeManager manager;

	std::vector<std::shared_ptr<InputOutputNode>> nodes;
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));
	nodes.push_back (std::shared_ptr<InputOutputNode> (new InputOutputNode ()));

	for (const std::shared_ptr<InputOutputNode>& node : nodes) {
		manager.AddNode (node);
	}
	
	for (size_t i = 0; i < nodes.size () - 1; ++i) {
		manager.ConnectOutputSlotToInputSlot (nodes[i]->GetOutputSlot (SlotId ("out")), nodes[i + 1]->GetInputSlot (SlotId ("in")));
	}

	{
		int count = 0;
		manager.EnumerateDependentNodes (nodes[0], [&] (const NodeConstPtr&) {
			count++;
		});
		ASSERT (count == 1);
	}

	{
		int count = 0;
		manager.EnumerateDependentNodesRecursive (nodes[0], [&] (const NodeConstPtr&) {
			count++;
		});
		ASSERT (count == 4);
	}
}

TEST (EvaluationEnvTest)
{
	NodeManager manager;


	std::shared_ptr<DummyEvaluationData> evalData (new DummyEvaluationData ());
	ASSERT (evalData->x == 5);

	std::shared_ptr<EnvModifierNode> node (new EnvModifierNode ());
	manager.AddNode (node);

	{
		NE::EvaluationEnv env (evalData);
		ValuePtr result = node->Evaluate (env);
		ASSERT (IntValue::Get (result) == 5);
		ASSERT (evalData->x == 6);
	}

	{
		ValuePtr result = node->GetCalculatedValue ();
		ASSERT (IntValue::Get (result) == 5);
		ASSERT (evalData->x == 6);
	}
}

}
