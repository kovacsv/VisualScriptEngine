#ifndef NUIE_EXTERNALFILEIO_HPP
#define NUIE_EXTERNALFILEIO_HPP

#include <vector>
#include <string>

namespace NUIE
{

class ExternalFileIO
{
public:
	ExternalFileIO ();
	virtual ~ExternalFileIO ();

	virtual bool	ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const = 0;
	virtual bool	WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer) const = 0;
};

}

#endif
