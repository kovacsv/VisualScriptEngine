#include "SimpleTest.hpp"
#include "NUIE_Localization.hpp"

using namespace NE;
using namespace NUIE;

namespace LocalizationTest
{

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

}
