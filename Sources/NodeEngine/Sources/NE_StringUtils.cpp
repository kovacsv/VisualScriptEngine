#include "NE_StringUtils.hpp"

#include <sstream>
#include <iomanip>

namespace NE
{

std::wstring DoubleToString (double val, const StringSettings& settings)
{
	std::wostringstream stream;
	int precision = settings.GetPrecision ();
    stream << std::fixed << std::setprecision (precision) << val;
	std::wstring string = stream.str ();

	wchar_t decimalSeparator = settings.GetDecimalSeparator ();
	if (decimalSeparator != L'.') {
		size_t decimalPos = string.find (L'.');
		if (decimalPos != std::wstring::npos) {
			wchar_t decimalSeparatorStr[2] = {};
			decimalSeparatorStr[0] = decimalSeparator;
			decimalSeparatorStr[1] = L'\0';
			string.replace (decimalPos, decimalPos, decimalSeparatorStr);
		}
	}

	return string;
}

}
