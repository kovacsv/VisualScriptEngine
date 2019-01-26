#include "SimpleTest.hpp"
#include "NE_Node.hpp"
#include "NE_NodeManager.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestNodes.hpp"

using namespace NE;

namespace NumberTypeNodeTest
{

class IntNode : public SerializableTestNode
{
public:
	IntNode () :
		SerializableTestNode ()
	{
		
	}

	virtual void Initialize () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValuePtr (new IntValue (5));
	}
};

class DoubleNode : public SerializableTestNode
{
public:
	DoubleNode () :
		SerializableTestNode ()
	{
		
	}

	virtual void Initialize () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlot (SlotId ("out"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValuePtr (new DoubleValue (6.0));
	}
};

class NumberInputSlot : public InputSlot
{
public:
	NumberInputSlot (const SlotId& id) :
		InputSlot (id, ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Single)
	{

	}
};

class NumberAdderNode : public SerializableTestNode
{
public:
	NumberAdderNode () :
		SerializableTestNode ()
	{
		
	}

	virtual void Initialize () override
	{
		RegisterInputSlot (InputSlotPtr (new NumberInputSlot (SlotId ("first"))));
		RegisterInputSlot (InputSlotPtr (new NumberInputSlot (SlotId ("second"))));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr a = EvaluateInputSlot (SlotId ("first"), env);
		ValueConstPtr b = EvaluateInputSlot (SlotId ("second"), env);
		double result = NumberValue::ToDouble (a) + NumberValue::ToDouble (b);
		return ValuePtr (new DoubleValue (result));
	}
};

TEST (NumberConnectionTest)
{
	NodeManager manager;

	NodePtr intNode = manager.AddNode (NodePtr (new IntNode ()));
	NodePtr doubleNode = manager.AddNode (NodePtr (new DoubleNode ()));
	NodePtr adderNode = manager.AddNode (NodePtr (new NumberAdderNode ()));

	ASSERT (manager.ConnectOutputSlotToInputSlot (intNode->GetOutputSlot (SlotId ("out")), adderNode->GetInputSlot (SlotId ("first"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (doubleNode->GetOutputSlot (SlotId ("out")), adderNode->GetInputSlot (SlotId ("second"))));

	ValueConstPtr result = adderNode->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (Value::IsType<NumberValue> (result));
	ASSERT (Value::IsType<DoubleValue> (result));
	ASSERT (Value::Cast<NumberValue> (result.get ())->ToDouble () == 11.0);
	ASSERT (DoubleValue::Get (result) == 11.0);
}

}
