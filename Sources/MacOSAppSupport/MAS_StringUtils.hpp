#ifndef MAS_STRINGUTILS_HPP
#define MAS_STRINGUTILS_HPP

#include <string>
#include "MAS_IncludeCocoaHeaders.hpp"

namespace MAS
{
	std::wstring s2ws (const std::string& str);
	std::string ws2s (const std::wstring& wstr);
	NSString* StdWStringToNSString (const std::wstring& str);
}

#endif
