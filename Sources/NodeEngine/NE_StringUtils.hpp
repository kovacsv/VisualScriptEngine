#ifndef NE_STRINGUTILS_HPP
#define NE_STRINGUTILS_HPP

#include <string>

namespace NE
{

std::wstring	StringToWString (const std::string& str);
std::string		WStringToString (const std::wstring& str);

}

#endif
