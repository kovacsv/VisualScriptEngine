#include "NE_StringSettings.hpp"

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

const BasicStringSettings& GetDefaultStringSettings ()
{
	static const BasicStringSettings defaultStringSettings (L'.', L',', 2);
	return defaultStringSettings;
}

}
