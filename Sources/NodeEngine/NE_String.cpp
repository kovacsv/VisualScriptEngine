#include "NE_String.hpp"

namespace NE
{

SerializationInfo String::serializationInfo (ObjectVersion (1));

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

}
