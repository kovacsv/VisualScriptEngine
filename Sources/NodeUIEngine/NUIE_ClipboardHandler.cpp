#include "NUIE_ClipboardHandler.hpp"
#include "NUIE_VersionCompatibility.hpp"

namespace NUIE
{

ClipboardHandler::ClipboardHandler ()
{

}

ClipboardHandler::~ClipboardHandler ()
{

}

NullClipboardHandler::NullClipboardHandler () :
	ClipboardHandler ()
{

}

NullClipboardHandler::~NullClipboardHandler ()
{

}

Version NullClipboardHandler::GetCurrentVersion () const
{
	return Version (0, 0, 0);
}

bool NullClipboardHandler::IsCompatibleVersion (const Version&) const
{
	return false;
}

bool NullClipboardHandler::HasClipboardContent () const
{
	return false;
}

bool NullClipboardHandler::GetClipboardContent (std::vector<char>&) const
{
	return false;
}

bool NullClipboardHandler::SetClipboardContent (const std::vector<char>&)
{
	return false;
}

MemoryClipboardHandler::MemoryClipboardHandler () :
	ClipboardHandler (),
	clipboard ()
{

}

MemoryClipboardHandler::~MemoryClipboardHandler ()
{

}

Version MemoryClipboardHandler::GetCurrentVersion () const
{
	return GetCurrentEngineVersion ();
}

bool MemoryClipboardHandler::IsCompatibleVersion (const Version& version) const
{
	return IsCompatibleEngineVersion (version);
}

bool MemoryClipboardHandler::HasClipboardContent () const
{
	return !clipboard.empty ();
}

bool MemoryClipboardHandler::GetClipboardContent (std::vector<char>& content) const
{
	if (clipboard.empty ()) {
		return false;
	}
	content = clipboard;
	return true;
}

bool MemoryClipboardHandler::SetClipboardContent (const std::vector<char>& content)
{
	clipboard = content;
	return true;
}

}
