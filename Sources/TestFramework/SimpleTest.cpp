#include "SimpleTest.hpp"

namespace SimpleTest
{

static std::string appLocation;

Test::Test (const std::string& testName) :
	testName (testName),
	testSuccess (true)
{
	
}

Test::~Test ()
{

}

bool Test::Run ()
{
	std::cout << testName << ": ";
	RunTest ();
	if (testSuccess) {
		std::cout << "SUCCESS" << std::endl;
	}
	return testSuccess;	
}

void Test::TestAssert (bool condition, const std::string& fileName, int lineNumber)
{
	if (!condition) {
		if (testSuccess) {
			std::cout << std::endl;
		}
		std::cout << "Assertion failed: " << fileName << " (" << lineNumber << ")" << std::endl;
		testSuccess = false;
	}
}

Suite::Suite ()
{

}

bool Suite::Run ()
{
	bool success = true;
	for (const std::shared_ptr<Test>& test : tests) {
		if (!test->Run ()) {
			success = false;
		}
	}
	if (!success) {
		std::cout << "FAILURE";
	} else {
		std::cout << "SUCCESS";
	}
	std::cout << std::endl;
	return success;
}

void Suite::AddTest (Test* test)
{
	tests.push_back (std::shared_ptr<Test> (test));
}

Suite& Suite::Get ()
{
	static Suite suite;
	return suite;
}

void SetAppLocation (const std::string& newAppLocation)
{
	appLocation = newAppLocation;
	for (size_t i = 0; i < appLocation.length (); ++i) {
		if (appLocation[i] == WIN_PATH_SEPARATOR) {
			appLocation[i] = PATH_SEPARATOR;
		}
	}
}

std::string GetAppLocation ()
{
	return appLocation;
}

std::string GetAppFolderLocation ()
{
	size_t lastSeparator = appLocation.find_last_of (PATH_SEPARATOR);
	if (lastSeparator == std::wstring::npos) {
		return std::string ();
	}
    std::string directoryPath = appLocation.substr (0, lastSeparator) + PATH_SEPARATOR;
	return directoryPath;
}

bool RunTests ()
{
	return Suite::Get ().Run ();
}

void RegisterTest (Test* test)
{
	Suite::Get ().AddTest (test);
}

bool Assert (bool condition, const std::string& fileName, int lineNumber)
{
	if (!condition) {
		std::cout << "FAILURE" << std::endl;
		std::cout << "-> " << fileName << " (" << lineNumber << ")" << std::endl;
	}
	return condition;
}

}
