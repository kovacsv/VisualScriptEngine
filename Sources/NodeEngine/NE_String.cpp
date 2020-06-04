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
	strValue (strValue),
	isCustom (false)
{
}

void String::SetCustom (const std::wstring& customValue)
{
	if (!isCustom && customValue == GetLocalized ()) {
		return;
	}
	strValue = customValue;
	isCustom = true;
}

std::wstring String::GetLocalized () const
{
	if (isCustom) {
		return strValue;
	} else {
		return LocalizeString (strValue);
	}
}

std::wstring String::GetLocalized (const Dictionary& dictionary) const
{
	if (isCustom) {
		return strValue;
	} else {
		return LocalizeString (dictionary, strValue);
	}
}

Stream::Status String::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	inputStream.Read (strValue);
	inputStream.Read (isCustom);
	return inputStream.GetStatus ();
}

Stream::Status String::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (strValue);
	outputStream.Write (isCustom);
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
