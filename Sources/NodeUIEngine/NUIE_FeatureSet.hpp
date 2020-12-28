#ifndef NUIE_FEATURESET_HPP
#define NUIE_FEATURESET_HPP

#include <string>

namespace NUIE
{

void	EnableFeature (const std::string& featureId, bool enabled);
bool	IsFeatureEnabled (const std::string& featureId);

}

#endif
