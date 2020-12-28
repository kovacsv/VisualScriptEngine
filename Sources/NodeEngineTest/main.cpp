#include <iostream>

#include "SimpleTest.hpp"
#include "NE_Debug.hpp"

int main (int, char* argv[])
{
	EnableLeakDetection ();

    std::string executablePath (argv[0]);
	std::wstring wExecutablePath (executablePath.begin (), executablePath.end ());
	SimpleTest::SetAppLocation (wExecutablePath);
	if (!SimpleTest::RunTests ()) {
		return 1;
	}

	return 0;
}
