#include "WAS_WindowsFileIO.hpp"
#include "NE_Debug.hpp"

#include <iostream>
#include <fstream>

namespace WAS
{

bool WindowsFileIO::ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const
{
	std::ifstream file;
	file.open (fileName, std::ios::binary);
	if (DBGERROR (!file.is_open ())) {
		return false;
	}
 
	buffer.assign (std::istreambuf_iterator<char> (file), std::istreambuf_iterator<char> ());
	file.close ();
	
	return true;
}

bool WindowsFileIO::WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer) const
{
	std::ofstream file;
	file.open (fileName, std::ios::binary);
	if (DBGERROR (!file.is_open ())) {
		return false;
	}

	file.write (buffer.data (), buffer.size ());
	file.close ();

	return true;
}

}
