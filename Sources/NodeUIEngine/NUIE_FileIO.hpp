#ifndef NUIE_FILEIO_HPP
#define NUIE_FILEIO_HPP

#include <vector>
#include <string>

namespace NUIE
{

bool	ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer);
bool	WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer);

bool	ReadUtf8File (const std::wstring& fileName, std::wstring& content);
bool	WriteUtf8File (const std::wstring& fileName, const std::wstring& content);

}

#endif
