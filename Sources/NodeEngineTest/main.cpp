#include <iostream>

#include "SimpleTest.hpp"
#include "Debug.hpp"

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
