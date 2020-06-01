#ifndef NE_MEMORYXMLSTREAM_HPP
#define NE_MEMORYXMLSTREAM_HPP

#include "NE_Stream.hpp"
#include <vector>

namespace NE
{

class MemoryXmlInputStream : public InputStream
{
public:
	MemoryXmlInputStream (const std::wstring& xmlText);
	virtual ~MemoryXmlInputStream ();

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
	
	void				Read (const std::wstring& tag, std::wstring& text);

private:
	std::wstring		xmlText;
	size_t				position;
};

class MemoryXmlOutputStream : public OutputStream
{
public:
	MemoryXmlOutputStream ();
	virtual ~MemoryXmlOutputStream ();

	const std::wstring&			GetXmlText () const;

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

	void						Write (const std::wstring& tag, const std::wstring& text);

private:
	std::wstring				xmlText;
};

}

#endif
