#include "NE_StringUtils.hpp"
#include "NE_Debug.hpp"

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

std::wstring ReplaceAll (const std::wstring& string, const std::wstring& fromString, const std::wstring& toString)
{
	std::wstring result = string;
	size_t searchPos = string.find (fromString);
	while (searchPos != std::wstring::npos) {
		result.replace (searchPos, fromString.length (), toString);
		searchPos = result.find (fromString, searchPos + toString.length ());
	}
	return result;
}

std::wstring ReplaceAll (const std::wstring& string, const std::wstring& fromString, const std::initializer_list<std::wstring>& toStrings)
{
	std::wstring result = string;
	auto toStringIt = toStrings.begin ();
	size_t searchPos = string.find (fromString);
	while (searchPos != std::wstring::npos && toStringIt != toStrings.end ()) {
		const std::wstring& currStr = *toStringIt;
		result.replace (searchPos, fromString.length (), currStr);
		searchPos = result.find (fromString, searchPos + currStr.length ());
		++toStringIt;
	}
	DBGASSERT (searchPos == std::wstring::npos && toStringIt == toStrings.end ());
	return result;
}

}
