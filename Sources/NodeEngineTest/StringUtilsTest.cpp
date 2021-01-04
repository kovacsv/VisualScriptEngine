#include "SimpleTest.hpp"
#include "NE_StringUtils.hpp"

using namespace NE;

namespace StringUtilsTest
{

TEST (ReplaceAllTest)
{
	std::wstring text = L"example of example of example";
	ASSERT (ReplaceAll (text, L"e", L"") == L"xampl of xampl of xampl");
	ASSERT (ReplaceAll (text, L"e", L"a") == L"axampla of axampla of axampla");
	ASSERT (ReplaceAll (text, L"z", L"a") == L"example of example of example");
	ASSERT (ReplaceAll (text, L"e", L"u") == L"uxamplu of uxamplu of uxamplu");
	ASSERT (ReplaceAll (text, L"e", L"e") == L"example of example of example");
	ASSERT (ReplaceAll (text, L"e", L"ee") == L"eexamplee of eexamplee of eexamplee");
	ASSERT (ReplaceAll (text, L"example", L"s") == L"s of s of s");
	ASSERT (ReplaceAll (text, L"example", L"something") == L"something of something of something");
}

TEST (ReplaceAllMultipleTest)
{
	std::wstring text = L"example of example of example";
	ASSERT (ReplaceAll (text, L"e", { L"", L"", L"", L"", L"", L"" }) == L"xampl of xampl of xampl");
	ASSERT (ReplaceAll (text, L"z", {}) == L"example of example of example");
	ASSERT (ReplaceAll (text, L"e", { L"a", L"b", L"c", L"d", L"e", L"f" }) == L"axamplb of cxampld of examplf");
	ASSERT (ReplaceAll (text, L"e", { L"a", L"bb", L"ccc", L"dddd", L"eeeee", L"ffffff" }) == L"axamplbb of cccxampldddd of eeeeexamplffffff");
	ASSERT (ReplaceAll (text, L"example", { L"x", L"y", L"z" }) == L"x of y of z");
	ASSERT (ReplaceAll (text, L"example", { L"example", L"example", L"example" }) == L"example of example of example");
	ASSERT (ReplaceAll (text, L"example", { L"something", L"more something", L"much more something" }) == L"something of more something of much more something");
}

TEST (ReplaceAllUnicodeTest)
{
	ASSERT (ReplaceAll (L"ABA", L"B", L"\u03c0") == L"A\u03c0A");
	ASSERT (ReplaceAll (L"ABA", L"B", L"\u03c0\u03c0") == L"A\u03c0\u03c0A");
	ASSERT (ReplaceAll (L"A\u03c0A", L"\u03c0", L"B") == L"ABA");
	ASSERT (ReplaceAll (L"A\u03c0B\u03c0A", L"\u03c0B\u03c0", L"B") == L"ABA");

	ASSERT (ReplaceAll (L"ABA", L"B", { L"\u03c0" }) == L"A\u03c0A");
	ASSERT (ReplaceAll (L"ABA", L"B", { L"\u03c0\u03c0" }) == L"A\u03c0\u03c0A");
	ASSERT (ReplaceAll (L"A\u03c0A", L"\u03c0", { L"B" }) == L"ABA");
	ASSERT (ReplaceAll (L"A\u03c0B\u03c0A", L"\u03c0B\u03c0", { L"B" }) == L"ABA");

	ASSERT (ReplaceAll (L"ABA", L"A", { L"\u03c0", L"\u03c0" }) == L"\u03c0B\u03c0");
	ASSERT (ReplaceAll (L"ABA", L"A", { L"\u03c0\u03c0", L"\u03c0\u03c0" }) == L"\u03c0\u03c0B\u03c0\u03c0");
	ASSERT (ReplaceAll (L"\u03c0B\u03c0", L"\u03c0", { L"A", L"A" }) == L"ABA");
	ASSERT (ReplaceAll (L"A\u03c0B\u03c0A", L"\u03c0B\u03c0", { L"B" }) == L"ABA");
}

}
