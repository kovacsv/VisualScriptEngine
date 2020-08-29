#include "NE_Serializable.hpp"
#include <unordered_map>

namespace NE
{

class ObjectRegistry
{
public:
	ObjectRegistry ()
	{
	
	}
	
	void RegisterSerializationInfo (const DynamicSerializationInfo* serializationInfo)
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

	const DynamicSerializationInfo* GetSerializationInfo (const ObjectId& objectId)
	{
		if (DBGERROR (objectMap.find (objectId) == objectMap.end ())) {
			return nullptr;
		}
		return objectMap[objectId];
	}

private:
	std::unordered_map<ObjectId, const DynamicSerializationInfo*> objectMap;
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
	
bool ObjectVersion::operator< (const ObjectVersion& rhs) const
{
	return versionNumber < rhs.versionNumber;
}

bool ObjectVersion::operator> (const ObjectVersion& rhs) const
{
	return versionNumber > rhs.versionNumber;
}

bool ObjectVersion::operator<= (const ObjectVersion& rhs) const
{
	return versionNumber <= rhs.versionNumber;
}

bool ObjectVersion::operator>= (const ObjectVersion& rhs) const
{
	return versionNumber >= rhs.versionNumber;
}

bool ObjectVersion::operator== (const ObjectVersion& rhs) const
{
	return versionNumber == rhs.versionNumber;
}

bool ObjectVersion::operator!= (const ObjectVersion& rhs) const
{
	return versionNumber != rhs.versionNumber;
}

SerializationInfo::SerializationInfo (const ObjectVersion& objectVersion) :
	objectVersion (objectVersion)
{
	
}

SerializationInfo::~SerializationInfo ()
{

}

const ObjectVersion& SerializationInfo::GetObjectVersion () const
{
	return objectVersion;
}

DynamicSerializationInfo::DynamicSerializationInfo (const ObjectId& objectId, const ObjectVersion& objectVersion, CreatorFunction creatorFunction) :
	SerializationInfo (objectVersion),
	objectId (objectId),
	creatorFunction (creatorFunction)
{
	GetObjectRegistry ().RegisterSerializationInfo (this);
}

DynamicSerializationInfo::~DynamicSerializationInfo ()
{

}

const ObjectId& DynamicSerializationInfo::GetObjectId () const
{
	return objectId;
}

DynamicSerializable* DynamicSerializationInfo::CreateInstance () const
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

DynamicSerializable::DynamicSerializable ()
{

}

DynamicSerializable::~DynamicSerializable ()
{

}

DynamicSerializable* CreateDynamicObject (const ObjectId& objectId)
{
	const DynamicSerializationInfo* dynamicSerializationInfo = GetObjectRegistry ().GetSerializationInfo (objectId);
	if (DBGERROR (dynamicSerializationInfo == nullptr)) {
		return nullptr;
	}
	return dynamicSerializationInfo->CreateInstance ();
}

DynamicSerializable* ReadDynamicObject (InputStream& inputStream)
{
	ObjectId objectId;
	objectId.Read (inputStream);
	if (inputStream.GetStatus () != Stream::Status::NoError) {
		return nullptr;
	}
	DynamicSerializable* serializable = CreateDynamicObject (objectId);
	if (DBGERROR (serializable == nullptr)) {
		return nullptr;
	}
	serializable->Read (inputStream);
	return serializable;
}

bool WriteDynamicObject (OutputStream& outputStream, const DynamicSerializable* object)
{
	if (DBGERROR (object == nullptr)) {
		return false;
	}
	const DynamicSerializationInfo* serializationInfo = object->GetDynamicSerializationInfo ();
	if (DBGERROR (serializationInfo == nullptr)) {
		return false;
	}
	Stream::Status status = serializationInfo->GetObjectId ().Write (outputStream);
	if (DBGERROR (status != Stream::Status::NoError)) {
		return false;
	}
	status = object->Write (outputStream);
	if (DBGERROR (status != Stream::Status::NoError)) {
		return false;
	}
	return true;
}

}
