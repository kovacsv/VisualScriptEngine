#include "SingleValues.hpp"
#include "StringUtils.hpp"

namespace NE
{

SerializationInfo			NumberValue::serializationInfo (ObjectId ("{05039AA5-7422-43DE-9770-F729B805CDA3}"), ObjectVersion (1));
DynamicSerializationInfo	IntValue::serializationInfo (ObjectId ("{FE348A6B-C5B8-42E2-9AD1-167BE291AEE8}"), ObjectVersion (1), IntValue::CreateSerializableInstance);
DynamicSerializationInfo	DoubleValue::serializationInfo (ObjectId ("{4D6581DC-7A20-4F2A-A1A3-95BF6DDFFDB6}"), ObjectVersion (1), DoubleValue::CreateSerializableInstance);
DynamicSerializationInfo	StringValue::serializationInfo (ObjectId ("{FABFAA20-48F4-4F15-A9FB-FD8F05581F31}"), ObjectVersion (1), StringValue::CreateSerializableInstance);

NumberValue::NumberValue () :
	SingleValue ()
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

Stream::Status NumberValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	SingleValue::Read (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status NumberValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	SingleValue::Write (outputStream);
	return outputStream.GetStatus ();
}

IntValue::IntValue () :
	NumberValue (),
	GenericValueBase (0)
{

}

IntValue::IntValue (int val) :
	NumberValue (),
	GenericValueBase (val)
{

}

IntValue::~IntValue ()
{

}

std::wstring IntValue::ToString () const
{
	return std::to_wstring (GetValue ());
}

int IntValue::ToInteger () const
{
	return GetValue ();
}

double IntValue::ToDouble () const
{
	return GetValue ();
}

Stream::Status IntValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	NumberValue::Read (inputStream);
	inputStream.Read (val);
	return inputStream.GetStatus ();
}

Stream::Status IntValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	NumberValue::Write (outputStream);
	outputStream.Write (val);
	return outputStream.GetStatus ();
}

DoubleValue::DoubleValue () :
	NumberValue (),
	GenericValueBase (0.0)
{

}

DoubleValue::DoubleValue (double val) :
	NumberValue (),
	GenericValueBase (val)
{

}

DoubleValue::~DoubleValue ()
{

}

std::wstring DoubleValue::ToString () const
{
	// TODO: Precision
	return DoubleToString (GetValue (), 2);
}

int DoubleValue::ToInteger () const
{
	return (int) GetValue ();
}

double DoubleValue::ToDouble () const
{
	return GetValue ();
}

Stream::Status DoubleValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	NumberValue::Read (inputStream);
	inputStream.Read (val);
	return inputStream.GetStatus ();
}

Stream::Status DoubleValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	NumberValue::Write (outputStream);
	outputStream.Write (val);
	return outputStream.GetStatus ();
}

StringValue::StringValue () :
	SingleValue (),
	GenericValueBase (L"")
{

}

StringValue::StringValue (const std::wstring& val) :
	SingleValue (),
	GenericValueBase (val)
{

}

StringValue::~StringValue ()
{

}

std::wstring StringValue::ToString () const
{
	return GetValue ();
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

}
