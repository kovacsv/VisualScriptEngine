#include "SimpleTest.hpp"
#include "NE_LocString.hpp"
#include "NUIE_Localization.hpp"

using namespace NE;
using namespace NUIE;

namespace LocalizationTest
{

TEST (EmptyStringTest)
{
	std::wstring poContent = LR"(
		msgid ""
		msgstr ""
		msgid "Apple"
		msgstr "Alma"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.GetLocalizedString (L"") == L"");
	ASSERT (dictionary.GetLocalizedString (L"Apple") == L"Alma");
}

TEST (SimplePoFileTest)
{
	std::wstring poContent = LR"(
		# this is a comment and an empty line after it
		
		msgid "Apple"
		msgstr "Alma"
		msgid "Banana"

		# another comment
		msgstr "Banan"
		invalidcommand "InvalidMessage"
		
		msgid "Lemon"
		msgstr "Citrom"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.GetLocalizedString (L"Apple") == L"Alma");
	ASSERT (dictionary.GetLocalizedString (L"Banana") == L"Banan");
	ASSERT (dictionary.GetLocalizedString (L"Lemon") == L"Citrom");
}

TEST (SimplePoFileWithEscapeTest)
{
	std::wstring poContent = LR"(
		msgid "App\"le"
		msgstr "Al\"ma"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.GetLocalizedString (L"App\"le") == L"Al\"ma");
}

TEST (SimplePoFileNonLocalizedTest)
{
	std::wstring poContent = LR"(
		msgid "Apple"
		msgstr "Alma"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.GetLocalizedString (L"Apple") == L"Alma");
	ASSERT (dictionary.GetLocalizedString (L"Banana") == L"Banana");
	ASSERT (dictionary.GetLocalizedString (L"Lemon") == L"Lemon");
}

TEST (SimplePoFileWithFormatTest)
{
	std::wstring poContent = LR"(
		msgid "apple"
		msgstr "alma"
		msgid "this is an %S"
		msgstr "ez itt egy %S"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.GetLocalizedString (L"apple") == L"alma");
	ASSERT (FormatString (dictionary.GetLocalizedString (L"this is an %S"), { dictionary.GetLocalizedString (L"apple") }) == L"ez itt egy alma");
}

TEST (SimplePoFileWithInvalidFormatTest)
{
	std::wstring poContent = LR"(
		msgid "Apple"
		msgstr "Alma"
		msgstr "Banan"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (!FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.IsEmpty ());
}

TEST (SimplePoFileWithInvalidFormatTest2)
{
	std::wstring poContent = LR"(
		msgid "Apple"
		msgid "Banana"
		msgstr "Banan"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (!FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.IsEmpty ());
}

TEST (SimplePoFileWithInvalidFormatTest3)
{
	std::wstring poContent = LR"(
		msgid ""Apple"
		msgstr "Alma"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (!FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.IsEmpty ());
}

TEST (SimplePoFileWithInvalidFormatTest4)
{
	std::wstring poContent = LR"(
		msgid "App"le"
		msgstr "Alma"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (!FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.IsEmpty ());
}

TEST (StringLocalizationTest)
{
	std::wstring poContent = LR"(
		msgid "apple"
		msgstr "alma"
		msgid "lemon"
		msgstr "citrom"
	)";

	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (FillDictionary (dictionary, poDictionarySource));

	LocString appleString (L"apple");
	LocString appleNonLocString (L"apple", LocString::Localization::DoNotLocalize);
	LocString lemonString (L"lemon");

	ASSERT (appleString.GetLocalized (dictionary) == L"alma");
	ASSERT (appleNonLocString.GetLocalized (dictionary) == L"apple");
	ASSERT (lemonString.GetLocalized (dictionary) == L"citrom");

	appleString.SetCustom (L"apple");
	ASSERT (appleString.GetLocalized (dictionary) == L"alma");

	appleString.SetCustom (L"banana");
	ASSERT (appleString.GetLocalized (dictionary) == L"banana");

	appleString.SetCustom (L"lemon");
	ASSERT (appleString.GetLocalized (dictionary) == L"lemon");

	appleString.SetCustom (L"apple");
	ASSERT (appleString.GetLocalized (dictionary) == L"apple");
}

TEST (FormatStringTest)
{
	ASSERT (FormatString (L"", {}) == L"");
	ASSERT (FormatString (L"Example", {}) == L"Example");
	ASSERT (FormatString (L"Example%", {}) == L"Example%");
	ASSERT (FormatString (L"Example%Example", {}) == L"Example%Example");
	ASSERT (FormatString (L"Example%s", { }) == L"Example%s");

	ASSERT (FormatString (L"%S", { L"" }) == L"");
	ASSERT (FormatString (L"%S", { L"A" }) == L"A");
	
	ASSERT (FormatString (L"Example%S", { L"A" }) == L"ExampleA");
	ASSERT (FormatString (L"Example%S", { L"%S" }) == L"Example%S");
	ASSERT (FormatString (L"Example%S%S", { L"%S", L"A" }) == L"Example%SA");
	ASSERT (FormatString (L"Example%S%S", { L"A", L"BC" }) == L"ExampleABC");
	ASSERT (FormatString (L"%SExample%S%S", { L"A", L"BC", L"DEF" }) == L"AExampleBCDEF");
	ASSERT (FormatString (L"%SExam%Sple%S%S", { L"A", L"BC", L"DEF", L"GHIJ" }) == L"AExamBCpleDEFGHIJ");

	ASSERT (FormatString (L"%S (%S)", { L"A", L"B" }) == L"A (B)");
	ASSERT (FormatString (L"%S / %S (%S)", { L"A", L"B", L"C" }) == L"A / B (C)");
	ASSERT (FormatString (L"Add To Group \"%S\"", { L"Group Name" }) == L"Add To Group \"Group Name\"");

	ASSERT (FormatString (L"\u03c0 unicode %S unicode \u03c0", { L"\u03c0" }) == L"\u03c0 unicode \u03c0 unicode \u03c0");
	ASSERT (FormatString (L"\u03c0 unicode %S unicode \u03c0", { L"\u03c0 unicode \u03c0" }) == L"\u03c0 unicode \u03c0 unicode \u03c0 unicode \u03c0");
}

}
