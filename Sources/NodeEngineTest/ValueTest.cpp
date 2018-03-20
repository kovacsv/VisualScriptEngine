#include "SimpleTest.hpp"
#include "Value.hpp"
#include "SingleValues.hpp"

using namespace NE;

namespace ValueTest
{

class A
{
public:
	A (const A& rhs) :
		x (rhs.x)
	{

	}

	A (int x) :
		x (x)
	{
	}

	bool operator== (const A& rhs) const
	{
		return x == rhs.x;
	}

	int x;
};

class AValue : public GenericValue<A>
{
	DYNAMIC_SERIALIZABLE (AValue);

public:
	AValue () :
		AValue (A (0))
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

	virtual std::wstring ToString () const override
	{
		return std::to_wstring (GetValue ().x);
	}
};

class AListValue : public ListValue
{
	DYNAMIC_SERIALIZABLE (AListValue);

public:
	AListValue () :
		ListValue ()
	{

	}
};

DynamicSerializationInfo AValue::serializationInfo (ObjectId ("{789CD6F8-A998-4A94-8B0A-96B5FD0925F4}"), ObjectVersion (1), AValue::CreateSerializableInstance);
DynamicSerializationInfo AListValue::serializationInfo (ObjectId ("{7427D535-508F-44E5-A076-3FAF1A35A413}"), ObjectVersion (1), AListValue::CreateSerializableInstance);

TEST (ValueTest)
{
	IntValue intValue (5);
	ValuePtr intValuePtr (new IntValue (6));
	ASSERT (Value::IsType<NumberValue> (&intValue));
	ASSERT (Value::IsType<IntValue> (&intValue));
	ASSERT (Value::IsType<IntValue> (intValuePtr));
	ASSERT (IntValue::Get (&intValue) == 5);
	ASSERT (IntValue::Get (intValuePtr) == 6);
	ASSERT (intValue.ToString () == L"5");
	ASSERT (intValue.ToDouble () == 5.0);
	ASSERT (GenericValue<int>::Get (&intValue) == 5);
	ASSERT (GenericValue<int>::Get (intValuePtr) == 6);
	ASSERT (NumberValue::ToDouble (&intValue) == 5.0);
	ASSERT (NumberValue::ToDouble (intValuePtr) == 6.0);
}

TEST (StringValueTest)
{
	StringValue val (L"Example");
	ASSERT (Value::IsType<StringValue> (&val));
	ASSERT (StringValue::Get (&val) == L"Example");
	ASSERT (val.ToString () == L"Example");
	ASSERT (GenericValue<std::wstring>::Get (&val) == L"Example");
}

TEST (CustomValueTest)
{
	AListValue aListVal;
	aListVal.Push (ValuePtr (new AValue (A (1))));
	aListVal.Push (ValuePtr (new AValue (A (2))));
	aListVal.Push (ValuePtr (new AValue (A (3))));
	ASSERT (aListVal.GetValue (0)->ToString () == L"1");
	ASSERT (aListVal.GetValue (1)->ToString () == L"2");
	ASSERT (aListVal.GetValue (2)->ToString () == L"3");
}

TEST (CloneTest)
{
	std::shared_ptr<AValue> original (new AValue (A (1)));
	ValuePtr cloned = original->Clone ();
	std::shared_ptr<AValue> typedCloned = Value::Cast<AValue> (cloned);
	ASSERT (AValue::Get (typedCloned) == A (1));
	typedCloned->SetValue (2);
	ASSERT (AValue::Get (typedCloned) == A (2));
	ASSERT (AValue::Get (original) == A (1));
}

}
