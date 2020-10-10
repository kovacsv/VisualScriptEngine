#include "NUIE_ClipboardState.hpp"

namespace NUIE
{

ClipboardState::ClipboardState (bool hasContent) :
	hasContent (hasContent)
{

}

bool ClipboardState::HasContent () const
{
	return hasContent;
}

}
