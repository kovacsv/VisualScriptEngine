#include "WAS_ClipboardHandler.hpp"
#include "NUIE_VersionInfo.hpp"
#include "NUIE_VersionCompatibility.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

ClipboardHandler::ClipboardHandler () :
	NUIE::ClipboardHandler (),
	formatId (0)
{
	formatId = RegisterClipboardFormat (VSE_LIB_NAME);
}

ClipboardHandler::~ClipboardHandler ()
{

}

NUIE::Version ClipboardHandler::GetCurrentVersion () const
{
	return NUIE::GetCurrentEngineVersion ();
}

bool ClipboardHandler::IsCompatibleVersion (const NUIE::Version& version) const
{
	return NUIE::IsCompatibleEngineVersion (version);
}

bool ClipboardHandler::HasClipboardContent () const
{
	if (!OpenClipboard (NULL)) {
		return false;
	}

	HGLOBAL clipboardHandle = GetClipboardData (formatId);
	bool hasClipboardContent = (clipboardHandle != NULL);
	CloseClipboard ();

	return hasClipboardContent;
}

bool ClipboardHandler::GetClipboardContent (std::vector<char>& content) const
{
	if (DBGERROR (!OpenClipboard (NULL))) {
		return false;
	}
	HGLOBAL clipboardHandle = GetClipboardData (formatId);
	if (clipboardHandle == NULL) {
		CloseClipboard ();
		return false;
	}

	SIZE_T handleSize = GlobalSize (clipboardHandle);
	if (handleSize == 0) {
		return false;
	}

	char* pointer = (char*) GlobalLock (clipboardHandle);
	content.assign (pointer, pointer + handleSize);
	GlobalUnlock (clipboardHandle);
	CloseClipboard ();

	return true;
}

bool ClipboardHandler::SetClipboardContent (const std::vector<char>& content)
{
	if (DBGERROR (content.empty ())) {
		return false;
	}

	if (DBGERROR (!OpenClipboard (NULL))) {
		return false;
	}

	EmptyClipboard ();

	HGLOBAL clipboardHandle = GlobalAlloc (GMEM_MOVEABLE, content.size ());
	LPVOID pointer = GlobalLock (clipboardHandle);
	memcpy (pointer, content.data (), content.size ());
	GlobalUnlock (clipboardHandle);

	HANDLE resultData = SetClipboardData (formatId, clipboardHandle);
	if (DBGERROR (resultData == NULL)) {
		CloseClipboard ();
		return false;
	}

	CloseClipboard ();
	return true;
}

}
