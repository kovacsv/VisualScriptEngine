#ifndef NE_STRINGUTILS_HPP
#define NE_STRINGUTILS_HPP

#include <string>

namespace NE
{

std::wstring	StringToWString (const std::string& str);
std::string		WStringToString (const std::wstring& str);

std::wstring	ReplaceAll (const std::wstring& string, const std::wstring& fromString, const std::wstring& toString);
std::wstring	ReplaceAll (const std::wstring& string, const std::wstring& fromString, const std::initializer_list<std::wstring>& toStrings);

}

#endif
