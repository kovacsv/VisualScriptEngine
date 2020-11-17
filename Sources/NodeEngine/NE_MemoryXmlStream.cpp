#include "NE_MemoryXmlStream.hpp"

#include <string>

namespace NE
{

static const std::wstring BoolTag		= L"Bool";
static const std::wstring CharTag		= L"Char";
static const std::wstring UCharTag		= L"UChar";
static const std::wstring ShortTag		= L"Short";
static const std::wstring SizeTag		= L"Size";
static const std::wstring IntTag		= L"Int";
static const std::wstring FloatTag		= L"Float";
static const std::wstring DoubleTag		= L"Double";
static const std::wstring StringTag		= L"String";
static const std::wstring WStringTag	= L"WString";

static const std::wstring TrueString	= L"True";
static const std::wstring FalseString	= L"False";

static std::wstring GetBeginTag (const std::wstring& tag)
{
	return L"<" + tag + L">";
}

static std::wstring GetEndTag (const std::wstring& tag)
{
	return L"</" + tag + L">";
}

MemoryXmlInputStream::MemoryXmlInputStream (const std::wstring& xmlText) :
	InputStream (),
	xmlText (xmlText),
	position (0)
{
	
}

MemoryXmlInputStream::~MemoryXmlInputStream ()
{
	
}

Stream::Status MemoryXmlInputStream::Read (bool& val)
{
	std::wstring valStr;
	Read (BoolTag, valStr);
	val = (valStr == TrueString ? true : false);
	return GetStatus ();
}

Stream::Status MemoryXmlInputStream::Read (char& val)
{
	std::wstring valStr;
	Read (CharTag, valStr);
	val = (char) std::stoi (valStr);
	return GetStatus ();
}

Stream::Status MemoryXmlInputStream::Read (unsigned char& val)
{
	std::wstring valStr;
	Read (UCharTag, valStr);
	val = (unsigned char) std::stoi (valStr);
	return GetStatus ();
}

Stream::Status MemoryXmlInputStream::Read (short& val)
{
	std::wstring valStr;
	Read (ShortTag, valStr);
	val = (short) std::stoi (valStr);
	return GetStatus ();
}

Stream::Status MemoryXmlInputStream::Read (size_t& val)
{
	std::wstring valStr;
	Read (SizeTag, valStr);
	val = (size_t) std::stoul (valStr);
	return GetStatus ();
}

Stream::Status MemoryXmlInputStream::Read (int& val)
{
	std::wstring valStr;
	Read (IntTag, valStr);
	val = std::stoi (valStr);
	return GetStatus ();
}

Stream::Status MemoryXmlInputStream::Read (float& val)
{
	std::wstring valStr;
	Read (FloatTag, valStr);
	val = std::stof (valStr);
	return GetStatus ();
}

Stream::Status MemoryXmlInputStream::Read (double& val)
{
	std::wstring valStr;
	Read (DoubleTag, valStr);
	val = std::stod (valStr);
	return GetStatus ();
}

Stream::Status MemoryXmlInputStream::Read (std::string& val)
{
	std::wstring valStr;
	Read (StringTag, valStr);
	for (wchar_t wch : valStr) {
		val += (char) wch;
	}
	return GetStatus ();
}

Stream::Status MemoryXmlInputStream::Read (std::wstring& val)
{
	Read (WStringTag, val);
	return GetStatus ();
}

void MemoryXmlInputStream::Read (const std::wstring& tag, std::wstring& text)
{
	if (status != Status::NoError) {
		return;
	}

	std::wstring beginTag = GetBeginTag (tag);
	std::wstring endTag = GetEndTag (tag);
	size_t beginTagPosition = xmlText.find (beginTag, position);
	size_t endTagPosition = xmlText.find (endTag, position);
	if (beginTagPosition == std::wstring::npos || endTagPosition == std::wstring::npos) {
		status = Status::Error;
		return;
	}
	size_t textStartPosition = beginTagPosition + beginTag.length ();
	size_t textEndPosition = endTagPosition;
	text = xmlText.substr (textStartPosition, textEndPosition - textStartPosition);
	position = endTagPosition + endTag.length ();
}

MemoryXmlOutputStream::MemoryXmlOutputStream () :
	OutputStream (),
	xmlText ()
{
	
}

MemoryXmlOutputStream::~MemoryXmlOutputStream ()
{
	
}

const std::wstring& MemoryXmlOutputStream::GetXmlText () const
{
	return xmlText;
}

Stream::Status MemoryXmlOutputStream::Write (const bool& val)
{
	Write (BoolTag, val ? TrueString : FalseString);
	return GetStatus ();
}

Stream::Status MemoryXmlOutputStream::Write (const char& val)
{
	Write (CharTag, std::to_wstring (val));
	return GetStatus ();
}

Stream::Status MemoryXmlOutputStream::Write (const unsigned char& val)
{
	Write (UCharTag, std::to_wstring (val));
	return GetStatus ();
}

Stream::Status MemoryXmlOutputStream::Write (const short& val)
{
	Write (ShortTag, std::to_wstring (val));
	return GetStatus ();
}

Stream::Status MemoryXmlOutputStream::Write (const size_t& val)
{
	Write (SizeTag, std::to_wstring (val));
	return GetStatus ();
}

Stream::Status MemoryXmlOutputStream::Write (const int& val)
{
	Write (IntTag, std::to_wstring (val));
	return GetStatus ();
}

Stream::Status MemoryXmlOutputStream::Write (const float& val)
{
	Write (FloatTag, std::to_wstring (val));
	return GetStatus ();
}

Stream::Status MemoryXmlOutputStream::Write (const double& val)
{
	Write (DoubleTag, std::to_wstring (val));
	return GetStatus ();
}

Stream::Status MemoryXmlOutputStream::Write (const std::string& val)
{
	std::wstring wStringVal (val.begin (), val.end ());
	Write (StringTag, wStringVal);
	return GetStatus ();
}

Stream::Status MemoryXmlOutputStream::Write (const std::wstring& val)
{
	Write (WStringTag, val);
	return GetStatus ();
}

void MemoryXmlOutputStream::Write (const std::wstring& tag, const std::wstring& text)
{
	std::wstring beginTag = GetBeginTag (tag);
	std::wstring endTag = GetEndTag (tag);
	xmlText += beginTag + text + endTag + L"\n";
}

}
