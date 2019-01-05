#ifndef WAS_WXFILEIO_HPP
#define WAS_WXFILEIO_HPP

#include "NUIE_NodeEditor.hpp"

namespace WXAS
{

class wxFileIO : public NUIE::ExternalFileIO
{
public:
	virtual bool	ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const override;
	virtual bool	WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer) const override;
};

}

#endif
