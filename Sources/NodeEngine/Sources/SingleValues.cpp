#include "SingleValues.hpp"

namespace NE
{

SerializationInfo			NumberValue::serializationInfo (ObjectId ("{05039AA5-7422-43DE-9770-F729B805CDA3}"), ObjectVersion (1));
DynamicSerializationInfo	IntValue::serializationInfo (ObjectId ("{FE348A6B-C5B8-42E2-9AD1-167BE291AEE8}"), ObjectVersion (1), IntValue::CreateSerializableInstance);
DynamicSerializationInfo	DoubleValue::serializationInfo (ObjectId ("{4D6581DC-7A20-4F2A-A1A3-95BF6DDFFDB6}"), ObjectVersion (1), DoubleValue::CreateSerializableInstance);

NumberValue::NumberValue () :
	SingleValue ()
{

}

NumberValue::~NumberValue ()
{

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
	return std::to_wstring (GetValue ());
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

}
