#include "SimpleTest.hpp"
#include "Serializable.hpp"
#include "SingleValues.hpp"
#include "MemoryStream.hpp"

#include <memory>

using namespace NE;

class MySerializable : public DynamicSerializable
{
	SERIALIZABLE;

public:
	MySerializable () :
		intVal (0),
		stringVal ("")
	{

	}

	virtual void VirtualFunction () const = 0;

	virtual Stream::Status Read (InputStream& inputStream) override
	{
		ObjectHeader header (inputStream);
		DBGASSERT (header.GetVersion () == ObjectVersion (2));
		inputStream.Read (intVal);
		inputStream.Read (stringVal);
		return inputStream.GetStatus ();
	}

	virtual Stream::Status Write (OutputStream& outputStream) const override
	{
		ObjectHeader header (outputStream, serializationInfo);
		outputStream.Write (intVal);
		outputStream.Write (stringVal);
		return outputStream.GetStatus ();
	}

	size_t			intVal;
	std::string		stringVal;
};

SerializationInfo MySerializable::serializationInfo (ObjectVersion (2));

class MyInheritedSerializable : public MySerializable
{
	DYNAMIC_SERIALIZABLE (MyInheritedSerializable);

public:
	MyInheritedSerializable () :
		inheritedIntVal (0)
	{

	}

	virtual void VirtualFunction () const override
	{
	
	}

	virtual Stream::Status Read (InputStream& inputStream) override
	{
		ObjectHeader header (inputStream);
		DBGASSERT (header.GetVersion () == ObjectVersion (3));
		MySerializable::Read (inputStream);
		inputStream.Read (inheritedIntVal);
		return inputStream.GetStatus ();
	}

	virtual Stream::Status Write (OutputStream& outputStream) const override
	{
		ObjectHeader header (outputStream, serializationInfo);
		MySerializable::Write (outputStream);
		outputStream.Write (inheritedIntVal);
		return outputStream.GetStatus ();
	}

	size_t inheritedIntVal;
};

DynamicSerializationInfo MyInheritedSerializable::serializationInfo (ObjectId ("{7CB03170-3A8D-43EC-9806-F423A39F07DA}"), ObjectVersion (3), MyInheritedSerializable::CreateSerializableInstance);

class MyInheritedSerializable2 : public MyInheritedSerializable
{
	DYNAMIC_SERIALIZABLE (MyInheritedSerializable2);

public:
	MyInheritedSerializable2 () :
		inheritedIntVal2 (0)
	{

	}

	virtual void VirtualFunction () const override
	{
	
	}

	virtual Stream::Status Read (InputStream& inputStream) override
	{
		ObjectHeader header (inputStream);
		DBGASSERT (header.GetVersion () == ObjectVersion (4));
		MyInheritedSerializable::Read (inputStream);
		inputStream.Read (inheritedIntVal2);
		return inputStream.GetStatus ();
	}

	virtual Stream::Status Write (OutputStream& outputStream) const override
	{
		ObjectHeader header (outputStream, serializationInfo);
		MyInheritedSerializable::Write (outputStream);
		outputStream.Write (inheritedIntVal2);
		return outputStream.GetStatus ();
	}

	size_t inheritedIntVal2;
};

DynamicSerializationInfo MyInheritedSerializable2::serializationInfo (ObjectId ("{C25B8A4C-9DAB-4FFF-9F13-28D9FF6A3190}"), ObjectVersion (4), MyInheritedSerializable2::CreateSerializableInstance);

namespace SerializableTest
{

TEST (BasicTest)
{
	MyInheritedSerializable serializable;
	const DynamicSerializationInfo* info = serializable.GetDynamicSerializationInfo ();
	ASSERT (info->GetObjectId () == ObjectId ("{7CB03170-3A8D-43EC-9806-F423A39F07DA}"));

	Serializable* createdSerializable = CreateDynamicObject (ObjectId ("{7CB03170-3A8D-43EC-9806-F423A39F07DA}"));
	ASSERT (createdSerializable != nullptr);
	ASSERT (dynamic_cast<MyInheritedSerializable*> (createdSerializable) != nullptr);
	delete createdSerializable;
}

TEST (BasicTest2)
{
	MyInheritedSerializable2 serializable;
	const DynamicSerializationInfo* info = serializable.GetDynamicSerializationInfo ();
	ASSERT (info->GetObjectId () == ObjectId ("{C25B8A4C-9DAB-4FFF-9F13-28D9FF6A3190}"));

	Serializable* createdSerializable = CreateDynamicObject (ObjectId ("{C25B8A4C-9DAB-4FFF-9F13-28D9FF6A3190}"));
	ASSERT (createdSerializable != nullptr);
	ASSERT (dynamic_cast<MyInheritedSerializable2*> (createdSerializable) != nullptr);
	delete createdSerializable;
}

TEST (IOTest)
{
	MyInheritedSerializable serializable;
	serializable.intVal = 42;
	serializable.stringVal = "Alma";

	MemoryOutputStream outputStream;
	serializable.Write (outputStream);

	MyInheritedSerializable serializable2;
	MemoryInputStream inputStream (outputStream.GetBuffer ());
	serializable2.Read (inputStream);

	ASSERT (serializable.intVal == serializable2.intVal);
	ASSERT (serializable.stringVal == serializable2.stringVal);
}

TEST (InheritedIOTest)
{
	MyInheritedSerializable serializable;
	serializable.intVal = 42;
	serializable.stringVal = "Alma";
	serializable.inheritedIntVal = 55;

	MemoryOutputStream outputStream;
	serializable.Write (outputStream);

	MyInheritedSerializable newSerializable;
	MemoryInputStream inputStream (outputStream.GetBuffer ());
	newSerializable.Read (inputStream);

	ASSERT (serializable.intVal == newSerializable.intVal);
	ASSERT (serializable.stringVal == newSerializable.stringVal);
	ASSERT (serializable.inheritedIntVal == newSerializable.inheritedIntVal);
}

TEST (InheritedIOTest2)
{
	MyInheritedSerializable2 serializable;
	serializable.intVal = 42;
	serializable.stringVal = "Alma";
	serializable.inheritedIntVal = 55;
	serializable.inheritedIntVal2 = 56;

	MemoryOutputStream outputStream;
	serializable.Write (outputStream);

	MyInheritedSerializable2 newSerializable;
	MemoryInputStream inputStream (outputStream.GetBuffer ());
	newSerializable.Read (inputStream);

	ASSERT (serializable.intVal == newSerializable.intVal);
	ASSERT (serializable.stringVal == newSerializable.stringVal);
	ASSERT (serializable.inheritedIntVal == newSerializable.inheritedIntVal);
	ASSERT (serializable.inheritedIntVal2 == newSerializable.inheritedIntVal2);
}

TEST (DynamicTest)
{
	MyInheritedSerializable serializable;
	serializable.intVal = 42;
	serializable.stringVal = "Alma";
	serializable.inheritedIntVal = 55;

	MemoryOutputStream outputStream;
	WriteDynamicObject (outputStream, &serializable);

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	std::shared_ptr<Serializable> serializableObj (ReadDynamicObject (inputStream));
	
	MyInheritedSerializable* serializable2 = dynamic_cast<MyInheritedSerializable*> (serializableObj.get ());
	ASSERT (serializable2 != nullptr);
	if (serializable2 != nullptr) {
		ASSERT (serializable.intVal == serializable2->intVal);
		ASSERT (serializable.stringVal == serializable2->stringVal);
		ASSERT (serializable.inheritedIntVal == serializable2->inheritedIntVal);
	}
}

TEST (DynamicTest2)
{
	MyInheritedSerializable serializable;
	serializable.intVal = 42;
	serializable.stringVal = "Alma";
	serializable.inheritedIntVal = 55;

	MemoryOutputStream outputStream;
	WriteDynamicObject (outputStream, &serializable);

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	std::shared_ptr<MyInheritedSerializable> serializable2 (ReadDynamicObject<MyInheritedSerializable> (inputStream));

	ASSERT (serializable2 != nullptr);
	ASSERT (serializable.intVal == serializable2->intVal);
	ASSERT (serializable.stringVal == serializable2->stringVal);
	ASSERT (serializable.inheritedIntVal == serializable2->inheritedIntVal);
}

static ValuePtr WriteAndReadValue (const ValuePtr& val)
{
	MemoryOutputStream outputStream;
	WriteDynamicObject (outputStream, val.get ());

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	return ValuePtr (ReadDynamicObject<Value> (inputStream));
}

TEST (ValueTest)
{
	ValuePtr intVal (new IntValue (15));
	ValuePtr newIntVal (WriteAndReadValue (intVal));
	ASSERT (Value::IsType<IntValue> (newIntVal));
	ASSERT (IntValue::Get (intVal) == IntValue::Get (newIntVal));

	ValuePtr doubleVal (new DoubleValue (16.0));
	ValuePtr newDoubleVal (WriteAndReadValue (doubleVal));
	ASSERT (Value::IsType<DoubleValue> (newDoubleVal));
	ASSERT (DoubleValue::Get (doubleVal) == DoubleValue::Get (newDoubleVal));

	ListValuePtr listVal (new ListValue ());
	listVal->Push (intVal);
	listVal->Push (doubleVal);
	ValuePtr newListValValue (WriteAndReadValue (listVal));
	ListValuePtr newListVal = std::dynamic_pointer_cast<ListValue> (newListValValue);
	ASSERT (newListVal != nullptr);

	ASSERT (Value::IsType<IntValue> (newListVal->GetValue (0)));
	ASSERT (IntValue::Get (intVal) == IntValue::Get (newListVal->GetValue (0)));

	ASSERT (Value::IsType<DoubleValue> (newListVal->GetValue (1)));
	ASSERT (DoubleValue::Get (doubleVal) == DoubleValue::Get (newListVal->GetValue (1)));
}

}
