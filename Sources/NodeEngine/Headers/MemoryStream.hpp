#ifndef MEMORYSTREAM_HPP
#define MEMORYSTREAM_HPP

#include "Stream.hpp"
#include <vector>

namespace NE
{

class MemoryInputStream : public InputStream
{
public:
	MemoryInputStream (const std::vector<char>& buffer);
	virtual ~MemoryInputStream ();

	virtual Status		Read (bool& val) override;
	virtual Status		Read (size_t& val) override;
	virtual Status		Read (int& val) override;
	virtual Status		Read (double& val) override;
	virtual Status		Read (std::string& val) override;
	virtual Status		Read (std::wstring& val) override;

private:
	void				ReadFromBuffer (char* dest, size_t size);

	std::vector<char>	buffer;
	size_t				position;
};

class MemoryOutputStream : public OutputStream
{
public:
	MemoryOutputStream ();
	virtual ~MemoryOutputStream ();

	const std::vector<char>&	GetBuffer () const;

	virtual Status				Write (const bool& val) override;
	virtual Status				Write (const size_t& val) override;
	virtual Status				Write (const int& val) override;
	virtual Status				Write (const double& val) override;
	virtual Status				Write (const std::string& val) override;
	virtual Status				Write (const std::wstring& val) override;

private:
	void						WriteToBuffer (const char* source, size_t size);

	std::vector<char>			buffer;
};

}

#endif
