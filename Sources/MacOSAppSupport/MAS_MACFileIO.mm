#include "MAS_MACFileIO.hpp"
#include "NE_StringUtils.hpp"

#include <iostream>
#include <fstream>

namespace MAS
{

bool MACFileIO::ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const
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

bool MACFileIO::WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer) const
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

}
