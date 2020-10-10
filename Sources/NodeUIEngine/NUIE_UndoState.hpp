#ifndef NUIE_UNDOSTATE_HPP
#define NUIE_UNDOSTATE_HPP

namespace NUIE
{

class UndoState
{
public:
	UndoState (bool canUndo, bool canRedo);

	bool	CanUndo () const;
	bool	CanRedo () const;

private:
	bool	canUndo;
	bool	canRedo;
};

}

#endif
