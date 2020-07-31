#include "TestReference.hpp"
#include "SimpleTest.hpp"

#include "NE_StringUtils.hpp"
#include "NUIE_FileIO.hpp"

std::wstring GetTestFilesPath ()
{
	return SimpleTest::GetAppFolderLocation () + L"VisualTestFiles" + PATH_SEPARATOR;
}

bool CheckDrawingReference (const SvgDrawingContext& context, const std::wstring& referenceFileName)
{
	std::wstring testFilesPath = GetTestFilesPath ();

	std::wstring referenceFilePath = testFilesPath + referenceFileName;
	std::wstring referenceContent;
	if (!ReadUtf8File (referenceFilePath, referenceContent)) {
		WriteUtf8File (testFilesPath + L"Current_" + referenceFileName, context.GetAsString ());
	}

	std::wstring currentContent = context.GetAsString ();
	referenceContent = NE::ReplaceAll (referenceContent, L"\r\n", L"\n");
	currentContent = NE::ReplaceAll (currentContent, L"\r\n", L"\n");
	if (referenceContent != currentContent) {
		std::wcout << std::endl << L"=== CURRENT ===" << std::endl;
		std::wcout << currentContent << std::endl;
		std::wcout << L"=== REFERENCE ===" << std::endl;
		std::wcout << referenceContent << std::endl;
		WriteUtf8File (testFilesPath + L"Current_" + referenceFileName, context.GetAsString ());
		return false;
	}
	return true;
}