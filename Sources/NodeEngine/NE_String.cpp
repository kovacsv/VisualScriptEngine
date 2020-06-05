#include "NE_String.hpp"

namespace NE
{

SERIALIZATION_INFO (String, 1);
SERIALIZATION_INFO (LocString, 1);

String::String () :
	String (std::wstring ())
{
}

String::String (const std::wstring& strValue) :
	String (strValue, Localization::Localizable)
{
}

String::String (const std::wstring& strValue, Localization localization) :
	strValue (strValue),
	localization (localization)
{
}

void String::SetCustom (const std::wstring& customValue)
{
	if (localization == Localization::Localizable && customValue == GetLocalized ()) {
		return;
	}
	strValue = customValue;
	localization = Localization::NonLocalizable;
}

std::wstring String::GetLocalized () const
{
	if (localization == Localization::Localizable) {
		return LocalizeString (strValue);
	} else if (localization == Localization::NonLocalizable){
		return strValue;
	}
	DBGBREAK ();
	return strValue;
}

std::wstring String::GetLocalized (const Dictionary& dictionary) const
{
	if (localization == Localization::Localizable) {
		return LocalizeString (dictionary, strValue);
	} else if (localization == Localization::NonLocalizable) {
		return strValue;
	}
	DBGBREAK ();
	return strValue;
}

Stream::Status String::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	inputStream.Read (strValue);
	ReadEnum<Localization> (inputStream, localization);
	return inputStream.GetStatus ();
}

Stream::Status String::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (strValue);
	WriteEnum<Localization> (outputStream, localization);
	return outputStream.GetStatus ();
}

LocString::LocString () :
	LocString (std::wstring ())
{
}

LocString::LocString (const std::wstring& strValue) :
	strValue (strValue)
{
}

std::wstring LocString::GetLocalized () const
{
	return LocalizeString (strValue);
}

std::wstring LocString::GetLocalized (const Dictionary& dictionary) const
{
	return LocalizeString (dictionary, strValue);
}

Stream::Status LocString::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	inputStream.Read (strValue);
	return inputStream.GetStatus ();
}

Stream::Status LocString::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (strValue);
	return outputStream.GetStatus ();
}

}
