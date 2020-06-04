#ifndef NE_STRING_HPP
#define NE_STRING_HPP

#include "NE_Serializable.hpp"
#include "NE_Localization.hpp"
#include <string>

namespace NE
{

class String
{
	SERIALIZABLE;

public:
	explicit String ();
	explicit String (const std::wstring& strValue);

	void			SetCustom (const std::wstring& customValue);
	std::wstring	GetLocalized () const;
	std::wstring	GetLocalized (const Dictionary& dictionary) const;

	Stream::Status	Read (InputStream& inputStream);
	Stream::Status	Write (OutputStream& outputStream) const;

private:
	std::wstring	strValue;
	bool			isCustom;
};

class LocString
{
	SERIALIZABLE;

public:
	explicit LocString ();
	explicit LocString (const std::wstring& strValue);

	std::wstring	GetLocalized () const;
	std::wstring	GetLocalized (const Dictionary& dictionary) const;

	Stream::Status	Read (InputStream& inputStream);
	Stream::Status	Write (OutputStream& outputStream) const;

private:
	std::wstring	strValue;
};

}

#endif
