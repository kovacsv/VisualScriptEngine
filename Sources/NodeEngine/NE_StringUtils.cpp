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

std::wstring ReplaceAll (const std::wstring& string, const std::wstring& from, const std::wstring& to)
{
	std::wstring result = string;
	size_t searchPos = string.find (from);
	while (searchPos != std::wstring::npos) {
		result.replace (searchPos, from.length (), to);
		searchPos = result.find (from, searchPos + to.length ());
	}
	return result;
}

}
