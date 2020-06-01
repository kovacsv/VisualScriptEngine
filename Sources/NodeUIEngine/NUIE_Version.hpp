#ifndef NUIE_VERSION_HPP
#define NUIE_VERSION_HPP

#include "NE_Stream.hpp"
#include <array>

namespace NUIE
{

class Version
{
public:
	Version ();
	Version (int version1, int version2, int version3);

	bool				operator== (const Version& rhs) const;
	bool				operator!= (const Version& rhs) const;

	bool				operator< (const Version& rhs) const;
	bool				operator> (const Version& rhs) const;

	bool				operator<= (const Version& rhs) const;
	bool				operator>= (const Version& rhs) const;

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

private:
	std::array<int, 3>	versions;
};

}

#endif
