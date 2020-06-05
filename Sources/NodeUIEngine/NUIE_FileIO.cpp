#include "NUIE_FileIO.hpp"

#include <codecvt>
#include <locale>
#include <fstream>
#include <sstream>

namespace NUIE
{

bool WriteUtf8File (const std::string& fileName, const std::wstring& content)
{
	std::wofstream file;

	std::locale loc (std::locale (), new std::codecvt_utf8<wchar_t> ());
	file.imbue (loc);

	file.open (fileName);
	if (!file.is_open ()) {
		return false;
	}

	file << content;
	file.close ();
	return true;
}

bool ReadUtf8File (const std::string& fileName, std::wstring& content)
{
	std::wifstream file;

	std::locale loc (std::locale (), new std::codecvt_utf8<wchar_t> ());
	file.imbue (loc);

	file.open (fileName);
	if (!file.is_open ()) {
		return false;
	}

	std::wstringstream fileBuffer;
	fileBuffer << file.rdbuf ();
	content = fileBuffer.str ();
	file.close ();

	return true;
}

}
