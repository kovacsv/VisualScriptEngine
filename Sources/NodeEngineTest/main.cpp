#include <iostream>

#include "SimpleTest.hpp"
#include "NE_Debug.hpp"

int main (int, char* argv[])
{
	EnableLeakDetection ();

    std::string executablePath (argv[0]);
	SimpleTest::SetAppLocation (executablePath);
	if (!SimpleTest::RunTests ()) {
		return 1;
	}

	return 0;
}
