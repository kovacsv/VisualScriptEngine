#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "TestNodes.hpp"

using namespace NE;

namespace NodeManagerTest
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

	}

	virtual ValuePtr Calculate (NE::EvaluationEnv&) const override
	{
		return nullptr;
	}
};

TEST (NodeManagerTest)
{
	NodeManager nodeManager;
	NodePtr node1 (new TestNode ());
	NodePtr node2 (new TestNode ());
	ASSERT (node1->GetId () == NullNodeId);
	ASSERT (node2->GetId () == NullNodeId);
	ASSERT (nodeManager.AddNode (node1) != nullptr);
	ASSERT (nodeManager.AddNode (node2) != nullptr);
	ASSERT (node1->GetId () != NullNodeId);
	ASSERT (node2->GetId () != NullNodeId);
	ASSERT (nodeManager.ContainsNode (node1->GetId ()));
	ASSERT (nodeManager.ContainsNode (node2->GetId ()));
	ASSERT (nodeManager.GetNode (node1->GetId ()) == node1);
	ASSERT (nodeManager.GetNode (node2->GetId ()) == node2);
}

}
