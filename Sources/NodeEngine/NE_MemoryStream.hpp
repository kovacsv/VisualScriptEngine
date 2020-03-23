#ifndef NE_MEMORYSTREAM_HPP
#define NE_MEMORYSTREAM_HPP

#include "NE_Stream.hpp"
#include <vector>

namespace NE
{

class MemoryInputStream : public InputStream
{
public:
	MemoryInputStream (const std::vector<char>& buffer);
	virtual ~MemoryInputStream ();

	virtual Status		Read (bool& val) override;
	virtual Status		Read (char& val) override;
	virtual Status		Read (unsigned char& val) override;
	virtual Status		Read (short& val) override;
	virtual Status		Read (size_t& val) override;
	virtual Status		Read (int& val) override;
	virtual Status		Read (float& val) override;
	virtual Status		Read (double& val) override;
	virtual Status		Read (std::string& val) override;
	virtual Status		Read (std::wstring& val) override;
	
	void				Read (char* dest, size_t size);

private:
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
	virtual Status				Write (const char& val) override;
	virtual Status				Write (const unsigned char& val) override;
	virtual Status				Write (const short& val) override;
	virtual Status				Write (const size_t& val) override;
	virtual Status				Write (const int& val) override;
	virtual Status				Write (const float& val) override;
	virtual Status				Write (const double& val) override;
	virtual Status				Write (const std::string& val) override;
	virtual Status				Write (const std::wstring& val) override;

	void						Write (const char* source, size_t size);

private:
	std::vector<char>			buffer;
};

}

#endif
