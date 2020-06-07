#include "NE_LocString.hpp"

namespace NE
{

SERIALIZATION_INFO (LocString, 1);

LocString::LocString () :
	LocString (std::wstring ())
{
}

LocString::LocString (const std::wstring& strValue) :
	LocString (strValue, Localization::Localizable)
{
}

LocString::LocString (const std::wstring& strValue, Localization localization) :
	strValue (strValue),
	localization (localization)
{
}

void LocString::SetCustom (const std::wstring& customValue)
{
	if (localization == Localization::Localizable && customValue == GetLocalized ()) {
		return;
	}
	strValue = customValue;
	localization = Localization::NonLocalizable;
}

std::wstring LocString::GetLocalized () const
{
	if (localization == Localization::Localizable) {
		return LocalizeString (strValue);
	} else if (localization == Localization::NonLocalizable){
		return strValue;
	}
	DBGBREAK ();
	return strValue;
}

std::wstring LocString::GetLocalized (const Dictionary& dictionary) const
{
	if (localization == Localization::Localizable) {
		return LocalizeString (dictionary, strValue);
	} else if (localization == Localization::NonLocalizable) {
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
