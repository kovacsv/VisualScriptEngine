#include "NE_UniqueIdGenerator.hpp"

namespace NE
{

SERIALIZATION_INFO (UniqueIdGenerator, 1);

UniqueIdGenerator::UniqueIdGenerator () :
	nextId (1)
{

}

UniqueIdGenerator::~UniqueIdGenerator ()
{

}

IdType UniqueIdGenerator::GenerateUniqueId ()
{
	IdType newId = nextId++;
	return newId;
}

Stream::Status UniqueIdGenerator::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	IdType nextIdFromStream = 0;
	inputStream.Read (nextIdFromStream);
	nextId = nextIdFromStream;
	return inputStream.GetStatus ();
}

Stream::Status UniqueIdGenerator::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (nextId);
	return outputStream.GetStatus ();
}

}
