#include "SimpleTest.hpp"
#include "NE_StringUtils.hpp"

using namespace NE;

namespace StringUtilsTest
{

TEST (ReplaceAllTest)
{
	std::wstring text = L"example of example of example";
	ASSERT (ReplaceAll (text, L"e", L"a") == L"axampla of axampla of axampla");
	ASSERT (ReplaceAll (text, L"e", L"u") == L"uxamplu of uxamplu of uxamplu");
	ASSERT (ReplaceAll (text, L"e", L"e") == L"example of example of example");
	ASSERT (ReplaceAll (text, L"e", L"ee") == L"eexamplee of eexamplee of eexamplee");
	ASSERT (ReplaceAll (text, L"example", L"s") == L"s of s of s");
	ASSERT (ReplaceAll (text, L"example", L"something") == L"something of something of something");
}

}
