#include "Value.hpp"
#include "Debug.hpp"

namespace NE
{

SerializationInfo			Value::serializationInfo			(ObjectId ("{524DAC7F-9CAA-4D23-A067-0DB47EF36316}"), ObjectVersion (1));
SerializationInfo			SingleValue::serializationInfo		(ObjectId ("{0F90CFD1-5619-469B-8677-6AEB6D1610A8}"), ObjectVersion (1));
SerializationInfo			IListValue::serializationInfo		(ObjectId ("{4AF05211-8150-460B-BA50-C70841657853}"), ObjectVersion (1));
DynamicSerializationInfo	ListValue::serializationInfo		(ObjectId ("{95418CFC-BAE7-4FB3-8ED5-E6EC3AB930AC}"), ObjectVersion (1), ListValue::CreateSerializableInstance);

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

Stream::Status IListValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	Value::Read (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status IListValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	Value::Write (outputStream);
	return outputStream.GetStatus ();
}

std::wstring IListValue::ToString () const
{
	// TODO: separator
	std::wstring result = L"";
	for (size_t i = 0; i < GetSize (); ++i) {
		result += GetValue (i)->ToString ();
		if (i < GetSize () - 1) {
			result += L", ";
		}
	}
	return result;
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

void ListValue::Push (const ValuePtr& value)
{
	values.push_back (value);
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

Stream::Status ListValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	IListValue::Read (inputStream);
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
	IListValue::Write (outputStream);
	outputStream.Write (values.size ());
	for (const ValuePtr& value : values) {
		WriteDynamicObject (outputStream, value.get ());
	}
	return outputStream.GetStatus ();
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
