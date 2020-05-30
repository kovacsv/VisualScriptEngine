#include "NUIE_ClipboardHandler.hpp"

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

bool NullClipboardHandler::HasClipboardContent () const
{
	return false;
}

bool NullClipboardHandler::GetClipboardContent (std::vector<char>&) const
{
	return false;
}

void NullClipboardHandler::SetClipboardContent (const std::vector<char>&)
{
	
}

MemoryClipboardHandler::MemoryClipboardHandler () :
	ClipboardHandler (),
	clipboard ()
{

}

MemoryClipboardHandler::~MemoryClipboardHandler ()
{

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

void MemoryClipboardHandler::SetClipboardContent (const std::vector<char>& content)
{
	clipboard = content;
}

}
