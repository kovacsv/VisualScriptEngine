#include "SimpleTest.hpp"

#ifdef _WIN32
	#include <windows.h>
#endif

namespace SimpleTest
{

enum class ConsoleColor
{
	Normal,
	Red,
	Green
};

#ifdef _WIN32
class ColoredConsoleWriter
{
public:
	ColoredConsoleWriter (ConsoleColor consoleColor) :
		consoleColor (consoleColor)
	{
		consoleHandle = GetStdHandle (STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo (consoleHandle, &screenBufferInfo);
	}

	~ColoredConsoleWriter ()
	{
		SetConsoleTextAttribute (consoleHandle, screenBufferInfo.wAttributes);
	}

	void Write (const std::string& text)
	{
		if (consoleColor == ConsoleColor::Green) {
			SetConsoleTextAttribute (consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		} else if (consoleColor == ConsoleColor::Red) {
			SetConsoleTextAttribute (consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		std::cout << text;
		SetConsoleTextAttribute (consoleHandle, screenBufferInfo.wAttributes);
	}

private:
	ConsoleColor				consoleColor;
	HANDLE						consoleHandle;
	CONSOLE_SCREEN_BUFFER_INFO	screenBufferInfo;
};
#else
class ColoredConsoleWriter
{
public:
	ColoredConsoleWriter (ConsoleColor)
	{
	}

	~ColoredConsoleWriter ()
	{
	}

	void Write (const std::string& text)
	{
		std::cout << text;
	}
};
#endif

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
	ColoredConsoleWriter green (ConsoleColor::Green);
	ColoredConsoleWriter red (ConsoleColor::Red);
	green.Write ("[ RUNNING ] ");
	std::cout << testName;
	RunTest ();
	if (testSuccess) {
		std::cout << std::endl;
		green.Write ("[ SUCCESS ]");
	} else {
		red.Write ("[ FAILURE ]");
	}
	std::cout << " " << testName << std::endl;
	return testSuccess;	
}

const std::string& Test::GetName () const
{
	return testName;
}

void Test::TestAssert (bool condition, const std::string& fileName, int lineNumber)
{
	if (!condition) {
		if (testSuccess) {
			std::cout << std::endl;
		}
		std::cout << "Assertion failed: " << fileName << " (" << lineNumber << ")" << std::endl;
#ifdef _WIN32
		OutputDebugStringA (fileName.c_str ());
		OutputDebugStringA ("(");
		OutputDebugStringA (std::to_string (lineNumber).c_str ());
		OutputDebugStringA ("): Assertion failed\n");
#endif
		testSuccess = false;
	}
}

Suite::Suite ()
{

}

bool Suite::Run ()
{
	ColoredConsoleWriter green (ConsoleColor::Green);
	ColoredConsoleWriter red (ConsoleColor::Red);
	green.Write ("[ ------- ] ");
	std::cout << "Running " << tests.size () << " tests." << std::endl;
	std::vector<std::string> failedTestNames;
	for (const std::shared_ptr<Test>& test : tests) {
		if (!test->Run ()) {
			failedTestNames.push_back (test->GetName ());
		}
	}
	green.Write ("[ ------- ] ");
	std::cout << "Finished running " << tests.size () << " tests." << std::endl << std::endl;
	bool success = (failedTestNames.empty ());
	if (success) {
		green.Write ("[ ------- ] ");
		std::cout << std::endl;
		green.Write ("[ SUCCESS ] ");
		std::cout << "All tests succeeeded." << std::endl;
		green.Write ("[ ------- ] ");
	} else {
		red.Write ("[ ------- ] ");
		std::cout << std::endl;
		red.Write ("[ FAILURE ] ");
		std::cout << "The following tests are failed:" << std::endl;
		for (const std::string& failedTestName : failedTestNames) {
			red.Write ("[ FAILURE ] ");
			std::cout << failedTestName << std::endl;
		}
		red.Write ("[ ------- ] ");
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
