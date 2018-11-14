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

BasicStringSettings DefaultStringSettings (L'.', L',', 2);

}
