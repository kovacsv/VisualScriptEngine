#include "WXAS_wxFileIO.hpp"
#include "NE_Debug.hpp"

#include <wx/file.h>

namespace WXAS
{

bool wxFileIO::ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const
{
	wxFile file (wxString (fileName), wxFile::OpenMode::read);
	if (DBGERROR (!file.IsOpened ())) {
		return false;
	}
	static const size_t bufferSize = 1024;
	char actBuffer[1024];
	size_t readBytes = 0;
	while ((readBytes = file.Read (&actBuffer, bufferSize)) != 0) {
		buffer.insert (std::end (buffer), std::begin (actBuffer), std::begin (actBuffer) + bufferSize); 
	}
	file.Close ();
	return true;
}

bool wxFileIO::WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer) const
{
	wxFile file (wxString (fileName), wxFile::OpenMode::write);
	if (DBGERROR (!file.IsOpened ())) {
		return false;
	}

	file.Write (buffer.data (), buffer.size ());
	file.Close ();
	return true;
}

}
