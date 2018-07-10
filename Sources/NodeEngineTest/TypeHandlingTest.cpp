#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "NE_GenericValue.hpp"
#include "TestNodes.hpp"

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
	DYNAMIC_SERIALIZABLE (AValue);

public:
	AValue () :
		AValue (A ())
	{

	}

	AValue (const A& val) :
		GenericValue<A> (val)
	{

	}

	virtual ValuePtr Clone () const override
	{
		return ValuePtr (new AValue (val));
	}

	virtual std::wstring ToString (const StringSettings&) const override
	{
		return std::to_wstring (GetValue ().x);
	}
};

class BValue : public GenericValue<B>
{
	DYNAMIC_SERIALIZABLE (BValue);

public:
	BValue () :
		BValue (B ())
	{

	}

	BValue (const B& val) :
		GenericValue<B> (val)
	{

	}

	virtual ValuePtr Clone () const override
	{
		return ValuePtr (new BValue (val));
	}

	virtual std::wstring ToString (const StringSettings&) const override
	{
		return std::to_wstring (GetValue ().x);
	}
};


class ABPairValue : public GenericValue<ABPair>
{
	DYNAMIC_SERIALIZABLE (ABPairValue);

public:
	ABPairValue () :
		ABPairValue (ABPair ())
	{

	}

	ABPairValue (const ABPair& val) :
		GenericValue<ABPair> (val)
	{

	}

	virtual ValuePtr Clone () const override
	{
		return ValuePtr (new ABPairValue (val));
	}

	virtual std::wstring ToString (const StringSettings&) const override
	{
		return std::to_wstring (GetValue ().a.x) + std::to_wstring (GetValue ().b.x);
	}
};

DynamicSerializationInfo AValue::serializationInfo (ObjectId ("{F1B3A71F-EAD2-4732-8814-38344F7F40EB}"), ObjectVersion (1), AValue::CreateSerializableInstance);
DynamicSerializationInfo BValue::serializationInfo (ObjectId ("{9A2698D6-A78B-4C59-BA0A-63171A9152D8}"), ObjectVersion (1), BValue::CreateSerializableInstance);
DynamicSerializationInfo ABPairValue::serializationInfo (ObjectId ("{D88A8420-6F72-4A88-A1AA-DCA827ABD2E8}"), ObjectVersion (1), ABPairValue::CreateSerializableInstance);

class NodeWithDifferentTypes : public SerializableTestNode
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
		SerializableTestNode ()
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
};

class OutputNodeA : public SerializableTestNode
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
		SerializableTestNode ()
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
};

class OutputNodeB : public SerializableTestNode
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
		SerializableTestNode ()
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
