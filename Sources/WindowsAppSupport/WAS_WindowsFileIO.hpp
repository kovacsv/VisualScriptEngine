#ifndef WAS_WINDOWSFILEIO_HPP
#define WAS_WINDOWSFILEIO_HPP

#include "NUIE_NodeEditor.hpp"

namespace WAS
{

class WindowsFileIO : public NUIE::ExternalFileIO
{
public:
	virtual bool	ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const override;
	virtual bool	WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer) const override;
};

}

#endif
