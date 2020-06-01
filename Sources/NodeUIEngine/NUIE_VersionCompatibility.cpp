#include "NUIE_VersionCompatibility.hpp"
#include "NUIE_VersionInfo.hpp"

namespace NUIE
{

const Version& GetCurrentVersion ()
{
	static const Version EngineVersion (VSE_VERSION_1, VSE_VERSION_2, VSE_VERSION_3);
	return EngineVersion;
}

bool IsCompatibleVersion (const Version& version)
{
	return version <= GetCurrentVersion ();
}

}
