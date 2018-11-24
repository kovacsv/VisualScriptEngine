#ifndef SIMPLETEST_HPP
#define SIMPLETEST_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#define WIN_PATH_SEPARATOR	'\\'
#define PATH_SEPARATOR		'/'

#define TEST(TESTNAME)											\
namespace TESTNAME##TestNamespace {								\
	class TESTNAME : public SimpleTest::Test {					\
	public:														\
		TESTNAME () :											\
			SimpleTest::Test (#TESTNAME)						\
		{														\
		}														\
		virtual void RunTest () override;						\
	};															\
	static class Register {										\
		public:													\
			Register ()											\
			{													\
				SimpleTest::RegisterTest (new TESTNAME ());		\
			}													\
	} TESTNAME##TestRegisterInstance;							\
}																\
void TESTNAME##TestNamespace::TESTNAME::RunTest ()

#define ASSERT(condition) TestAssert (condition, __FILE__, __LINE__)

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

void			SetAppLocation (const std::string& newAppLocation);
std::string		GetAppLocation ();
std::string		GetAppFolderLocation ();

bool			RunTests ();
void			RegisterTest (Test* test);

}

#endif
