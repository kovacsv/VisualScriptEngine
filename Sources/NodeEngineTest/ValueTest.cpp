#include "SimpleTest.hpp"
#include "NE_Value.hpp"
#include "NE_SingleValues.hpp"

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

	A& operator= (const A& rhs)
	{
		if (rhs == *this) {
			return *this;
		}
		x = rhs.x;
		return *this;
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
		return std::make_shared<AValue> (val);
	}

	virtual std::wstring ToString (const StringConverter&) const override
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

DYNAMIC_SERIALIZATION_INFO (AValue, 1, "{789CD6F8-A998-4A94-8B0A-96B5FD0925F4}");
DYNAMIC_SERIALIZATION_INFO (AListValue, 1, "{7427D535-508F-44E5-A076-3FAF1A35A413}");

static const BasicStringConverter DefaultStringConverter = GetDefaultStringConverter ();

TEST (ValueTest)
{
	IntValue intValue (5);
	ValuePtr intValuePtr (new IntValue (6));
	ASSERT (IsSingleValue (intValuePtr));
	ASSERT (!IsListValue (intValuePtr));
	ASSERT (Value::IsType<NumberValue> (&intValue));
	ASSERT (Value::IsType<IntValue> (&intValue));
	ASSERT (Value::IsType<IntValue> (intValuePtr));
	ASSERT (IntValue::Get (&intValue) == 5);
	ASSERT (IntValue::Get (intValuePtr) == 6);
	ASSERT (intValue.ToString (DefaultStringConverter) == L"5");
	ASSERT (intValue.ToDouble () == 5.0);
	ASSERT (GenericValue<int>::Get (&intValue) == 5);
	ASSERT (GenericValue<int>::Get (intValuePtr) == 6);
	ASSERT (NumberValue::ToDouble (&intValue) == 5.0);
	ASSERT (NumberValue::ToDouble (intValuePtr) == 6.0);
}

TEST (ListValueTest)
{
	ValuePtr listValue (new ListValue ());
	ASSERT (!IsSingleValue (listValue));
	ASSERT (IsListValue (listValue));
}

TEST (BooleanValueTest)
{
	BooleanValue val (true);
	ASSERT (Value::IsType<BooleanValue> (&val));
	ASSERT (BooleanValue::Get (&val) == true);
	ASSERT (val.ToString (DefaultStringConverter) == L"true");
	ASSERT (GenericValue<bool>::Get (&val) == true);
}

TEST (StringValueTest)
{
	StringValue val (L"Example");
	ASSERT (Value::IsType<StringValue> (&val));
	ASSERT (StringValue::Get (&val) == L"Example");
	ASSERT (val.ToString (DefaultStringConverter) == L"Example");
	ASSERT (GenericValue<std::wstring>::Get (&val) == L"Example");
}

TEST (StringSettingsTest)
{
	ListValue listVal;
	listVal.Push (ValuePtr (new DoubleValue (1.0)));
	listVal.Push (ValuePtr (new DoubleValue (2.0)));
	listVal.Push (ValuePtr (new DoubleValue (3.0)));

	ASSERT (listVal.ToString (DefaultStringConverter) == L"1.00, 2.00, 3.00");

	BasicStringSettings hungarianSettings (L',', L';', 3);
	BasicStringConverter hungarianConverter (hungarianSettings);
	ASSERT (listVal.ToString (hungarianConverter) == L"1,000; 2,000; 3,000");
}

TEST (CustomValueTest)
{
	AListValue aListVal;
	aListVal.Push (ValuePtr (new AValue (A (1))));
	aListVal.Push (ValuePtr (new AValue (A (2))));
	aListVal.Push (ValuePtr (new AValue (A (3))));
	ASSERT (aListVal.ToString (DefaultStringConverter) == L"1, 2, 3");
	ASSERT (aListVal.GetValue (0)->ToString (DefaultStringConverter) == L"1");
	ASSERT (aListVal.GetValue (1)->ToString (DefaultStringConverter) == L"2");
	ASSERT (aListVal.GetValue (2)->ToString (DefaultStringConverter) == L"3");
}

TEST (CloneTest)
{
	std::shared_ptr<AValue> original (new AValue (A (1)));
	ValuePtr cloned = original->Clone ();
	std::shared_ptr<AValue> typedCloned = Value::Cast<AValue> (cloned);
	ASSERT (typedCloned->GetValue () == A (1));
	original->SetValue (A (2));
	ASSERT (original->GetValue () == A (2));
	ASSERT (typedCloned->GetValue () == A (1));
}

TEST (ListCloneTest)
{
	ListValuePtr original (new ListValue ());
	original->Push (ValuePtr (new AValue (A (1))));
	original->Push (ValuePtr (new AValue (A (2))));
	original->Push (ValuePtr (new AValue (A (3))));
	ValuePtr cloned = original->Clone ();
	ListValuePtr typedCloned = Value::Cast<ListValue> (cloned);
	ASSERT (typedCloned->GetSize () == 3);
	ASSERT (AValue::Get (typedCloned->GetValue (0)) == A (1));
	ASSERT (AValue::Get (typedCloned->GetValue (1)) == A (2));
	ASSERT (AValue::Get (typedCloned->GetValue (2)) == A (3));
}

TEST (ConstNonConstTest)
{
	ValuePtr val (new IntValue (5));
	ValueConstPtr cval (new IntValue (5));
	ASSERT (IntValue::Get (val) == 5);
	ASSERT (IntValue::Get (cval) == 5);
}

TEST (FlattenTest)
{
	ListValuePtr theListValue (new ListValue ());

	ValuePtr innerSingleValue (new IntValue (1));
	ListValuePtr innerListValue (new ListValue ());
	innerListValue->Push (ValuePtr (new IntValue (2)));
	innerListValue->Push (ValuePtr (new IntValue (3)));

	theListValue->Push (innerSingleValue);
	theListValue->Push (innerListValue);

	ValueConstPtr flatten = FlattenValue (theListValue);
	ASSERT (IsComplexType<IntValue> (theListValue));
	ASSERT (IsComplexType<IntValue> (flatten));
	ASSERT (Value::IsType<ListValue> (flatten));
	ListValueConstPtr flattenList = Value::Cast<ListValue> (flatten);
	ASSERT (flattenList->GetSize () == 3);
	ASSERT (IntValue::Get (flattenList->GetValue (0)) == 1);
	ASSERT (IntValue::Get (flattenList->GetValue (1)) == 2);
	ASSERT (IntValue::Get (flattenList->GetValue (2)) == 3);
}

}
