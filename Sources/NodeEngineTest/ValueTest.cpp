#include "SimpleTest.hpp"
#include "Value.hpp"
#include "SingleValues.hpp"

using namespace NE;

namespace ValueTest
{

class A
{
public:
	A (int x) : x (x) {}
	int x;
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

class AListValue : public ListValue
{
public:
	AListValue () :
		ListValue ()
	{

	}
};

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
	ASSERT (GenericValueBase<int>::Get (&intValue) == 5);
	ASSERT (GenericValueBase<int>::Get (intValuePtr) == 6);
	ASSERT (NumberValue::ToDouble (&intValue) == 5.0);
	ASSERT (NumberValue::ToDouble (intValuePtr) == 6.0);
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

}
