#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_EvaluationEnv.hpp"
#include "TestNodes.hpp"

using namespace NE;

namespace NodeTest
{

class TestInputSlot : public InputSlot
{
public:
	TestInputSlot (const SlotId& id) :
		InputSlot (id, nullptr, OutputSlotConnectionMode::Single)
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

class TestNode : public SerializableTestNode
{
public:
	TestNode () :
		SerializableTestNode ()
	{

	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new TestInputSlot (SlotId ("a"))));
		RegisterInputSlot (InputSlotPtr (new TestInputSlot (SlotId ("b"))));
		RegisterOutputSlot (OutputSlotPtr (new TestOutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
	{
		return nullptr;
	}
};

class DummyEvaluationData : public EvaluationData
{
public:
	DummyEvaluationData (int x) :
		x (x)
	{

	}

	int x;
};

TEST (EvaluationEnvTest)
{
	EvaluationEnv env (std::shared_ptr<EvaluationData> (new DummyEvaluationData (5)));
	ASSERT (!env.IsDataType<TestNode> ());
	ASSERT (env.IsDataType<EvaluationData> ());
	ASSERT (env.IsDataType<DummyEvaluationData> ());
	std::shared_ptr<DummyEvaluationData> data = env.GetData<DummyEvaluationData> ();
	ASSERT (data->x == 5);
}

TEST (NodeTest)
{
	NodeManager manager;

	NodePtr node (new TestNode ());
	manager.AddNode (node);

	ASSERT (node->HasInputSlot (SlotId ("a")));
	ASSERT (node->HasInputSlot (SlotId ("b")));
	ASSERT (!node->HasInputSlot (SlotId ("out")));

	ASSERT (!node->HasOutputSlot (SlotId ("a")));
	ASSERT (!node->HasOutputSlot (SlotId ("b")));
	ASSERT (node->HasOutputSlot (SlotId ("out")));

	ASSERT (node->GetInputSlot (SlotId ("a"))->GetOwnerNodeId () == node->GetId ());
	ASSERT (node->GetInputSlot (SlotId ("b"))->GetOwnerNodeId () == node->GetId ());
	ASSERT (node->GetOutputSlot (SlotId ("out"))->GetOwnerNodeId () == node->GetId ());
}

TEST (NodeSlotEnumerationTest)
{
	NodeManager manager;

	NodePtr node (new TestNode ());
	manager.AddNode (node);

	ASSERT (node->HasInputSlot (SlotId ("a")));
	ASSERT (node->HasInputSlot (SlotId ("b")));
	ASSERT (node->HasOutputSlot (SlotId ("out")));

	{
		int count = 0;
		node->EnumerateInputSlots ([&] (const InputSlotPtr&) -> bool {
			count++;
			return true;
		});
		ASSERT (count == 2);
	}

	{
		int count = 0;
		node->EnumerateOutputSlots ([&] (const OutputSlotPtr&) -> bool {
			count++;
			return true;
		});
		ASSERT (count == 1);
	}

	{
		int count = 0;
		node->EnumerateSlots<InputSlotPtr> ([&] (const InputSlotPtr&) -> bool {
			count++;
			return true;
		});
		ASSERT (count == 2);
	}

	{
		int count = 0;
		node->EnumerateSlots<OutputSlotPtr> ([&] (const OutputSlotPtr&) -> bool {
			count++;
			return true;
		});
		ASSERT (count == 1);
	}
}

}
