#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_MemoryStream.hpp"
#include "NE_SingleValues.hpp"

#include <memory>

using namespace NE;

namespace NodeManagerSerializationTest
{

class TestNode : public Node
{
	DYNAMIC_SERIALIZABLE (TestNode);

public:
	TestNode () :
		TestNode (0)
	{
	
	}

	TestNode (int val) :
		Node (),
		val (val)
	{
	
	}

	int GetVal () const
	{
		return val;
	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("a"), ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Single)));
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("b"), ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Single)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("c"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr firstResult = EvaluateInputSlot (SlotId ("a"), env);
		ValueConstPtr secondResult = EvaluateInputSlot (SlotId ("b"), env);
		return ValueConstPtr (new IntValue (val + IntValue::Get (firstResult) + IntValue::Get (secondResult)));
	}

	virtual Stream::Status Read (InputStream& inputStream) override
	{
		ObjectHeader header (inputStream);
		Node::Read (inputStream);
		inputStream.Read (val);
		return inputStream.GetStatus ();
	}

	virtual Stream::Status Write (OutputStream& outputStream) const override
	{
		ObjectHeader header (outputStream, serializationInfo);
		Node::Write (outputStream);
		outputStream.Write (val);
		return outputStream.GetStatus ();
	}

private:
	int val;
};

class TestGroup : public NodeGroup
{
	DYNAMIC_SERIALIZABLE (TestGroup);

public:
	TestGroup () :
		TestGroup (std::wstring ())
	{
	
	}

	TestGroup (const std::wstring& name) :
		NodeGroup (),
		name (name)
	{
	}

	const std::wstring& GetName () const
	{
		return name;
	}

	virtual Stream::Status Read (InputStream& inputStream) override
	{
		ObjectHeader header (inputStream);
		NodeGroup::Read (inputStream);
		inputStream.Read (name);
		return inputStream.GetStatus ();
	}

	virtual Stream::Status Write (OutputStream& outputStream) const override
	{
		ObjectHeader header (outputStream, serializationInfo);
		NodeGroup::Write (outputStream);
		outputStream.Write (name);
		return outputStream.GetStatus ();
	}

private:
	std::wstring name;
};

DYNAMIC_SERIALIZATION_INFO (TestNode, 1, "{9E0304A4-3B92-4EFA-9846-F0372A633038}");
DYNAMIC_SERIALIZATION_INFO (TestGroup, 1, "{66E68205-83BF-423F-B2B2-41C356B68125}");

TEST (EmptyNodeManagerSerializationTest)
{
	NodeManager source;
	NodeManager target;
	ASSERT (NodeManager::Clone (source, target));
	ASSERT (source.GetNodeCount () == target.GetNodeCount ());
}

TEST (OneNodeSerializazionTest)
{
	NodeManager source;
	std::shared_ptr<TestNode> sourceNode (new TestNode (42));
	source.AddNode (sourceNode);

	NodeManager target;
	ASSERT (NodeManager::Clone (source, target));
	ASSERT (source.GetNodeCount () == target.GetNodeCount ());
	ASSERT (target.ContainsNode (sourceNode->GetId ()));
	std::shared_ptr<TestNode> targetNode = std::dynamic_pointer_cast<TestNode> (target.GetNode (sourceNode->GetId ()));
	ASSERT (targetNode != nullptr);
	ASSERT (sourceNode != targetNode);
	ASSERT (sourceNode->GetVal () == targetNode->GetVal ());
}

TEST (ConnectionSerializationTest)
{
	NodeManager source;
	std::shared_ptr<TestNode> sourceNode1 (new TestNode (1));
	std::shared_ptr<TestNode> sourceNode2 (new TestNode (2));
	std::shared_ptr<TestNode> sourceNode3 (new TestNode (3));
	source.AddNode (sourceNode1);
	source.AddNode (sourceNode2);
	source.AddNode (sourceNode3);
	source.ConnectOutputSlotToInputSlot (sourceNode1->GetOutputSlot (SlotId ("c")), sourceNode3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (sourceNode2->GetOutputSlot (SlotId ("c")), sourceNode3->GetInputSlot (SlotId ("b")));
	ValueConstPtr sourceValue = sourceNode3->Evaluate (EmptyEvaluationEnv);
	ASSERT (IntValue::Get (sourceValue) == 6);

	NodeManager target;
	ASSERT (NodeManager::Clone (source, target));
	ASSERT (source.GetNodeCount () == target.GetNodeCount ());
	std::shared_ptr<TestNode> targetNode3 = std::dynamic_pointer_cast<TestNode> (target.GetNode (sourceNode3->GetId ()));
	ValueConstPtr targetValue = targetNode3->Evaluate (EmptyEvaluationEnv);
	ASSERT (IntValue::Get (targetValue) == 6);
}

TEST (NodeGroupSerializationTest)
{
	NodeManager source;
	std::shared_ptr<TestNode> sourceNode1 (new TestNode (1));
	std::shared_ptr<TestNode> sourceNode2 (new TestNode (2));
	std::shared_ptr<TestNode> sourceNode3 (new TestNode (3));
	source.AddNode (sourceNode1);
	source.AddNode (sourceNode2);
	source.AddNode (sourceNode3);
	source.ConnectOutputSlotToInputSlot (sourceNode1->GetOutputSlot (SlotId ("c")), sourceNode3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (sourceNode2->GetOutputSlot (SlotId ("c")), sourceNode3->GetInputSlot (SlotId ("b")));
	NodeGroupPtr testGroup (new TestGroup (L"Test"));
	source.AddNodeGroup (testGroup);
	source.AddNodeToGroup (testGroup->GetId (), sourceNode1->GetId ());
	source.AddNodeToGroup (testGroup->GetId (), sourceNode2->GetId ());

	NodeManager target;
	ASSERT (NodeManager::Clone (source, target));
	ASSERT (std::static_pointer_cast<const TestGroup> (target.GetNodeGroup (sourceNode1->GetId ()))->GetName () == L"Test");
	ASSERT (std::static_pointer_cast<const TestGroup> (target.GetNodeGroup (sourceNode2->GetId ()))->GetName () == L"Test");
}

}
