#include "MAS_StringUtils.hpp"

#include <locale>
#include <codecvt>

namespace MAS
{

std::wstring s2ws (const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes (str);
}

std::string ws2s (const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes (wstr);
}

NSString* StdWStringToNSString (const std::wstring& str)
{
	return [[[NSString alloc] initWithBytes : str.data () length : str.length() * sizeof (wchar_t) encoding : NSUTF32LittleEndianStringEncoding] autorelease];
}

}
