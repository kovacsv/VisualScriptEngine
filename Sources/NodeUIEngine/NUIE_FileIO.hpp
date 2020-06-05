#ifndef NUIE_FILEIO_HPP
#define NUIE_FILEIO_HPP

#include <string>

namespace NUIE
{

bool WriteUtf8File (const std::string& fileName, const std::wstring& content);
bool ReadUtf8File (const std::string& fileName, std::wstring& content);

}

#endif
