#include "NUIE_Version.hpp"

namespace NUIE
{

Version::Version () :
	Version (0, 0, 0)
{

}

Version::Version (int version1, int version2, int version3) :
	versions { { version1, version2, version3 } }
{
}

bool Version::operator== (const Version& rhs) const
{
	return versions == rhs.versions;
}

bool Version::operator!= (const Version& rhs) const
{
	return !operator== (rhs);
}

bool Version::operator< (const Version& rhs) const
{
	for (size_t i = 0; i < versions.size (); i++) {
		if (versions[i] < rhs.versions[i]) {
			return true;
		} else if (versions[i] > rhs.versions[i]) {
			return false;
		}
	}
	return false;
}

bool Version::operator> (const Version& rhs) const
{
	for (size_t i = 0; i < versions.size (); i++) {
		if (versions[i] > rhs.versions[i]) {
			return true;
		} else if (versions[i] < rhs.versions[i]) {
			return false;
		}
	}
	return false;
}

bool Version::operator<= (const Version& rhs) const
{
	return operator< (rhs) || operator== (rhs);
}

bool Version::operator>= (const Version& rhs) const
{
	return operator> (rhs) || operator== (rhs);
}

NE::Stream::Status Version::Read (NE::InputStream& inputStream)
{
	inputStream.Read (versions[0]);
	inputStream.Read (versions[1]);
	inputStream.Read (versions[2]);
	return inputStream.GetStatus ();
}

NE::Stream::Status Version::Write (NE::OutputStream& outputStream) const
{
	outputStream.Write (versions[0]);
	outputStream.Write (versions[1]);
	outputStream.Write (versions[2]);
	return outputStream.GetStatus ();
}

}
