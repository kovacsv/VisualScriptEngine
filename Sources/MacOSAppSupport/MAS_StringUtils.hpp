#ifndef MAS_STRINGUTILS_HPP
#define MAS_STRINGUTILS_HPP

#include <string>
#include "MAS_IncludeCocoaHeaders.hpp"

namespace MAS
{
	NSString* StdWStringToNSString (const std::wstring& str);
}

#endif
