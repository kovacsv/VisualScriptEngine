#ifndef NE_SERIALIZABLE_HPP
#define NE_SERIALIZABLE_HPP

#include "NE_Stream.hpp"
#include "NE_Debug.hpp"

namespace NE
{

class ObjectId
{
public:
	ObjectId ();
	ObjectId (const std::string& id);
	~ObjectId ();

	size_t				GenerateHashValue () const;

	Stream::Status		Read (InputStream& inputStream);
	Stream::Status		Write (OutputStream& outputStream) const;

	bool				operator== (const ObjectId& rhs) const;
	bool				operator!= (const ObjectId& rhs) const;

private:
	std::string id;
};

class ObjectVersion
{
public:
	ObjectVersion ();
	ObjectVersion (size_t versionNumber);
	~ObjectVersion ();

	size_t				GetVersionNumber () const;

	Stream::Status		Read (InputStream& inputStream);
	Stream::Status		Write (OutputStream& outputStream) const;

	bool				operator< (const ObjectVersion& rhs) const;
	bool				operator> (const ObjectVersion& rhs) const;

	bool				operator<= (const ObjectVersion& rhs) const;
	bool				operator>= (const ObjectVersion& rhs) const;

	bool				operator== (const ObjectVersion& rhs) const;
	bool				operator!= (const ObjectVersion& rhs) const;

private:
	size_t				versionNumber;
};

class SerializationInfo
{
public:
	SerializationInfo (const ObjectVersion& objectVersion);
	virtual ~SerializationInfo ();
	
	const ObjectVersion&	GetObjectVersion () const;

private:
	ObjectVersion		objectVersion;
};

class DynamicSerializable;
using CreatorFunction = DynamicSerializable* (*)();

class DynamicSerializationInfo : public SerializationInfo
{
public:
	DynamicSerializationInfo (const ObjectId& objectId, const ObjectVersion& objectVersion, CreatorFunction creatorFunction);
	virtual ~DynamicSerializationInfo ();
	
	const ObjectId&			GetObjectId () const;
	DynamicSerializable*	CreateInstance () const;

private:
	ObjectId				objectId;
	CreatorFunction			creatorFunction;
};

class ObjectHeader
{
public:
	ObjectHeader (InputStream& inputStream);
	ObjectHeader (OutputStream& outputStream, const SerializationInfo& serializationInfo);

	const ObjectVersion& GetVersion () const;

private:
	ObjectVersion version;
};

class DynamicSerializable
{
public:
	DynamicSerializable ();
	virtual ~DynamicSerializable ();

	virtual const DynamicSerializationInfo*		GetDynamicSerializationInfo () const = 0;
	virtual Stream::Status						Read (InputStream& inputStream) = 0;
	virtual Stream::Status						Write (OutputStream& outputStream) const = 0;
};

DynamicSerializable*	CreateDynamicObject (const ObjectId& objectId);
DynamicSerializable*	ReadDynamicObject (InputStream& inputStream);
bool					WriteDynamicObject (OutputStream& outputStream, const DynamicSerializable* object);

template <class ObjectType>
ObjectType* ReadDynamicObject (InputStream& inputStream)
{
	DynamicSerializable* obj = ReadDynamicObject (inputStream);
	ObjectType* typedObj = dynamic_cast<ObjectType*> (obj);
	if (DBGERROR (typedObj == nullptr)) {
		delete obj;
		return nullptr;
	}
	return typedObj;
}

}

namespace std
{
	template <>
	struct hash<NE::ObjectId>
	{
		size_t operator() (const NE::ObjectId& id) const noexcept
		{
			return id.GenerateHashValue ();
		}
	};
}

#define SERIALIZABLE																		\
private:																					\
static NE::SerializationInfo serializationInfo;												\
private:																					\

#define DYNAMIC_SERIALIZABLE(ClassName)														\
public:																						\
virtual const NE::DynamicSerializationInfo* GetDynamicSerializationInfo () const override	\
{																							\
	return &serializationInfo;																\
}																							\
static NE::DynamicSerializable* CreateSerializableInstance ()								\
{																							\
	return new ClassName ();																\
}																							\
private:																					\
static NE::DynamicSerializationInfo serializationInfo;										\
private:																					\

#define SERIALIZATION_INFO(ClassName,ObjectVersionNumber) \
NE::SerializationInfo ClassName::serializationInfo (NE::ObjectVersion (ObjectVersionNumber))

#define DYNAMIC_SERIALIZATION_INFO(ClassName,ObjectVersionNumber,ObjectIdString) \
NE::DynamicSerializationInfo ClassName::serializationInfo (NE::ObjectId (ObjectIdString), NE::ObjectVersion (ObjectVersionNumber), ClassName::CreateSerializableInstance)

#endif
