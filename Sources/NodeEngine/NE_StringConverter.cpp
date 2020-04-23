#include "NE_StringConverter.hpp"

#include <sstream>
#include <iomanip>

namespace NE
{

StringSettings::StringSettings ()
{

}

StringSettings::~StringSettings ()
{

}

BasicStringSettings::BasicStringSettings (wchar_t decimalSeparator, wchar_t listSeparator, int precision) :
	decimalSeparator (decimalSeparator),
	listSeparator (listSeparator),
	precision (precision)
{

}

BasicStringSettings::~BasicStringSettings ()
{

}

wchar_t BasicStringSettings::GetDecimalSeparator () const
{
	return decimalSeparator;
}

wchar_t BasicStringSettings::GetListSeparator () const
{
	return listSeparator;
}

int BasicStringSettings::GetPrecision () const
{
	return precision;
}

void BasicStringSettings::SetDecimalSeparator (wchar_t newDecimalSeparator)
{
	decimalSeparator = newDecimalSeparator;
}

void BasicStringSettings::SetListSeparator (wchar_t newListSeparator)
{
	listSeparator = newListSeparator;
}

void BasicStringSettings::SetPrecision (int newPrecision)
{
	precision = newPrecision;
}

StringConverter::ListEnumerator::ListEnumerator ()
{

}

StringConverter::ListEnumerator::~ListEnumerator ()
{

}

StringConverter::StringConverter ()
{

}

StringConverter::~StringConverter ()
{

}

BasicStringConverter::BasicStringConverter (const BasicStringSettings& settings) :
	StringConverter (),
	settings (settings)
{

}

BasicStringConverter::~BasicStringConverter ()
{

}

std::wstring BasicStringConverter::IntegerToString (int val) const
{
	return std::to_wstring (val);
}

std::wstring BasicStringConverter::NumberToString (double val, Measure) const
{
	return ConvertNumberToString (val, settings.GetPrecision (), settings.GetDecimalSeparator ());
}

std::wstring BasicStringConverter::ListToString (const ListEnumerator& enumerator) const
{
	return ConvertListToString (enumerator, settings.GetListSeparator ());
}

std::wstring ConvertNumberToString (double val, int precision, wchar_t decimalSeparator)
{
	std::wostringstream stream;
	stream << std::fixed << std::setprecision (precision) << val;
	std::wstring string = stream.str ();
	if (decimalSeparator != L'.') {
		size_t decimalPos = string.find (L'.');
		if (decimalPos != std::wstring::npos) {
			string[decimalPos] = decimalSeparator;
		}
	}
	return string;
}

std::wstring ConvertListToString (const StringConverter::ListEnumerator& enumerator, wchar_t listSeparator)
{
	std::wstring result;
	for (size_t i = 0; i < enumerator.GetSize (); ++i) {
		result += enumerator.GetItem (i);
		if (i < enumerator.GetSize () - 1) {
			result += listSeparator;
			result += ' ';
		}
	}
	return result;
}

const BasicStringSettings& GetDefaultStringSettings ()
{
	static const BasicStringSettings defaultStringSettings (L'.', L',', 2);
	return defaultStringSettings;
}

const BasicStringConverter& GetDefaultStringConverter ()
{
	static const BasicStringConverter defaultStringConverter (GetDefaultStringSettings ());
	return defaultStringConverter;
}

}
