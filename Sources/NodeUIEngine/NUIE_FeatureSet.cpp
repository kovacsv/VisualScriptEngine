#include "NUIE_FeatureSet.hpp"

#include <unordered_map>

namespace NUIE
{

FeatureEnableGuard::FeatureEnableGuard (const std::string& featureId, bool enabled) :
	featureId (featureId),
	oldEnabled (IsFeatureEnabled (featureId))
{
	EnableFeature (featureId, enabled);
}

FeatureEnableGuard::~FeatureEnableGuard ()
{
	EnableFeature (featureId, oldEnabled);
}

static std::unordered_map<std::string, bool> features;

void EnableFeature (const std::string& featureId, bool enabled)
{
	auto found = features.find (featureId);
	if (found == features.end ()) {
		features.insert ({ featureId, enabled });
		return;
	}
	found->second = enabled;
}

bool IsFeatureEnabled (const std::string& featureId)
{
	auto found = features.find (featureId);
	if (found == features.end ()) {
		return false;
	}
	return found->second;
}

}
