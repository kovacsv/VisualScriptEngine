#include "NUIE_IconId.hpp"

namespace NUIE
{

SERIALIZATION_INFO (IconId, 1);

IconId::IconId (const IconIdType& id) :
	id (id)
{

}

IconId::~IconId ()
{

}

IconIdType IconId::GetId () const
{
	return id;
}

size_t IconId::GenerateHashValue () const
{
	return std::hash<IconIdType> {} (id);
}

bool IconId::operator== (const IconId& rhs) const
{
	return id == rhs.id;
}

bool IconId::operator!= (const IconId& rhs) const
{
	return !operator== (rhs);
}

NE::Stream::Status IconId::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	inputStream.Read (id);
	return inputStream.GetStatus ();
}

NE::Stream::Status IconId::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (id);
	return outputStream.GetStatus ();
}

const IconId InvalidIconId (-1);

}
