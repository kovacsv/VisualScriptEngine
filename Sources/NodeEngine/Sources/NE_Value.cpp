#include "NE_Value.hpp"
#include "NE_Debug.hpp"

namespace NE
{

SerializationInfo			Value::serializationInfo					(ObjectVersion (1));
SerializationInfo			SingleValue::serializationInfo				(ObjectVersion (1));
DynamicSerializationInfo	ListValue::serializationInfo				(ObjectId ("{95418CFC-BAE7-4FB3-8ED5-E6EC3AB930AC}"), ObjectVersion (1), ListValue::CreateSerializableInstance);

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

ListValue::ListValue (const std::vector<ValuePtr>& values) :
	values (values)
{

}

ListValue::~ListValue ()
{

}

ValuePtr ListValue::Clone () const
{
	ListValuePtr result (new ListValue ());
	for (const ValuePtr& value : values) {
		result->Push (value->Clone ());
	}
	return result;
}

std::wstring ListValue::ToString (const StringSettings& stringSettings) const
{
	std::wstring result = L"";
	for (size_t i = 0; i < GetSize (); ++i) {
		result += values[i]->ToString (stringSettings);
		if (i < GetSize () - 1) {
			result += stringSettings.GetListSeparator ();
			result += ' ';
		}
	}
	return result;
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
	for (const ValuePtr& value : values) {
		WriteDynamicObject (outputStream, value.get ());
	}
	return outputStream.GetStatus ();
}

size_t ListValue::GetSize () const
{
	return values.size ();
}

const NE::ValuePtr& ListValue::GetValue (size_t index) const
{
	return values[index];
}

void ListValue::Enumerate (const std::function<void (const ValuePtr&)>& processor) const
{
	for (const ValuePtr& value : values) {
		processor (value);
	}
}

void ListValue::Push (const ValuePtr& value)
{
	values.push_back (value);
}

ValueToListValueAdapter::ValueToListValueAdapter (const ValuePtr& val) :
	val (val)
{

}

size_t ValueToListValueAdapter::GetSize () const
{
	return 1;
}

const NE::ValuePtr& ValueToListValueAdapter::GetValue (size_t) const
{
	return val;
}

void ValueToListValueAdapter::Enumerate (const std::function<void (const ValuePtr&)>& processor) const
{
	processor (val);
}

ValuePtr CreateSingleValue (const ValuePtr& value)
{
	if (Value::IsType<SingleValue> (value)) {
		return value;
	} else if (Value::IsType<ListValue> (value)) {
		ListValuePtr listVal = Value::Cast<ListValue> (value);
		if (listVal->GetSize () != 1) {
			return nullptr;
		}
		return listVal->GetValue (0);
	}

	DBGBREAK ();
	return nullptr;

}

IListValuePtr CreateListValue (const ValuePtr& value)
{
	if (Value::IsType<SingleValue> (value)) {
		return IListValuePtr (new ValueToListValueAdapter (value));
	} else if (Value::IsType<ListValue> (value)) {
		return std::dynamic_pointer_cast<IListValue> (value);
	}

	DBGBREAK ();
	return nullptr;
}

}
