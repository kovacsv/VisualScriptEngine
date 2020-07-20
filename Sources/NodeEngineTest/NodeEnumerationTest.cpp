#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"

using namespace NE;

namespace NodeEnumerationTest
{

class TestNode : public Node
{
	DYNAMIC_SERIALIZABLE (TestNode);

public:
	TestNode ()
	{
	
	}

	virtual void Initialize () override
	{

	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
	{
		return nullptr;
	}

	virtual NE::Stream::Status Read (NE::InputStream&) override
	{
		return NE::Stream::Status::NoError;
	}

	virtual NE::Stream::Status Write (NE::OutputStream&) const override
	{
		return NE::Stream::Status::NoError;
	}
};

DYNAMIC_SERIALIZATION_INFO (TestNode, 1, "{3E9C9805-0724-4D0D-B052-BF2C27B1C29E}");

TEST (EnumerationOrderTest)
{
	NodeManager nodeManager;
	
	std::vector<NodeId> nodeAddOrder;

	static const size_t initialNodeCount = 100;
	for (size_t i = 0; i < initialNodeCount; i++) {
		nodeAddOrder.push_back (nodeManager.AddNode (NodePtr (new TestNode ()))->GetId ());
	}

	int nodeCount = 0;
	nodeManager.EnumerateNodes ([&] (const NE::NodePtr& node) {
		ASSERT (node->GetId () == nodeAddOrder[nodeCount]);
		nodeCount += 1;
		return true;
	});

	ASSERT (nodeCount == initialNodeCount);
}

}
