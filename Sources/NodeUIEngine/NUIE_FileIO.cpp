#include "NUIE_FileIO.hpp"
#include "NE_StringUtils.hpp"
#include "NE_Debug.hpp"

#include <codecvt>
#include <locale>
#include <fstream>
#include <sstream>

namespace NUIE
{

bool ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer)
{
	std::ifstream file;
	file.open (NE::WStringToString (fileName), std::ios::binary);
	if (DBGERROR (!file.is_open ())) {
		return false;
	}

	buffer.assign (std::istreambuf_iterator<char> (file), std::istreambuf_iterator<char> ());
	file.close ();

	return true;
}

bool WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer)
{
	std::ofstream file;
	file.open (NE::WStringToString (fileName), std::ios::binary);
	if (DBGERROR (!file.is_open ())) {
		return false;
	}

	file.write (buffer.data (), buffer.size ());
	file.close ();

	return true;
}

bool ReadUtf8File (const std::wstring& fileName, std::wstring& content)
{
	std::wifstream file;

	std::locale loc (std::locale (), new std::codecvt_utf8<wchar_t> ());
	file.imbue (loc);

	file.open (NE::WStringToString (fileName));
	if (!file.is_open ()) {
		return false;
	}

	std::wstringstream fileBuffer;
	fileBuffer << file.rdbuf ();
	content = fileBuffer.str ();
	file.close ();

	return true;
}

bool WriteUtf8File (const std::wstring& fileName, const std::wstring& content)
{
	std::wofstream file;

	std::locale loc (std::locale (), new std::codecvt_utf8<wchar_t> ());
	file.imbue (loc);

	file.open (NE::WStringToString (fileName));
	if (!file.is_open ()) {
		return false;
	}

	file << content;
	file.close ();
	return true;
}

}
