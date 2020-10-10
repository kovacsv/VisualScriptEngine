#include "NUIE_UndoState.hpp"

namespace NUIE
{

UndoState::UndoState (bool canUndo, bool canRedo) :
	canUndo (canUndo),
	canRedo (canRedo)
{

}

bool UndoState::CanUndo () const
{
	return canUndo;
}

bool UndoState::CanRedo () const
{
	return canRedo;
}

}
