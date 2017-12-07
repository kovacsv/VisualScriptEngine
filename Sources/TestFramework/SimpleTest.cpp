#include "SimpleTest.hpp"

namespace SimpleTest
{

static std::string appLocation;

AssertException::AssertException (const std::string& fileName, int lineNumber) :
	fileName (fileName),
	lineNumber (lineNumber)
{
	
}

const std::string& AssertException::GetFileName () const
{
	return fileName;
}

int AssertException::GetLineNumber () const
{
	return lineNumber;
}

Test::Test (const std::string& name) :
	name (name)
{
	
}

Test::~Test ()
{

}

bool Test::Run ()
{
	try {
		std::cout << name << ": ";
		RunTest ();
	} catch (const AssertException& ex) {
		std::cout << "FAILURE" << std::endl;
		std::cout << "-> " << ex.GetFileName () << " (" << ex.GetLineNumber () << ")" << std::endl;
		return false;
	}
	std::cout << "SUCCESS" << std::endl;
	return true;	
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
}

std::string GetAppLocation ()
{
	return appLocation;
}

std::string GetAppFolderLocation ()
{
	static const std::string separator (PATH_SEPARATOR);
	size_t lastSeparator = appLocation.find_last_of (separator);
	if (lastSeparator == std::wstring::npos) {
		return std::string ();
	}
    std::string directoryPath = appLocation.substr (0, lastSeparator) + separator;
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

void Assert (bool condition, const std::string& fileName, int lineNumber)
{
	if (!condition) {
		throw AssertException (fileName, lineNumber);
	}
}

}
