#include "NE_SingleValues.hpp"
#include "NE_StringUtils.hpp"
#include "NE_Debug.hpp"

namespace NE
{

DynamicSerializationInfo	IntValue::serializationInfo (ObjectId ("{FE348A6B-C5B8-42E2-9AD1-167BE291AEE8}"), ObjectVersion (1), IntValue::CreateSerializableInstance);
DynamicSerializationInfo	DoubleValue::serializationInfo (ObjectId ("{4D6581DC-7A20-4F2A-A1A3-95BF6DDFFDB6}"), ObjectVersion (1), DoubleValue::CreateSerializableInstance);
DynamicSerializationInfo	StringValue::serializationInfo (ObjectId ("{FABFAA20-48F4-4F15-A9FB-FD8F05581F31}"), ObjectVersion (1), StringValue::CreateSerializableInstance);
DynamicSerializationInfo	EnumerationValue::serializationInfo (ObjectId ("{C0702048-B054-4300-B9F1-054067EA46A7}"), ObjectVersion (1), EnumerationValue::CreateSerializableInstance);

NumberValue::NumberValue ()
{

}

NumberValue::~NumberValue ()
{

}

int NumberValue::ToInteger (const ValuePtr& val)
{
	return Value::Cast<NumberValue> (val)->ToInteger ();
}

int NumberValue::ToInteger (Value* val)
{
	return Value::Cast<NumberValue> (val)->ToInteger ();
}

double NumberValue::ToDouble (const ValuePtr& val)
{
	return Value::Cast<NumberValue> (val)->ToDouble ();
}

double NumberValue::ToDouble (Value* val)
{
	return Value::Cast<NumberValue> (val)->ToDouble ();
}

IntValue::IntValue () :
	IntValue (0)
{

}

IntValue::IntValue (int val) :
	NumberValue (),
	GenericValue<int> (val)
{

}

IntValue::~IntValue ()
{

}

ValuePtr IntValue::Clone () const
{
	return ValuePtr (new IntValue (val));
}

std::wstring IntValue::ToString (const StringSettings&) const
{
	return std::to_wstring (val);
}

int IntValue::ToInteger () const
{
	return val;
}

double IntValue::ToDouble () const
{
	return val;
}

Stream::Status IntValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	SingleValue::Read (inputStream);
	inputStream.Read (val);
	return inputStream.GetStatus ();
}

Stream::Status IntValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	SingleValue::Write (outputStream);
	outputStream.Write (val);
	return outputStream.GetStatus ();
}

DoubleValue::DoubleValue () :
	DoubleValue (0.0)
{

}

DoubleValue::DoubleValue (double val) :
	NumberValue (),
	GenericValue<double> (val)
{

}

DoubleValue::~DoubleValue ()
{

}

ValuePtr DoubleValue::Clone () const
{
	return ValuePtr (new DoubleValue (val));
}

std::wstring DoubleValue::ToString (const StringSettings& stringSettings) const
{
	return DoubleToString (val, stringSettings);
}

int DoubleValue::ToInteger () const
{
	return (int) val;
}

double DoubleValue::ToDouble () const
{
	return val;
}

Stream::Status DoubleValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	SingleValue::Read (inputStream);
	inputStream.Read (val);
	return inputStream.GetStatus ();
}

Stream::Status DoubleValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	SingleValue::Write (outputStream);
	outputStream.Write (val);
	return outputStream.GetStatus ();
}

StringValue::StringValue () :
	StringValue (L"")
{

}

StringValue::StringValue (const std::wstring& val) :
	GenericValue<std::wstring> (val)
{

}

StringValue::~StringValue ()
{

}

ValuePtr StringValue::Clone () const
{
	return ValuePtr (new StringValue (val));
}

std::wstring StringValue::ToString (const StringSettings&) const
{
	return val;
}

Stream::Status StringValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	SingleValue::Read (inputStream);
	inputStream.Read (val);
	return inputStream.GetStatus ();
}

Stream::Status StringValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	SingleValue::Write (outputStream);
	outputStream.Write (val);
	return outputStream.GetStatus ();
}

EnumerationValue::EnumerationValue () :
	EnumerationValue (-1, {})
{

}

EnumerationValue::EnumerationValue (int val, const std::vector<std::wstring>& enumValues) :
	GenericValue<int> (val),
	enumValues (enumValues)
{

}

EnumerationValue::~EnumerationValue ()
{

}

ValuePtr EnumerationValue::Clone () const
{
	return ValuePtr (new EnumerationValue (val, enumValues));
}

std::wstring EnumerationValue::ToString (const StringSettings&) const
{
	if (DBGERROR (val < 0 || val >= enumValues.size ())) {
		return L"";
	}
	return enumValues[val];
}

Stream::Status EnumerationValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	SingleValue::Read (inputStream);
	inputStream.Read (val);
	size_t enumValueCount = 0;
	for (size_t i = 0; i < enumValueCount; i++) {
		std::wstring enumValue;
		inputStream.Read (enumValue);
		enumValues.push_back (enumValue);
	}
	return inputStream.GetStatus ();
}

Stream::Status EnumerationValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	SingleValue::Write (outputStream);
	outputStream.Write (val);
	outputStream.Write (enumValues.size ());
	for (const std::wstring& enumValue : enumValues) {
		outputStream.Write (enumValue);
	}
	return outputStream.GetStatus ();
}

}
