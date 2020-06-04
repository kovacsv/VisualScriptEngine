#include "NUIE_VersionCompatibility.hpp"
#include "NUIE_VersionInfo.hpp"

namespace NUIE
{

static const Version& GetFirstCompatibleVersion ()
{
	static const Version FirstCompatibleVersion (0, 3, 2);
	return FirstCompatibleVersion;
}

const Version& GetCurrentVersion ()
{
	static const Version EngineVersion (VSE_VERSION_1, VSE_VERSION_2, VSE_VERSION_3);
	return EngineVersion;
}

bool IsCompatibleVersion (const Version& version)
{
	if (version < GetFirstCompatibleVersion ()) {
		return false;
	} else if (version > GetCurrentVersion ()) {
		return false;
	}
	return true;
}

}
