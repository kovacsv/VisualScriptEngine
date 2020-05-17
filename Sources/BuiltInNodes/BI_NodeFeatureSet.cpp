#include "BI_NodeFeatureSet.hpp"

namespace BI
{

SERIALIZATION_INFO (NodeFeatureSet, 1);

NodeFeatureSet::NodeFeatureSet ()
{

}

NodeFeatureSet::~NodeFeatureSet ()
{

}

void NodeFeatureSet::AddFeature (const FeatureId& featureId, const NodeFeaturePtr& feature)
{
	size_t index = features.size ();
	features.push_back (feature);
	idToIndex.insert ({ featureId, index });
}

bool NodeFeatureSet::HasFeature (const FeatureId& featureId) const
{
	return idToIndex.find (featureId) != idToIndex.end ();
}

const NodeFeaturePtr& NodeFeatureSet::GetFeature (const FeatureId& featureId) const
{
	return features[idToIndex.at (featureId)];
}

void NodeFeatureSet::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	for (const NodeFeaturePtr& feature : features) {
		feature->RegisterCommands (commandRegistrator);
	}
}

void NodeFeatureSet::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	for (const NodeFeaturePtr& feature : features) {
		feature->RegisterParameters (parameterList);
	}
}

NE::Stream::Status NodeFeatureSet::Read (NE::InputStream& inputStream)
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

NE::Stream::Status NodeFeatureSet::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (features.size ());
	for (const NodeFeaturePtr& feature : features) {
		NE::WriteDynamicObject (outputStream, feature.get ());
	}
	return outputStream.GetStatus ();
}

}
