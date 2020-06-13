#include "NE_LocString.hpp"

namespace NE
{

SERIALIZATION_INFO (LocString, 1);

LocString::LocString () :
	LocString (std::wstring ())
{
}

LocString::LocString (const std::wstring& strValue) :
	LocString (strValue, Localization::Localize)
{
}

LocString::LocString (const std::wstring& strValue, Localization localization) :
	strValue (strValue),
	localization (localization)
{
}

void LocString::SetCustom (const std::wstring& customValue)
{
	if (localization == Localization::Localize && customValue == GetLocalized ()) {
		return;
	}
	strValue = customValue;
	localization = Localization::DoNotLocalize;
}

std::wstring LocString::GetLocalized () const
{
	if (localization == Localization::Localize) {
		return LocalizeString (strValue);
	} else if (localization == Localization::DoNotLocalize){
		return strValue;
	}
	DBGBREAK ();
	return strValue;
}

std::wstring LocString::GetLocalized (const Dictionary& dictionary) const
{
	if (localization == Localization::Localize) {
		return LocalizeString (dictionary, strValue);
	} else if (localization == Localization::DoNotLocalize) {
		return strValue;
	}
	DBGBREAK ();
	return strValue;
}

Stream::Status LocString::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	inputStream.Read (strValue);
	ReadEnum<Localization> (inputStream, localization);
	return inputStream.GetStatus ();
}

Stream::Status LocString::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (strValue);
	WriteEnum<Localization> (outputStream, localization);
	return outputStream.GetStatus ();
}

}
