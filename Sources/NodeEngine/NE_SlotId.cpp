#include "NE_SlotId.hpp"

namespace NE
{

SERIALIZATION_INFO (SlotId, 1);

SlotId::SlotId () :
	id ()
{

}

SlotId::SlotId (const std::string& id) :
	id (id)
{

}

SlotId::~SlotId ()
{

}

size_t SlotId::GenerateHashValue () const
{
	return std::hash<std::string> {} (id);
}

bool SlotId::operator< (const SlotId& rhs) const
{
	return id < rhs.id;
}

bool SlotId::operator> (const SlotId& rhs) const
{
	return id > rhs.id;
}

bool SlotId::operator== (const SlotId& rhs) const
{
	return id == rhs.id;
}

bool SlotId::operator!= (const SlotId& rhs) const
{
	return !operator== (rhs);
}

Stream::Status SlotId::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	inputStream.Read (id);
	return inputStream.GetStatus ();
}

Stream::Status SlotId::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (id);
	return outputStream.GetStatus ();
}

const SlotId NullSlotId;

}
