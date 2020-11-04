#ifndef NUIE_VERSIONCOMPATIBILITY_HPP
#define NUIE_VERSIONCOMPATIBILITY_HPP

#include "NUIE_Version.hpp"

namespace NUIE
{

const Version&	GetCurrentEngineVersion ();
bool			IsCompatibleEngineVersion (const Version& version);

}

#endif
