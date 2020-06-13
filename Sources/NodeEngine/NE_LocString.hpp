#ifndef NE_LOCSTRING_HPP
#define NE_LOCSTRING_HPP

#include "NE_Serializable.hpp"
#include "NE_Localization.hpp"
#include <string>

namespace NE
{

class LocString
{
	SERIALIZABLE;

public:
	enum class Localization
	{
		Localize		= 1,
		DoNotLocalize	= 2
	};

	explicit LocString ();
	explicit LocString (const std::wstring& strValue);
	explicit LocString (const std::wstring& strValue, Localization localization);

	void			SetCustom (const std::wstring& customValue);
	std::wstring	GetLocalized () const;
	std::wstring	GetLocalized (const Dictionary& dictionary) const;

	Stream::Status	Read (InputStream& inputStream);
	Stream::Status	Write (OutputStream& outputStream) const;

private:
	std::wstring	strValue;
	Localization	localization;
};

}

#endif
