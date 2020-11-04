#include "NE_Value.hpp"
#include "NE_Debug.hpp"

namespace NE
{

SERIALIZATION_INFO (Value, 1);
SERIALIZATION_INFO (SingleValue, 1);
DYNAMIC_SERIALIZATION_INFO (ListValue, 1, "{95418CFC-BAE7-4FB3-8ED5-E6EC3AB930AC}");

Value::Value ()
{

}

Value::~Value ()
{

}

Stream::Status Value::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status Value::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	return outputStream.GetStatus ();
}

SingleValue::SingleValue ()
{

}

SingleValue::~SingleValue ()
{

}

Stream::Status SingleValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	Value::Read (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status SingleValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	Value::Write (outputStream);
	return outputStream.GetStatus ();
}

IListValue::IListValue ()
{

}

IListValue::~IListValue ()
{

}

ListValue::ListValue ()
{

}

ListValue::ListValue (const std::vector<ValueConstPtr>& values) :
	values (values)
{

}

ListValue::~ListValue ()
{

}

ValuePtr ListValue::Clone () const
{
	ListValuePtr result (new ListValue ());
	for (const ValueConstPtr& value : values) {
		result->Push (value->Clone ());
	}
	return result;
}

std::wstring ListValue::ToString (const StringConverter& stringConverter) const
{
	class ListEnumerator : public StringConverter::ListEnumerator
	{
	public:
		ListEnumerator (const ListValue* val, const StringConverter& converter) :
			val (val),
			converter (converter)
		{
		}

		virtual size_t GetSize () const override
		{
			return val->GetSize ();
		}

		virtual std::wstring GetItem (size_t index) const override
		{
			return val->GetValue (index)->ToString (converter);
		}

	private:
		const ListValue*		val;
		const StringConverter&	converter;
	};

	ListEnumerator enumerator (this, stringConverter);
	return stringConverter.ListToString (enumerator);
}

Stream::Status ListValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	Value::Read (inputStream);
	size_t valueCount = 0;
	inputStream.Read (valueCount);
	for (size_t i = 0; i < valueCount; i++) {
		ValuePtr value (ReadDynamicObject<Value> (inputStream));
		if (DBGVERIFY (value != nullptr)) {
			values.push_back (value);
		}
	}
	return inputStream.GetStatus ();
}

Stream::Status ListValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	Value::Write (outputStream);
	outputStream.Write (values.size ());
	for (const ValueConstPtr& value : values) {
		WriteDynamicObject (outputStream, value.get ());
	}
	return outputStream.GetStatus ();
}

size_t ListValue::GetSize () const
{
	return values.size ();
}

const ValueConstPtr& ListValue::GetValue (size_t index) const
{
	return values[index];
}

bool ListValue::Enumerate (const std::function<bool (const ValueConstPtr&)>& processor) const
{
	for (const ValueConstPtr& value : values) {
		if (!processor (value)) {
			return false;
		}
	}
	return true;
}

void ListValue::Push (const ValueConstPtr& value)
{
	values.push_back (value);
}

ValueToListValueAdapter::ValueToListValueAdapter (const ValueConstPtr& val) :
	val (val)
{

}

size_t ValueToListValueAdapter::GetSize () const
{
	return 1;
}

const ValueConstPtr& ValueToListValueAdapter::GetValue (size_t) const
{
	return val;
}

bool ValueToListValueAdapter::Enumerate (const std::function<bool (const ValueConstPtr&)>& processor) const
{
	return processor (val);
}

bool IsSingleValue (const ValueConstPtr& value)
{
	return Value::IsType<SingleValue> (value);
}

bool IsListValue (const ValueConstPtr& value)
{
	return Value::IsType<ListValue> (value);
}

ValueConstPtr CreateSingleValue (const ValueConstPtr& value)
{
	if (Value::IsType<SingleValue> (value)) {
		return value;
	} else if (Value::IsType<ListValue> (value)) {
		ListValueConstPtr listVal = Value::Cast<ListValue> (value);
		if (listVal->GetSize () != 1) {
			return nullptr;
		}
		return listVal->GetValue (0);
	}

	DBGBREAK ();
	return nullptr;

}

IListValueConstPtr CreateListValue (const ValueConstPtr& value)
{
	if (Value::IsType<SingleValue> (value)) {
		return IListValueConstPtr (new ValueToListValueAdapter (value));
	} else if (Value::IsType<ListValue> (value)) {
		return std::dynamic_pointer_cast<const IListValue> (value);
	}

	DBGBREAK ();
	return nullptr;
}

bool FlatEnumerate (const ValueConstPtr& value, const std::function<bool (const ValueConstPtr&)>& processor)
{
	if (value == nullptr) {
		return processor (value);
	}
	IListValueConstPtr listValue = CreateListValue (value);
	return listValue->Enumerate ([&] (const ValueConstPtr& innerValue) {
		if (Value::IsType<SingleValue> (innerValue)) {
			if (!processor (innerValue)) {
				return false;
			}
		} else {
			return FlatEnumerate (innerValue, processor);
		}
		return true;
	});
}

ValueConstPtr FlattenValue (const ValueConstPtr& value)
{
	ListValuePtr listValue (new ListValue ());
	FlatEnumerate (value, [&] (const ValueConstPtr& value) {
		listValue->Push (value);
		return true;
	});
	return listValue;
}

}
