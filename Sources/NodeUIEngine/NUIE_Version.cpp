#include "NUIE_Version.hpp"
#include "NUIE_VersionInfo.hpp"

namespace NUIE
{

Version::Version () :
	version1 (0),
	version2 (0),
	version3 (0)
{

}

Version::Version (int version1, int version2, int version3) :
	version1 (version1),
	version2 (version2),
	version3 (version3)
{

}

NE::Stream::Status Version::Read (NE::InputStream& inputStream)
{
	inputStream.Read (version1);
	inputStream.Read (version2);
	inputStream.Read (version3);
	return inputStream.GetStatus ();
}

NE::Stream::Status Version::Write (NE::OutputStream& outputStream) const
{
	outputStream.Write (version1);
	outputStream.Write (version2);
	outputStream.Write (version3);
	return outputStream.GetStatus ();
}

const Version currentVersion (VSE_VERSION_1, VSE_VERSION_2, VSE_VERSION_3);

}
