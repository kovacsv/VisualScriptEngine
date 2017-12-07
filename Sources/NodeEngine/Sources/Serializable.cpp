#include "Serializable.hpp"
#include <unordered_map>

namespace NE
{

class ObjectRegistry
{
public:
	ObjectRegistry ()
	{
	
	}
	
	void RegisterSerializationInfo (const SerializationInfo* serializationInfo)
	{
		if (DBGERROR (serializationInfo == nullptr)) {
			return;
		}
		const ObjectId& objectId = serializationInfo->GetObjectId ();
		if (DBGERROR (objectMap.find (objectId) != objectMap.end ())) {
			return;
		}
		objectMap.insert ({ objectId, serializationInfo });
	}

	const SerializationInfo* GetSerializationInfo (const ObjectId& objectId)
	{
		if (DBGERROR (objectMap.find (objectId) == objectMap.end ())) {
			return nullptr;
		}
		return objectMap[objectId];
	}

private:
	std::unordered_map<ObjectId, const SerializationInfo*> objectMap;
};

static ObjectRegistry& GetObjectRegistry ()
{
	static ObjectRegistry objectRegistry;
	return objectRegistry;
}


ObjectId::ObjectId () :
	id ("")
{

}

ObjectId::ObjectId (const std::string& id) :
	id (id)
{

}

ObjectId::~ObjectId ()
{

}

size_t ObjectId::GenerateHashValue () const
{
	return std::hash<std::string> {} (id);
}

Stream::Status ObjectId::Read (InputStream& inputStream)
{
	inputStream.Read (id);
	return inputStream.GetStatus ();
}

Stream::Status ObjectId::Write (OutputStream& outputStream) const
{
	outputStream.Write (id);
	return outputStream.GetStatus ();
}

bool ObjectId::operator== (const ObjectId& rhs) const
{
	return id == rhs.id;
}

bool ObjectId::operator!= (const ObjectId& rhs) const
{
	return !operator== (rhs);
}

ObjectVersion::ObjectVersion () :
	versionNumber (0)
{

}

ObjectVersion::ObjectVersion (size_t versionNumber) :
	versionNumber (versionNumber)
{

}

ObjectVersion::~ObjectVersion ()
{

}

size_t ObjectVersion::GetVersionNumber () const
{
	return versionNumber;
}

Stream::Status ObjectVersion::Read (InputStream& inputStream)
{
	inputStream.Read (versionNumber);
	return inputStream.GetStatus ();
}

Stream::Status ObjectVersion::Write (OutputStream& outputStream) const
{
	outputStream.Write (versionNumber);
	return outputStream.GetStatus ();
}
	
bool ObjectVersion::operator== (const ObjectVersion& rhs) const
{
	return versionNumber == rhs.versionNumber;
}

bool ObjectVersion::operator!= (const ObjectVersion& rhs) const
{
	return !operator== (rhs);
}

SerializationInfo::SerializationInfo (const ObjectId& objectId, const ObjectVersion& objectVersion) :
	objectId (objectId),
	objectVersion (objectVersion)
{
	GetObjectRegistry ().RegisterSerializationInfo (this);
}

SerializationInfo::~SerializationInfo ()
{

}

const ObjectId& SerializationInfo::GetObjectId () const
{
	return objectId;
}

const ObjectVersion& SerializationInfo::GetObjectVersion () const
{
	return objectVersion;
}

DynamicSerializationInfo::DynamicSerializationInfo (const ObjectId& objectId, const ObjectVersion& objectVersion, CreatorFunction creatorFunction) :
	SerializationInfo (objectId, objectVersion),
	creatorFunction (creatorFunction)
{
	
}

DynamicSerializationInfo::~DynamicSerializationInfo ()
{

}

Serializable* DynamicSerializationInfo::CreateInstance () const
{
	if (DBGERROR (creatorFunction == nullptr)) {
		return nullptr;
	}
	return creatorFunction ();
}

ObjectHeader::ObjectHeader (InputStream& inputStream)
{
	version.Read (inputStream);
}

ObjectHeader::ObjectHeader (OutputStream& outputStream, const SerializationInfo& serializationInfo) :
	version (serializationInfo.GetObjectVersion ())
{
	version.Write (outputStream);
}

const ObjectVersion& ObjectHeader::GetVersion () const
{
	return version;
}

Serializable::Serializable ()
{

}

Serializable::~Serializable ()
{

}

Serializable* CreateDynamicObject (const ObjectId& objectId)
{
	const SerializationInfo* serializationInfo = GetObjectRegistry ().GetSerializationInfo (objectId);
	const DynamicSerializationInfo* dynamicSerializationInfo = dynamic_cast<const DynamicSerializationInfo*> (serializationInfo);
	if (DBGERROR (dynamicSerializationInfo == nullptr)) {
		return nullptr;
	}
	return dynamicSerializationInfo->CreateInstance ();
}

Serializable* ReadDynamicObject (InputStream& inputStream)
{
	ObjectId objectId;
	objectId.Read (inputStream);
	if (inputStream.GetStatus () != Stream::Status::NoError) {
		return nullptr;
	}
	Serializable* serializable = CreateDynamicObject (objectId);
	if (DBGERROR (serializable == nullptr)) {
		return nullptr;
	}
	serializable->Read (inputStream);
	return serializable;
}

void WriteDynamicObject (OutputStream& outputStream, const Serializable* object)
{
	object->GetSerializationInfo ().GetObjectId ().Write (outputStream);
	object->Write (outputStream);
}

}
