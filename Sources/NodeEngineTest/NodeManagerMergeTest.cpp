#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_NodeManagerMerge.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "NE_MemoryStream.hpp"

using namespace NE;

namespace MergeTest
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

	const std::wstring& GetName () const
	{
		return name;
	}

	void SetName (const std::wstring& newName)
	{
		name = newName;
	}

	virtual void Initialize () override
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

DynamicSerializationInfo TestNode::serializationInfo (ObjectId ("{16CF9B46-D77D-4F5E-96EB-494617522929}"), ObjectVersion (1), TestNode::CreateSerializableInstance);

std::vector<NodeConstPtr> FindNodesByName (const NodeManager& manager, const std::wstring& name)
{
	std::vector<NodeConstPtr> result;
	manager.EnumerateNodes ([&] (const NodeConstPtr& node) {
		std::shared_ptr<const TestNode> testNode = std::dynamic_pointer_cast<const TestNode> (node);
		if (DBGERROR (testNode == nullptr)) {
			return true;
		}
		if (testNode->GetName () == name) {
			result.push_back (testNode);
		}
		return true;
	});
	return result;
}

static void InitNodeManager (NodeManager& manager)
{
	NodePtr node1 = manager.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr node2 = manager.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr node3 = manager.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr node4 = manager.AddNode (NodePtr (new TestNode (L"4")));
	manager.ConnectOutputSlotToInputSlot (node1->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("a")));
	manager.ConnectOutputSlotToInputSlot (node2->GetOutputSlot (SlotId ("out")), node3->GetInputSlot (SlotId ("b")));
	manager.ConnectOutputSlotToInputSlot (node3->GetOutputSlot (SlotId ("out")), node4->GetInputSlot (SlotId ("a")));
}

static bool IsEqualNodeManagers (const NodeManager& source, const NodeManager& target)
{
	if (source.GetNodeCount () != target.GetNodeCount ()) {
		return false;
	}
	if (source.GetConnectionCount () != target.GetConnectionCount ()) {
		return false;
	}
	bool isEqual = true;
	source.EnumerateNodes ([&] (const NodeConstPtr& sourceNode) {
		NodeConstPtr targetNode = target.GetNode (sourceNode->GetId ());
		if (!Node::IsEqual (sourceNode, targetNode)) {
			isEqual = false;
		}
		ValuePtr sourceResult = sourceNode->Evaluate (EmptyEvaluationEnv);
		ValuePtr targetResult = targetNode->Evaluate (EmptyEvaluationEnv);
		if (IntValue::Get (sourceResult) != IntValue::Get (targetResult)) {
			isEqual = false;
		}
		return isEqual;
	});
	return isEqual;
}

class AllNodeFilter : public NodeFilter
{
public:
	virtual bool NeedToProcessNode (const NodeId&) const override
	{
		return true;
	}
};

class SomeNodesFilter : public NodeFilter
{
public:
	SomeNodesFilter (const std::unordered_set<NodeId>& nodeIds) :
		nodeIds (nodeIds)
	{
	
	}

	virtual bool NeedToProcessNode (const NodeId& nodeId) const override
	{
		return nodeIds.find (nodeId) != nodeIds.end ();
	}

private:
	std::unordered_set<NodeId> nodeIds;
};

class EmptyEventHandler : public MergeEventHandler
{
public:
	EmptyEventHandler () :
		MergeEventHandler ()
	{
	}

	virtual ~EmptyEventHandler ()
	{
	
	}

	virtual void BeforeNodeDelete (const NE::NodeId&) override
	{
	
	}
};

static EmptyEventHandler eventHandler;

TEST (MergeAllNodesTest)
{
	NodeManager source;
	InitNodeManager (source);

	NodeManager target;
	AllNodeFilter allNodeFilter;
	NodeManagerMerge::AppendNodeManager (source, target, allNodeFilter);
	ASSERT (target.GetNodeCount () == 4);
	ASSERT (target.GetConnectionCount () == 3);

	ASSERT (FindNodesByName (target, L"1").size () == 1);
	ASSERT (FindNodesByName (target, L"2").size () == 1);
	ASSERT (FindNodesByName (target, L"3").size () == 1);
	ASSERT (FindNodesByName (target, L"4").size () == 1);
	
	NodeConstPtr targetNode4 = FindNodesByName (target, L"4")[0];
	ValuePtr result = targetNode4->Evaluate (EmptyEvaluationEnv);
	ASSERT (IntValue::Get (result) == 5);
}

TEST (MergeAllNodesTest_Scope)
{
	NodeManager target;
	{
		NodeManager source;
		InitNodeManager (source);
		AllNodeFilter allNodeFilter;
		NodeManagerMerge::AppendNodeManager (source, target, allNodeFilter);
	}

	ASSERT (target.GetNodeCount () == 4);
	ASSERT (target.GetConnectionCount () == 3);

	ASSERT (FindNodesByName (target, L"1").size () == 1);
	ASSERT (FindNodesByName (target, L"2").size () == 1);
	ASSERT (FindNodesByName (target, L"3").size () == 1);
	ASSERT (FindNodesByName (target, L"4").size () == 1);
	
	NodeConstPtr targetNode4 = FindNodesByName (target, L"4")[0];
	ValuePtr result = targetNode4->Evaluate (EmptyEvaluationEnv);
	ASSERT (IntValue::Get (result) == 5);
}


TEST (MergeAllNodesTwiceTest)
{
	NodeManager source;
	InitNodeManager (source);

	NodeManager target;
	AllNodeFilter allNodeFilter;
	NodeManagerMerge::AppendNodeManager (source, target, allNodeFilter);
	NodeManagerMerge::AppendNodeManager (source, target, allNodeFilter);
	ASSERT (target.GetNodeCount () == 8);
	ASSERT (target.GetConnectionCount () == 6);

	ASSERT (FindNodesByName (target, L"1").size () == 2);
	ASSERT (FindNodesByName (target, L"2").size () == 2);
	ASSERT (FindNodesByName (target, L"3").size () == 2);
	ASSERT (FindNodesByName (target, L"4").size () == 2);
	
	std::vector<NodeConstPtr> targetNode4 = FindNodesByName (target, L"4");
	for (const NodeConstPtr& node : targetNode4) {
		ValuePtr result = node->Evaluate (EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 5);
	}
}

TEST (MergeAllNodesToSameManager)
{
	NodeManager source;
	InitNodeManager (source);

	AllNodeFilter allNodeFilter;
	NodeManagerMerge::AppendNodeManager (source, source, allNodeFilter);
	ASSERT (source.GetNodeCount () == 8);
	ASSERT (source.GetConnectionCount () == 6);

	ASSERT (FindNodesByName (source, L"1").size () == 2);
	ASSERT (FindNodesByName (source, L"2").size () == 2);
	ASSERT (FindNodesByName (source, L"3").size () == 2);
	ASSERT (FindNodesByName (source, L"4").size () == 2);
	
	std::vector<NodeConstPtr> sourceNode4 = FindNodesByName (source, L"4");
	for (const NodeConstPtr& node : sourceNode4) {
		ValuePtr result = node->Evaluate (EmptyEvaluationEnv);
		ASSERT (IntValue::Get (result) == 5);
	}
}

TEST (MergeOnlyOneNode)
{
	NodeManager source;
	InitNodeManager (source);
	NodeId nodeIdToMerge = FindNodesByName (source, L"3")[0]->GetId ();

	NodeManager target;
	SomeNodesFilter someNodesFilter ({ nodeIdToMerge });
	NodeManagerMerge::AppendNodeManager (source, target, someNodesFilter);
	ASSERT (target.GetNodeCount () == 1);
	ASSERT (target.GetConnectionCount () == 0);
}

TEST (MergeMultipleNodes)
{
	NodeManager source;
	InitNodeManager (source);
	NodeId nodeIdToMerge1 = FindNodesByName (source, L"1")[0]->GetId ();
	NodeId nodeIdToMerge2 = FindNodesByName (source, L"2")[0]->GetId ();
	NodeId nodeIdToMerge3 = FindNodesByName (source, L"3")[0]->GetId ();

	NodeManager target;
	SomeNodesFilter someNodesFilter ({ nodeIdToMerge1, nodeIdToMerge2, nodeIdToMerge3 });
	NodeManagerMerge::AppendNodeManager (source, target, someNodesFilter);
	ASSERT (target.GetNodeCount () == 3);
	ASSERT (target.GetConnectionCount () == 2);

	NodeConstPtr targetNode3 = FindNodesByName (target, L"3")[0];
	ValuePtr result = targetNode3->Evaluate (EmptyEvaluationEnv);
	ASSERT (IntValue::Get (result) == 4);
}

TEST (NodeManagerUpdateTest_AddNode)
{
	NodeManager source;
	NodePtr sourceNode1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr sourceNode2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr sourceNode3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr sourceNode4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (sourceNode1->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (sourceNode2->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (sourceNode3->GetOutputSlot (SlotId ("out")), sourceNode4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	NodeManager::Clone (source, target);
	NodePtr targetNode5 = target.AddNode (NodePtr (new TestNode (L"5")));
	ASSERT (target.ContainsNode (targetNode5->GetId ()));
	NodeManagerMerge::UpdateNodeManager (source, target, eventHandler);
	ASSERT (IsEqualNodeManagers (source, target));
}

TEST (NodeManagerUpdateTest_DeleteNode)
{
	NodeManager source;
	NodePtr sourceNode1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr sourceNode2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr sourceNode3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr sourceNode4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (sourceNode1->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (sourceNode2->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (sourceNode3->GetOutputSlot (SlotId ("out")), sourceNode4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	NodeManager::Clone (source, target);
	target.DeleteNode (sourceNode4->GetId ());
	ASSERT (!target.ContainsNode (sourceNode4->GetId ()));
	NodeManagerMerge::UpdateNodeManager (source, target, eventHandler);
	ASSERT (IsEqualNodeManagers (source, target));
}

TEST (NodeManagerUpdateTest_DeleteNode2)
{
	NodeManager source;
	NodePtr sourceNode1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr sourceNode2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr sourceNode3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr sourceNode4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (sourceNode1->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (sourceNode2->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (sourceNode3->GetOutputSlot (SlotId ("out")), sourceNode4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	NodeManager::Clone (source, target);
	target.DeleteNode (sourceNode3->GetId ());
	ASSERT (!target.ContainsNode (sourceNode3->GetId ()));
	NodeManagerMerge::UpdateNodeManager (source, target, eventHandler);
	ASSERT (IsEqualNodeManagers (source, target));
}

TEST (NodeManagerUpdateTest_ModifyNode)
{
	NodeManager source;
	NodePtr sourceNode1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr sourceNode2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr sourceNode3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr sourceNode4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (sourceNode1->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (sourceNode2->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (sourceNode3->GetOutputSlot (SlotId ("out")), sourceNode4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	NodeManager::Clone (source, target);
	Node::Cast<TestNode> (target.GetNode (sourceNode3->GetId ()))->SetName (L"MyCoolNewName");
	ASSERT (Node::Cast<TestNode> (target.GetNode (sourceNode3->GetId ()))->GetName () == L"MyCoolNewName");
	NodeManagerMerge::UpdateNodeManager (source, target, eventHandler);
	ASSERT (IsEqualNodeManagers (source, target));
	ASSERT (Node::Cast<TestNode> (target.GetNode (sourceNode3->GetId ()))->GetName () == L"3");
}

TEST (NodeManagerUpdateTest_AddConnection)
{
	NodeManager source;
	NodePtr sourceNode1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr sourceNode2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr sourceNode3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr sourceNode4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (sourceNode1->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (sourceNode2->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (sourceNode3->GetOutputSlot (SlotId ("out")), sourceNode4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	NodeManager::Clone (source, target);
	target.ConnectOutputSlotToInputSlot (
		target.GetNode (sourceNode3->GetId ())->GetOutputSlot (SlotId ("out")),
		target.GetNode (sourceNode4->GetId ())->GetInputSlot (SlotId ("b"))
	);
	NodeManagerMerge::UpdateNodeManager (source, target, eventHandler);
	ASSERT (IsEqualNodeManagers (source, target));
}

TEST (NodeManagerUpdateTest_DeleteConnection)
{
	NodeManager source;
	NodePtr sourceNode1 = source.AddNode (NodePtr (new TestNode (L"1")));
	NodePtr sourceNode2 = source.AddNode (NodePtr (new TestNode (L"2")));
	NodePtr sourceNode3 = source.AddNode (NodePtr (new TestNode (L"3")));
	NodePtr sourceNode4 = source.AddNode (NodePtr (new TestNode (L"4")));
	source.ConnectOutputSlotToInputSlot (sourceNode1->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("a")));
	source.ConnectOutputSlotToInputSlot (sourceNode2->GetOutputSlot (SlotId ("out")), sourceNode3->GetInputSlot (SlotId ("b")));
	source.ConnectOutputSlotToInputSlot (sourceNode3->GetOutputSlot (SlotId ("out")), sourceNode4->GetInputSlot (SlotId ("a")));

	NodeManager target;
	NodeManager::Clone (source, target);
	target.DisconnectOutputSlotFromInputSlot (
		target.GetNode (sourceNode3->GetId ())->GetOutputSlot (SlotId ("out")),
		target.GetNode (sourceNode4->GetId ())->GetInputSlot (SlotId ("a"))
	);
	NodeManagerMerge::UpdateNodeManager (source, target, eventHandler);
	ASSERT (IsEqualNodeManagers (source, target));
}

}
