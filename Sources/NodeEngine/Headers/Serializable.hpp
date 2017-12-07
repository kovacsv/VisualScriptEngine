#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

#include "Stream.hpp"
#include "Debug.hpp"

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

	bool				operator== (const ObjectVersion& rhs) const;
	bool				operator!= (const ObjectVersion& rhs) const;

private:
	size_t	versionNumber;
};

class Serializable;
typedef Serializable* (*CreatorFunction)();

class SerializationInfo
{
public:
	SerializationInfo (const ObjectId& objectId, const ObjectVersion& objectVersion);
	virtual ~SerializationInfo ();
	
	const ObjectId&			GetObjectId () const;
	const ObjectVersion&	GetObjectVersion () const;

private:
	ObjectId			objectId;
	ObjectVersion		objectVersion;
};

class DynamicSerializationInfo : public SerializationInfo
{
public:
	DynamicSerializationInfo (const ObjectId& objectId, const ObjectVersion& objectVersion, CreatorFunction creatorFunction);
	virtual ~DynamicSerializationInfo ();
	
	Serializable*		CreateInstance () const;

private:
	CreatorFunction		creatorFunction;
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

class Serializable
{
public:
	Serializable ();
	virtual ~Serializable ();

	virtual const SerializationInfo&	GetSerializationInfo () const = 0;
	virtual Stream::Status				Read (InputStream& inputStream) = 0;
	virtual Stream::Status				Write (OutputStream& outputStream) const = 0;
};

Serializable*	CreateDynamicObject (const ObjectId& objectId);
Serializable*	ReadDynamicObject (InputStream& inputStream);
void			WriteDynamicObject (OutputStream& outputStream, const Serializable* object);

template <class ObjectType>
ObjectType* ReadDynamicObject (InputStream& inputStream)
{
	Serializable* obj = ReadDynamicObject (inputStream);
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

#define SERIALIZABLE															\
public:																			\
virtual const NE::SerializationInfo& GetSerializationInfo () const override		\
{																				\
	return serializationInfo;													\
}																				\
static const NE::SerializationInfo& GetStaticSerializationInfo ()				\
{																				\
	return serializationInfo;													\
}																				\
private:																		\
static NE::SerializationInfo serializationInfo;									\
private:																		\

#define DYNAMIC_SERIALIZABLE(ClassName)											\
public:																			\
virtual const NE::SerializationInfo& GetSerializationInfo () const override		\
{																				\
	return serializationInfo;													\
}																				\
static const NE::SerializationInfo& GetStaticSerializationInfo ()				\
{																				\
	return serializationInfo;													\
}																				\
static NE::Serializable* CreateSerializableInstance ()							\
{																				\
	return new ClassName ();													\
}																				\
private:																		\
static NE::DynamicSerializationInfo serializationInfo;							\
private:																		\

#endif
