#include "NE_SingleValues.hpp"
#include "NE_StringUtils.hpp"
#include "NE_Localization.hpp"
#include "NE_Debug.hpp"

namespace NE
{

DynamicSerializationInfo	BooleanValue::serializationInfo (ObjectId ("{70071F15-3F3D-45CF-B7D5-CD4AF3E759FA}"), ObjectVersion (1), BooleanValue::CreateSerializableInstance);
DynamicSerializationInfo	IntValue::serializationInfo (ObjectId ("{FE348A6B-C5B8-42E2-9AD1-167BE291AEE8}"), ObjectVersion (1), IntValue::CreateSerializableInstance);
DynamicSerializationInfo	FloatValue::serializationInfo (ObjectId ("{D7FB2FE0-90E3-4A85-9C0F-9D90AFC8CD8A}"), ObjectVersion (1), FloatValue::CreateSerializableInstance);
DynamicSerializationInfo	DoubleValue::serializationInfo (ObjectId ("{4D6581DC-7A20-4F2A-A1A3-95BF6DDFFDB6}"), ObjectVersion (1), DoubleValue::CreateSerializableInstance);
DynamicSerializationInfo	StringValue::serializationInfo (ObjectId ("{FABFAA20-48F4-4F15-A9FB-FD8F05581F31}"), ObjectVersion (1), StringValue::CreateSerializableInstance);

BooleanValue::BooleanValue () :
	BooleanValue (false)
{

}

BooleanValue::BooleanValue (bool val) :
	GenericValue<bool> (val)
{

}

BooleanValue::~BooleanValue ()
{

}

ValuePtr BooleanValue::Clone () const
{
	return ValuePtr (new BooleanValue (val));
}

std::wstring BooleanValue::ToString (const StringSettings&) const
{
	return val ? Localize (L"true") : Localize (L"false");
}

Stream::Status BooleanValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	SingleValue::Read (inputStream);
	inputStream.Read (val);
	return inputStream.GetStatus ();
}

Stream::Status BooleanValue::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	SingleValue::Write (outputStream);
	outputStream.Write (val);
	return outputStream.GetStatus ();
}

StringValue::StringValue () :
	StringValue (std::wstring ())
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

NumberValue::NumberValue ()
{

}

NumberValue::~NumberValue ()
{

}

int NumberValue::ToInteger (const ValueConstPtr& val)
{
	return Value::Cast<NumberValue> (val)->ToInteger ();
}

int NumberValue::ToInteger (const ValuePtr& val)
{
	return Value::Cast<NumberValue> (val)->ToInteger ();
}

int NumberValue::ToInteger (Value* val)
{
	return Value::Cast<NumberValue> (val)->ToInteger ();
}

float NumberValue::ToFloat (const ValuePtr& val)
{
	return Value::Cast<NumberValue> (val)->ToFloat ();
}

float NumberValue::ToFloat (const ValueConstPtr& val)
{
	return Value::Cast<NumberValue> (val)->ToFloat ();
}

float NumberValue::ToFloat (Value* val)
{
	return Value::Cast<NumberValue> (val)->ToFloat ();
}

double NumberValue::ToDouble (const ValueConstPtr& val)
{
	return Value::Cast<NumberValue> (val)->ToDouble ();
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

float IntValue::ToFloat () const
{
	return (float) val;
}

double IntValue::ToDouble () const
{
	return (float) val;
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

FloatValue::FloatValue () :
	FloatValue (0.0f)
{

}

FloatValue::FloatValue (float val) :
	NumberValue (),
	GenericValue<float> (val)
{

}

FloatValue::~FloatValue ()
{

}

ValuePtr FloatValue::Clone () const
{
	return ValuePtr (new FloatValue (val));
}

std::wstring FloatValue::ToString (const StringSettings& stringSettings) const
{
	return DoubleToString (val, stringSettings);
}

int FloatValue::ToInteger () const
{
	return (int) val;
}

float FloatValue::ToFloat () const
{
	return val;
}

double FloatValue::ToDouble () const
{
	return (double) val;
}

Stream::Status FloatValue::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	SingleValue::Read (inputStream);
	inputStream.Read (val);
	return inputStream.GetStatus ();
}

Stream::Status FloatValue::Write (OutputStream& outputStream) const
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

float DoubleValue::ToFloat () const
{
	return (float) val;
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

}
