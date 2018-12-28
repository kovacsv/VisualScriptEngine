#ifndef NUIE_VERSION_HPP
#define NUIE_VERSION_HPP

#include "NE_Stream.hpp"

namespace NUIE
{

class Version
{
public:
	Version ();
	Version (int version1, int version2, int version3);

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

private:
	int version1;
	int version2;
	int version3;
};

extern const Version currentVersion;

}

#endif
