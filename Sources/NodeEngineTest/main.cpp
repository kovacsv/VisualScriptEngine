#include <iostream>

#include "SimpleTest.hpp"
#include "NE_Debug.hpp"

int wmain (int, wchar_t* argv[])
{
	EnableLeakDetection ();

    std::wstring executablePath (argv[0]);
	SimpleTest::SetAppLocation (executablePath);
	if (!SimpleTest::RunTests ()) {
		return 1;
	}

	return 0;
}
