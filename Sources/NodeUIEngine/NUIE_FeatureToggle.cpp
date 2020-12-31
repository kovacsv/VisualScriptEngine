#include "NUIE_FeatureToggle.hpp"
#include "NUIE_HashUtils.hpp"

#include <unordered_map>
#include <functional>

namespace NUIE
{

FeatureEnableGuard::FeatureEnableGuard (Feature featureId, bool enabled) :
	featureId (featureId),
	oldEnabled (IsFeatureEnabled (featureId))
{
	EnableFeature (featureId, enabled);
}

FeatureEnableGuard::~FeatureEnableGuard ()
{
	EnableFeature (featureId, oldEnabled);
}

static std::unordered_map<Feature, bool, EnumHash> features;

void EnableFeature (Feature featureId, bool enabled)
{
	auto found = features.find (featureId);
	if (found == features.end ()) {
		features.insert ({ featureId, enabled });
		return;
	}
	found->second = enabled;
}

bool IsFeatureEnabled (Feature featureId)
{
	auto found = features.find (featureId);
	if (found == features.end ()) {
		return false;
	}
	return found->second;
}

}
