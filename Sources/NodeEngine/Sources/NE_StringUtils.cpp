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
			string[decimalPos] = decimalSeparator;
		}
	}

	return string;
}

}
