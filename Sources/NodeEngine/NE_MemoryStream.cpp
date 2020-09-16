#include "NE_MemoryStream.hpp"
#include "NE_StringUtils.hpp"
#include "NE_Debug.hpp"

namespace NE
{

#ifdef _WIN32
#define SIZEOFWCHAR 2
#else
#define SIZEOFWCHAR 4
#endif

static_assert (sizeof (bool) == 1, "invalid size for bool");
static_assert (sizeof (char) == 1, "invalid size for char");
static_assert (sizeof (wchar_t) == SIZEOFWCHAR, "invalid size for wchar_t");
static_assert (sizeof (char16_t) == 2, "invalid size for double");
static_assert (sizeof (char32_t) == 4, "invalid size for double");
static_assert (sizeof (double) == 8, "invalid size for double");
static_assert (sizeof (unsigned char) == 1, "invalid size for unsigned char");
static_assert (sizeof (short) == 2, "invalid size for short");
static_assert (sizeof (size_t) <= sizeof (uint64_t), "invalid size for size_t");
static_assert (sizeof (uint64_t) == 8, "invalid size for uint64_t");
static_assert (sizeof (int) == 4, "invalid size for int");
static_assert (sizeof (float) == 4, "invalid size for float");
static_assert (sizeof (double) == 8, "invalid size for double");

static Stream::Status ReadString (MemoryInputStream& stream, std::string& val)
{
	size_t count = 0;
	if (stream.Read (count) != Stream::Status::NoError) {
		return stream.GetStatus ();
	}

	char* str = new char[count + 1];
	stream.Read ((char*) str, count * sizeof (char));
	str[count] = 0;

	val = str;
	delete[] str;

	return stream.GetStatus ();
}

static Stream::Status WriteString (MemoryOutputStream& stream, const std::string& val)
{
	stream.Write (val.length ());
	stream.Write ((const char*) val.c_str (), val.length () * sizeof (char));
	return stream.GetStatus ();
}

Stream::Status ReadString (MemoryInputStream& stream, std::wstring& val)
{
	std::string str;
	Stream::Status status = ReadString (stream, str);
	if (status != Stream::Status::NoError) {
		return status;
	}

	val = StringToWString (str);
	return Stream::Status::NoError;
}

static Stream::Status WriteString (MemoryOutputStream& stream, const std::wstring& val)
{
	std::string str = WStringToString (val);
	return WriteString (stream, str);
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
	uint64_t val64 = 0;
	Read ((char*) &val64, sizeof (val64));
	val = (size_t) val64;
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
	return ReadString (*this, val);
}

Stream::Status MemoryInputStream::Read (std::wstring& val)
{
	return ReadString (*this, val);
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
	uint64_t val64 = (uint64_t) val;
	Write ((char*) &val64, sizeof (val64));
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
	return WriteString (*this, val);
}

Stream::Status MemoryOutputStream::Write (const std::wstring& val)
{
	return WriteString (*this, val);
}

void MemoryOutputStream::Write (const char* source, size_t size)
{
	if (status != Status::NoError) {
		return;
	}
	buffer.insert (buffer.end (), source, source + size);
}

}
