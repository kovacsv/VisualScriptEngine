#ifndef SIMPLETEST_HPP
#define SIMPLETEST_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#define WIN_PATH_SEPARATOR	L'\\'
#define PATH_SEPARATOR		L'/'

namespace SimpleTest
{

class Test
{
public:
	Test (const std::string& testName);
	virtual ~Test ();
	
	bool				Run ();
	const std::string&	GetName () const;

protected:
	void				TestAssert (bool condition, const std::string& fileName, int lineNumber);
	virtual void		RunTest () = 0;

	std::string			testName;
	bool				testSuccess;
};

class Suite
{
public:
	Suite ();

	bool			Run ();
	void			AddTest (Test* test);

	static Suite&	Get ();

private:
	std::vector<std::shared_ptr<Test>> tests;
};

void			SetAppLocation (const std::wstring& newAppLocation);
std::wstring	GetAppLocation ();
std::wstring	GetAppFolderLocation ();

bool			RunTests ();
void			RegisterTest (Test* test);

}

#define TEST(TESTNAME)											\
class TESTNAME##_Test : public SimpleTest::Test {				\
public:															\
	TESTNAME##_Test () :										\
		SimpleTest::Test (#TESTNAME)							\
	{															\
	}															\
	virtual void RunTest () override;							\
};																\
static class TESTNAME##_Registrator {							\
	public:														\
		TESTNAME##_Registrator ()								\
		{														\
			SimpleTest::RegisterTest (new TESTNAME##_Test ());	\
		}														\
} TESTNAME##_RegistratorInstance;								\
void TESTNAME##_Test::RunTest ()

#define ASSERT(condition) TestAssert (condition, __FILE__, __LINE__)

#endif
