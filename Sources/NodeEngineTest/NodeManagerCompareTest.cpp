#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "NE_NodeManagerCompare.hpp"

using namespace NE;

namespace NodeCompareManagerTest
{

class TestNode : public Node
{
	DYNAMIC_SERIALIZABLE (TestNode);

public:
	TestNode () :
		TestNode (L"")
	{

	}

	TestNode (const std::wstring& name) :
		Node (),
		name (name)
	{

	}

	void SetName (const std::wstring& newName)
	{
		name = newName;
	}

	const std::wstring& GetName () const
	{
		return name;
	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("a"), ValuePtr (new IntValue (1)), OutputSlotConnectionMode::Single)));
		RegisterInputSlot (InputSlotPtr (new InputSlot (SlotId ("b"), ValuePtr (new IntValue (1)), OutputSlotConnectionMode::Single)));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValuePtr a = EvaluateSingleInputSlot (SlotId ("a"), env);
		ValuePtr b = EvaluateSingleInputSlot (SlotId ("b"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual Stream::Status Read (InputStream& inputStream) override
	{
		ObjectHeader header (inputStream);
		Node::Read (inputStream);
		inputStream.Read (name);
		return inputStream.GetStatus ();
	}

	virtual Stream::Status Write (OutputStream& outputStream) const override
	{
		ObjectHeader header (outputStream, serializationInfo);
		Node::Write (outputStream);
		outputStream.Write (name);
		return outputStream.GetStatus ();
	}

private:
	std::wstring name;
};

DynamicSerializationInfo TestNode::serializationInfo (ObjectId ("{6C8DABF5-739C-450F-8607-0CD9EA9B2E7F}"), ObjectVersion (1), TestNode::CreateSerializableInstance);

static size_t NodeCountInDelta (const NodeManagerDelta& delta)
{
	return delta.GetNewNodes ().size () + delta.GetDeletedNodes ().size () + delta.GetModifiedNodes ().size ();
}

TEST (NodeManagerCompareTest_Empty)
{
	NodeManager nodeManager;
	NodeManagerDelta delta = CompareNodeManagers (nodeManager, nodeManager);
	ASSERT (delta.IsEmpty ());
	ASSERT (NodeCountInDelta (delta) == 0);
}

TEST (NodeManagerCompareTest_Equal)
{
	NodeManager source;
	NodePtr node1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr node2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr node3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr node4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (node2->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (node3->GetOutputSlot (SlotId ("out")), node4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	target.AppendFrom (source, AllNodesFilter ());

	NodeManagerDelta delta = CompareNodeManagers (source, target);
	ASSERT (delta.IsEmpty ());
	ASSERT (NodeCountInDelta (delta) == 0);
}

TEST (NodeManagerCompareTest_DeletedNode)
{
	NodeManager source;
	NodePtr node1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr node2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr node3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr node4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (node2->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (node3->GetOutputSlot (SlotId ("out")), node4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	target.AppendFrom (source, AllNodesFilter ());
	target.DeleteNode (node3->GetId ());

	NodeManagerDelta delta = CompareNodeManagers (source, target);
	ASSERT (!delta.IsEmpty ());
	ASSERT (delta.GetDeletedNodes ().size () == 1);
	ASSERT (delta.GetDeletedNodes ()[0] == node3->GetId ());
	ASSERT (NodeCountInDelta (delta) == 1);
}

TEST (NodeManagerCompareTest_NewNode)
{
	NodeManager source;
	NodePtr node1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr node2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr node3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr node4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (node2->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (node3->GetOutputSlot (SlotId ("out")), node4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	target.AppendFrom (source, AllNodesFilter ());
	NodePtr node5 = target.AddNode (NodePtr (new TestNode (L"5")));

	NodeManagerDelta delta = CompareNodeManagers (source, target);
	ASSERT (!delta.IsEmpty ());
	ASSERT (delta.GetNewNodes ().size () == 1);
	ASSERT (delta.GetNewNodes ()[0] == node5->GetId ());
	ASSERT (NodeCountInDelta (delta) == 1);
}

TEST (NodeManagerCompareTest_ModifiedNode)
{
	NodeManager source;
	NodePtr node1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr node2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr node3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr node4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (node2->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (node3->GetOutputSlot (SlotId ("out")), node4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	target.AppendFrom (source, AllNodesFilter ());
	std::shared_ptr<TestNode> targetNode = Node::Cast<TestNode> (target.GetNode (node3->GetId ()));
	targetNode->SetName (L"NewName");

	NodeManagerDelta delta = CompareNodeManagers (source, target);
	ASSERT (!delta.IsEmpty ());
	ASSERT (delta.GetModifiedNodes ().size () == 1);
	ASSERT (delta.GetModifiedNodes ()[0] == node3->GetId ());
	ASSERT (NodeCountInDelta (delta) == 1);
}

}
