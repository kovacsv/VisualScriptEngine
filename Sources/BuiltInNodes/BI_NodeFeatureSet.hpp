#ifndef BI_NODEFEATURESET_HPP
#define BI_NODEFEATURESET_HPP

#include "BI_NodeFeature.hpp"

#include <vector>
#include <unordered_map>

namespace BI
{

class NodeFeatureSet
{
	SERIALIZABLE;

public:
	NodeFeatureSet ();
	~NodeFeatureSet ();

	void						AddFeature (const FeatureId& featureId, const NodeFeaturePtr& feature);
	bool						HasFeature (const FeatureId& featureId) const;
	const NodeFeaturePtr&		GetFeature (const FeatureId& featureId) const;

	void						RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const;
	void						RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	NE::Stream::Status			Read (NE::InputStream& inputStream);
	NE::Stream::Status			Write (NE::OutputStream& outputStream) const;

private:
	std::vector<NodeFeaturePtr>				features;
	std::unordered_map<FeatureId, size_t>	idToIndex;
};

}

#endif
