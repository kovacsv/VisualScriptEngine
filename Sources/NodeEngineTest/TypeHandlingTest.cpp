#include "SimpleTest.hpp"
#include "NodeManager.hpp"
#include "Node.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"
#include "GenericValue.hpp"

using namespace NE;

namespace TypeHandlingTest
{

class A
{
public:
	int x;
};

class B
{
public:
	int x;
};

class ABPair
{
public:
	A a;
	B b;
};

class AValue : public GenericValue<A>
{
public:
	AValue (const A& val) :
		GenericValue<A> (val)
	{

	}

	virtual std::wstring ToString () const override
	{
		return std::to_wstring (GetValue ().x);
	}
};

class BValue : public GenericValue<B>
{
public:
	BValue (const B& val) :
		GenericValue<B> (val)
	{

	}

	virtual std::wstring ToString () const override
	{
		return std::to_wstring (GetValue ().x);
	}
};

class ABPairValue : public GenericValue<ABPair>
{
public:
	ABPairValue (const ABPair& val) :
		GenericValue<ABPair> (val)
	{

	}

	virtual std::wstring ToString () const override
	{
		return std::to_wstring (GetValue ().a.x) + std::to_wstring (GetValue ().b.x);
	}
};

class NodeWithDifferentTypes : public Node
{
public:
	class OutputSlotA : public OutputSlot
	{
	public:
		OutputSlotA (const SlotId& id) :
			OutputSlot (id)
		{
		
		}

		virtual ValuePtr Evaluate (NE::EvaluationEnv& env) const override
		{
			ValuePtr ownerVal = EvaluateOwnerNode (env);
			GenericValue<ABPair>* pair = Value::Cast<ABPairValue> (ownerVal.get ());
			return ValuePtr (new AValue (pair->GetValue ().a));
		}
	};

	class OutputSlotB : public OutputSlot
	{
	public:
		OutputSlotB (const SlotId& id) :
			OutputSlot (id)
		{
		
		}

		virtual ValuePtr Evaluate (NE::EvaluationEnv& env) const override
		{
			ValuePtr ownerVal = EvaluateOwnerNode (env);
			GenericValue<ABPair>* pair = Value::Cast<ABPairValue> (ownerVal.get ());
			return ValuePtr (new BValue (pair->GetValue ().b));
		}
	};

	NodeWithDifferentTypes () :
		Node ()
	{
	
	}

	virtual void RegisterSlots () override
	{
		RegisterOutputSlot (OutputSlotPtr (new OutputSlotA (SlotId ("outA"))));
		RegisterOutputSlot (OutputSlotPtr (new OutputSlotB (SlotId ("outB"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv&) const override
	{
		ABPair pair;
		pair.a.x = 5;
		pair.b.x = 6;
		return ValuePtr (new ABPairValue (pair));
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
	}
};

class OutputNodeA : public Node
{
public:
	class InputSlotA : public InputSlot
	{
	public:
		InputSlotA (const SlotId& id) :
			InputSlot (id, nullptr, OutputSlotConnectionMode::Single)
		{
		
		}
	};

	OutputNodeA () :
		Node ()
	{

	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlotA (SlotId ("inA"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		return EvaluateSingleInputSlot (SlotId ("inA"), env);
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
	}
};

class OutputNodeB : public Node
{
public:
	class InputSlotB : public InputSlot
	{
	public:
		InputSlotB (const SlotId& id) :
			InputSlot (id, nullptr, OutputSlotConnectionMode::Single)
		{
		
		}
	};

	OutputNodeB () :
		Node ()
	{

	}

	virtual void RegisterSlots () override
	{
		RegisterInputSlot (InputSlotPtr (new InputSlotB (SlotId ("inB"))));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		return EvaluateSingleInputSlot (SlotId ("inB"), env);
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
	}
};

TEST (TypeHandlingTest)
{
	NodeManager manager;

	NodePtr diffTypesNode = manager.AddNode (NodePtr (new NodeWithDifferentTypes ()));
	NodePtr outputA = manager.AddNode (NodePtr (new OutputNodeA ()));
	NodePtr outputB = manager.AddNode (NodePtr (new OutputNodeB ()));
	
	ASSERT (manager.ConnectOutputSlotToInputSlot (diffTypesNode->GetOutputSlot (SlotId ("outA")), outputA->GetInputSlot (SlotId ("inA"))));
	ASSERT (manager.ConnectOutputSlotToInputSlot (diffTypesNode->GetOutputSlot (SlotId ("outB")), outputB->GetInputSlot (SlotId ("inB"))));

	ValuePtr origVal = diffTypesNode->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (Value::IsType<GenericValue<ABPair>> (origVal));

	ValuePtr aVal = outputA->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (Value::IsType<GenericValue<A>> (aVal));
	ASSERT (Value::Cast<GenericValue<A>> (aVal.get ())->GetValue ().x == 5);

	ValuePtr bVal = outputB->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (Value::IsType<GenericValue<B>> (bVal));
	ASSERT (Value::Cast<GenericValue<B>> (bVal.get ())->GetValue ().x == 6);
}

}
