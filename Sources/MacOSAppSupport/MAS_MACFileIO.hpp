#ifndef MAS_MACFILEIO_HPP
#define MAS_MACFILEIO_HPP

#include "NUIE_NodeEditor.hpp"

namespace MAS
{

class MACFileIO : public NUIE::ExternalFileIO
{
public:
	virtual bool	ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const override;
	virtual bool	WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer) const override;
};

}

#endif
