#ifndef WAS_WINDOWSAPPUTILS_HPP
#define WAS_WINDOWSAPPUTILS_HPP

#include "WAS_IncludeWindowsHeaders.hpp"

#include "NE_StringConverter.hpp"
#include "NUIE_Geometry.hpp"
#include "NUIE_MenuCommands.hpp"
#include "NUIE_InputEventHandler.hpp"

namespace WAS
{

void						SetWindowCapture (HWND hwnd);
void						ReleaseWindowCapture (HWND hwnd);

NE::BasicStringSettings		GetStringSettingsFromSystem ();
NUIE::ModifierKeys			GetModiferKeysFromEvent (WPARAM wParam);
NUIE::MenuCommandPtr		SelectCommandFromContextMenu (HWND hwnd, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands);

struct FileFilter
{
	std::wstring fileType;
	std::wstring fileExtension;
};

FileFilter					GetDefaultFileFilter ();
bool						OpenFileDialog (HWND hwnd, const FileFilter& filter, std::wstring& selectedFileName);
bool						SaveFileDialog (HWND hwnd, const FileFilter& filter, std::wstring& selectedFileName);

std::wstring				GetResourceFileContentUtf8BOM (LPCWSTR resourceName, LPCWSTR resourceType);

}

#endif
