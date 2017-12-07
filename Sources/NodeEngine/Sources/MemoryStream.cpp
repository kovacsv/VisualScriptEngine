#include "MemoryStream.hpp"
#include "Debug.hpp"

namespace NE
{

MemoryInputStream::MemoryInputStream (const std::vector<char>& buffer) :
	buffer (buffer),
	position (0)
{
	
}

MemoryInputStream::~MemoryInputStream ()
{
	
}

Stream::Status MemoryInputStream::Read (bool& val)
{
	ReadFromBuffer ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (size_t& val)
{
	ReadFromBuffer ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (int& val)
{
	ReadFromBuffer ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (double& val)
{
	ReadFromBuffer ((char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (std::string& val)
{
	size_t count;
	if (Read (count) != Status::NoError) {
		return GetStatus ();
	}

	char* str = new char[count + 1];
	ReadFromBuffer (str, count);
	str[count] = 0;

	val = str;
	delete[] str;

	return GetStatus ();
}

Stream::Status MemoryInputStream::Read (std::wstring& val)
{
	// TODO: Almost the same as the string version
	size_t count;
	if (Read (count) != Status::NoError) {
		return GetStatus ();
	}

	wchar_t* str = new wchar_t[count + 1];
	ReadFromBuffer ((char*) str, count * sizeof (wchar_t));
	str[count] = 0;

	val = str;
	delete[] str;

	return GetStatus ();
}

void MemoryInputStream::ReadFromBuffer (char* dest, size_t size)
{
	if (status != Status::NoError) {
		return;
	}

	for (size_t i = 0; i < size; i++) {
		if (DBGERROR (position >= buffer.size ())) {
			status = Status::Error;
			return;
		}
		dest[i] = buffer[position];
		position++;
	}
}

MemoryOutputStream::MemoryOutputStream ()
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
	WriteToBuffer ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const size_t& val)
{
	WriteToBuffer ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const int& val)
{
	WriteToBuffer ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const double& val)
{
	WriteToBuffer ((const char*) &val, sizeof (val));
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const std::string& val)
{
	Write (val.length ());
	WriteToBuffer (val.c_str (), val.length ());
	return GetStatus ();
}

Stream::Status MemoryOutputStream::Write (const std::wstring& val)
{
	// TODO: Almost the same as the string version
	Write (val.length ());
	WriteToBuffer ((const char*) val.c_str (), val.length () * sizeof (wchar_t));
	return GetStatus ();
}

void MemoryOutputStream::WriteToBuffer (const char* source, size_t size)
{
	if (status != Status::NoError) {
		return;
	}
	for (size_t i = 0; i < size; i++) {
		buffer.push_back (source[i]);
	}
}

}
