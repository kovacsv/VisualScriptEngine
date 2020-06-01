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

NUIE::Version NullClipboardHandler::GetCurrentVersion () const
{
	return NUIE::Version (0, 0, 0);
}

bool NullClipboardHandler::IsCompatibleVersion (const NUIE::Version&) const
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

NUIE::Version MemoryClipboardHandler::GetCurrentVersion () const
{
	return NUIE::GetCurrentVersion ();
}

bool MemoryClipboardHandler::IsCompatibleVersion (const NUIE::Version& version) const
{
	return NUIE::IsCompatibleVersion (version);
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
