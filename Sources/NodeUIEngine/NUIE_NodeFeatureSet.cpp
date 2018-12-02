#include "NUIE_NodeFeatureSet.hpp"

namespace NUIE
{

NE::SerializationInfo FeatureId::serializationInfo (NE::ObjectVersion (1));
NE::SerializationInfo NodeFeature::serializationInfo (NE::ObjectVersion (1));
NE::SerializationInfo UINodeFeatureSet::serializationInfo (NE::ObjectVersion (1));

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

UINodeFeatureSet::UINodeFeatureSet ()
{

}

UINodeFeatureSet::~UINodeFeatureSet ()
{

}

void UINodeFeatureSet::AddFeature (const FeatureId& featureId, const NodeFeaturePtr& feature)
{
	size_t index = features.size ();
	features.push_back (feature);
	idToIndex.insert ({ featureId, index });
}

bool UINodeFeatureSet::HasFeature (const FeatureId& featureId) const
{
	return idToIndex.find (featureId) != idToIndex.end ();
}

const NodeFeaturePtr& UINodeFeatureSet::GetFeature (const FeatureId& featureId) const
{
	return features[idToIndex.at (featureId)];
}

void UINodeFeatureSet::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	for (const NodeFeaturePtr& feature : features) {
		feature->RegisterCommands (commandRegistrator);
	}
}

void UINodeFeatureSet::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	for (const NodeFeaturePtr& feature : features) {
		feature->RegisterParameters (parameterList);
	}
}

NE::Stream::Status UINodeFeatureSet::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	size_t featureCount = 0;
	inputStream.Read (featureCount);
	for (size_t i = 0; i < featureCount; i++) {
		NodeFeaturePtr feature (NE::ReadDynamicObject<NodeFeature> (inputStream));
		AddFeature (feature->GetId (), feature);
	}
	return inputStream.GetStatus ();
}

NE::Stream::Status UINodeFeatureSet::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (features.size ());
	for (const NodeFeaturePtr& feature : features) {
		NE::WriteDynamicObject (outputStream, feature.get ());
	}
	return outputStream.GetStatus ();
}

}
