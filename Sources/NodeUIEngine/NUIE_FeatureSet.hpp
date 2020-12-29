#ifndef NUIE_FEATURESET_HPP
#define NUIE_FEATURESET_HPP

#include <string>

namespace NUIE
{

enum class Feature
{
	ConnectionDisplay
};

class FeatureEnableGuard
{
public:
	FeatureEnableGuard (Feature featureId, bool enabled);
	~FeatureEnableGuard ();

private:
	Feature		featureId;
	bool		oldEnabled;
};

void	EnableFeature (Feature featureId, bool enabled);
bool	IsFeatureEnabled (Feature featureId);

}

#endif
