#include "MAS_StringUtils.hpp"

#include <locale>
#include <codecvt>

namespace MAS
{

NSString* StdWStringToNSString (const std::wstring& str)
{
	return [[[NSString alloc] initWithBytes : str.data () length : str.length() * sizeof (wchar_t) encoding : NSUTF32LittleEndianStringEncoding] autorelease];
}

}
