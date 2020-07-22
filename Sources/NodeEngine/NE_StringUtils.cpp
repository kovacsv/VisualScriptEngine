#include "NE_StringUtils.hpp"

#include <locale>
#include <codecvt>

namespace NE
{

std::wstring StringToWString (const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
	return std::wstring (convert.from_bytes (str));
}

std::string WStringToString (const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
	return std::string (convert.to_bytes (str));
}

}
