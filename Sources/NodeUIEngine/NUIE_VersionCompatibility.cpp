#include "NUIE_VersionCompatibility.hpp"
#include "NUIE_VersionInfo.hpp"

namespace NUIE
{

static const Version& GetFirstCompatibleEngineVersion ()
{
	static const Version FirstCompatibleVersion (0, 3, 2);
	return FirstCompatibleVersion;
}

const Version& GetCurrentEngineVersion ()
{
	static const Version EngineVersion (VSE_VERSION_1, VSE_VERSION_2, VSE_VERSION_3);
	return EngineVersion;
}

bool IsCompatibleEngineVersion (const Version& version)
{
	if (version < GetFirstCompatibleEngineVersion ()) {
		return false;
	} else if (version > GetCurrentEngineVersion ()) {
		return false;
	}
	return true;
}

}
