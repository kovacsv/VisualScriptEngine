#include <iostream>

#include "SimpleTest.hpp"
#include "NE_Debug.hpp"
#include "NUIE_FeatureSet.hpp"

int main (int, char* argv[])
{
	EnableLeakDetection ();
	NUIE::EnableFeature ("HideConnections", true);

    std::string executablePath (argv[0]);
	std::wstring wExecutablePath (executablePath.begin (), executablePath.end ());
	SimpleTest::SetAppLocation (wExecutablePath);
	if (!SimpleTest::RunTests ()) {
		return 1;
	}

	return 0;
}
