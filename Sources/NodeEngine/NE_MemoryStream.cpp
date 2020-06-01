#include "NE_MemoryStream.hpp"
#include "NE_Debug.hpp"

namespace NE
{

template <typename CharType, typename StringType>
Stream::Status ReadString (MemoryInputStream& stream, StringType& val)
{
	size_t count = 0;
	if (stream.Read (count) != Stream::Status::NoError) {
		return stream.GetStatus ();
	}

	CharType* str = new CharType[count + 1];
	stream.Read ((char*) str, count * sizeof (CharType));
	str[count] = 0;

	val = str;
	delete[] str;

	return stream.GetStatus ();
}

template <typename CharType, typename StringType>
Stream::Status WriteString (MemoryOutputStream& stream, const StringType& val)
{
	stream.Write (val.length ());
	stream.Write ((const char*) val.c_str (), val.length () * sizeof (CharType));
	return stream.GetStatus ();
}

MemoryInputStream::MemoryInputStream (const std::vector<char>& buffer) :
	InputStream (),
	buffer (buffer),
	position (0)
{
	
}

MemoryInputStream::~MemoryInputStream ()
{
	
}

Stream::Status MemoryInputStream::Read (bool& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (char& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (unsigned char& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (short& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (size_t& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (int& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (float& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (double& val)
{
	Read ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (std::string& val)
{
	return ReadString<char, std::string> (*this, val);
}

Stream::Status MemoryInputStream::Read (std::wstring& val)
{
	return ReadString<wchar_t, std::wstring> (*this, val);
}

void MemoryInputStream::Read (char* dest, size_t size)
{
	if (status != Status::NoError) {
		return;
	}
	if (DBGERROR (position + size > buffer.size ())) {
		status = Status::Error;
		return;
	}
	std::copy (buffer.begin () + position, buffer.begin () + position + size, dest);
	position += size;
}

MemoryOutputStream::MemoryOutputStream () :
	OutputStream (),
	buffer ()
{
	
}

MemoryOutputStream::~MemoryOutputStream ()
{
	
}

const std::vector<char>& MemoryOutputStream::GetBuffer () const
{
	return buffer;
}

Stream::Status MemoryOutputStream::Write (const bool& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const char& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const unsigned char& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const short& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const size_t& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const int& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const float& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const double& val)
{
	Write ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const std::string& val)
{
	return WriteString<char, std::string> (*this, val);
}

Stream::Status MemoryOutputStream::Write (const std::wstring& val)
{
	return WriteString<wchar_t, std::wstring> (*this, val);
}

void MemoryOutputStream::Write (const char* source, size_t size)
{
	if (status != Status::NoError) {
		return;
	}
	buffer.insert (buffer.end (), source, source + size);
}

}
