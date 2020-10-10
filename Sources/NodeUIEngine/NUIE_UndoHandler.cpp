#include "NUIE_UndoHandler.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

static void AddStateToStack (const NE::NodeManager& nodeManager, std::vector<std::shared_ptr<NE::NodeManager>>& stack)
{
	std::shared_ptr<NE::NodeManager> undoState (new NE::NodeManager ());
	NE::NodeManager::Clone (nodeManager, *undoState.get ());
	stack.push_back (undoState);
}

UndoHandler::UndoHandler ()
{

}

bool UndoHandler::CanUndo () const
{
	return !undoStack.empty ();
}

bool UndoHandler::CanRedo () const
{
	return !redoStack.empty ();
}

UndoHandler::ChangeResult UndoHandler::AddUndoStep (const NE::NodeManager& nodeManager)
{
	redoStack.clear ();
	AddStateToStack (nodeManager, undoStack);
	return UndoHandler::ChangeResult::Changed;
}

UndoHandler::ChangeResult UndoHandler::Undo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler)
{
	if (!CanUndo ()) {
		return ChangeResult::NotChanged;
	}

	AddStateToStack (targetNodeManager, redoStack);

	std::shared_ptr<NE::NodeManager> undoState = undoStack.back ();
	undoStack.pop_back ();

	bool success = NE::NodeManagerMerge::UpdateNodeManager (*undoState.get (), targetNodeManager, eventHandler);
	if (DBGERROR (!success)) {
		return ChangeResult::NotChanged;
	}

	return ChangeResult::Changed;
}

UndoHandler::ChangeResult UndoHandler::Redo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler)
{
	if (!CanRedo ()) {
		return ChangeResult::NotChanged;
	}

	AddStateToStack (targetNodeManager, undoStack);

	std::shared_ptr<NE::NodeManager> redoState = redoStack.back ();
	redoStack.pop_back ();

	bool success = NE::NodeManagerMerge::UpdateNodeManager (*redoState.get (), targetNodeManager, eventHandler);
	if (DBGERROR (!success)) {
		return ChangeResult::NotChanged;
	}

	return ChangeResult::Changed;
}

UndoHandler::ChangeResult UndoHandler::Clear ()
{
	ChangeResult result = ChangeResult::NotChanged;
	if (!undoStack.empty () || !redoStack.empty ()) {
		undoStack.clear ();
		redoStack.clear ();
		result = ChangeResult::Changed;
	}
	return result;
}

}
