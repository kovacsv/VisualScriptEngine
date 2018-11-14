#ifndef NE_STREAM_HPP
#define NE_STREAM_HPP

#include <string>

namespace NE
{

class Stream
{
public:
	enum class Status
	{
		NoError,
		Error
	};

	Stream ();
	virtual ~Stream ();

	Status	GetStatus () const;

protected:
	Status	status;
};

class InputStream : public Stream
{
public:
	InputStream ();
	virtual ~InputStream ();

	virtual Status	Read (bool& val) = 0;
	virtual Status	Read (unsigned char& val) = 0;
	virtual Status	Read (size_t& val) = 0;
	virtual Status	Read (int& val) = 0;
	virtual Status	Read (float& val) = 0;
	virtual Status	Read (double& val) = 0;
	virtual Status	Read (std::string& val) = 0;
	virtual Status	Read (std::wstring& val) = 0;
};

class OutputStream : public Stream
{
public:
	OutputStream ();
	virtual ~OutputStream ();

	virtual Status	Write (const bool& val) = 0;
	virtual Status	Write (const unsigned char& val) = 0;
	virtual Status	Write (const size_t& val) = 0;
	virtual Status	Write (const int& val) = 0;
	virtual Status	Write (const float& val) = 0;
	virtual Status	Write (const double& val) = 0;
	virtual Status	Write (const std::string& val) = 0;
	virtual Status	Write (const std::wstring& val) = 0;
};

}

#endif
