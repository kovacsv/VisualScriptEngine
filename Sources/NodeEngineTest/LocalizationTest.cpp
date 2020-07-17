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
		msgid "this is an %ls"
		msgstr "ez itt egy %ls"
	)";
	Dictionary dictionary;
	PoDictionarySource poDictionarySource (poContent);
	ASSERT (FillDictionary (dictionary, poDictionarySource));
	ASSERT (dictionary.GetLocalizedString (L"apple") == L"alma");
	ASSERT (FormatString (dictionary.GetLocalizedString (L"this is an %ls"), dictionary.GetLocalizedString (L"apple").c_str ()) == L"ez itt egy alma");
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

}
