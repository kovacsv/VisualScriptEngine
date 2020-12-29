#include "NUIE_FeatureSet.hpp"

#include <unordered_map>
#include <functional>

namespace std
{
	template <>
	struct hash<NUIE::Feature>
	{
		size_t operator() (const NUIE::Feature& featureId) const noexcept
		{
			return (size_t) featureId;
		}
	};
}

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

static std::unordered_map<Feature, bool> features;

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
