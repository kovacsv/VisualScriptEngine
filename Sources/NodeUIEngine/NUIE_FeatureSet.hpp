#ifndef NUIE_FEATURESET_HPP
#define NUIE_FEATURESET_HPP

#include <string>

namespace NUIE
{

class FeatureEnableGuard
{
public:
	FeatureEnableGuard (const std::string& featureId, bool enabled);
	~FeatureEnableGuard ();

private:
	std::string		featureId;
	bool			oldEnabled;
};

void	EnableFeature (const std::string& featureId, bool enabled);
bool	IsFeatureEnabled (const std::string& featureId);

}

#endif
