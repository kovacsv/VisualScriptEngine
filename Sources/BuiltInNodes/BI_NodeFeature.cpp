#include "BI_NodeFeatureSet.hpp"

namespace BI
{

SERIALIZATION_INFO (FeatureId, 1);
SERIALIZATION_INFO (NodeFeature, 1);

FeatureId::FeatureId () :
	id ()
{

}

FeatureId::FeatureId (const std::string& id) :
	id (id)
{

}

FeatureId::~FeatureId ()
{

}

size_t FeatureId::GenerateHashValue () const
{
	return std::hash<std::string> {} (id);
}

bool FeatureId::operator< (const FeatureId& rhs) const
{
	return id < rhs.id;
}

bool FeatureId::operator> (const FeatureId& rhs) const
{
	return id > rhs.id;
}

bool FeatureId::operator== (const FeatureId& rhs) const
{
	return id == rhs.id;
}

bool FeatureId::operator!= (const FeatureId& rhs) const
{
	return !operator== (rhs);
}

NE::Stream::Status FeatureId::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	inputStream.Read (id);
	return inputStream.GetStatus ();
}

NE::Stream::Status FeatureId::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (id);
	return outputStream.GetStatus ();
}

NodeFeature::NodeFeature () :
	NodeFeature (FeatureId ())
{

}

NodeFeature::NodeFeature (const FeatureId& featureId) :
	featureId (featureId)
{
}

NodeFeature::~NodeFeature ()
{

}

const FeatureId& NodeFeature::GetId () const
{
	return featureId;
}

NE::Stream::Status NodeFeature::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NodeFeature::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	return outputStream.GetStatus ();
}

}
