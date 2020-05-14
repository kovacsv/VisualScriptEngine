#include "NUIE_UndoHandler.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

static void SaveAndAddState (const NE::NodeManager& nodeManager, std::vector<std::shared_ptr<NE::NodeManager>>& stack)
{
	std::shared_ptr<NE::NodeManager> undoState (new NE::NodeManager ());
	NE::NodeManager::Clone (nodeManager, *undoState.get ());
	stack.push_back (undoState);
}

UndoHandler::UndoHandler ()
{

}

void UndoHandler::SaveUndoState (const NE::NodeManager& nodeManager)
{
	redoStack.clear ();
	SaveAndAddState (nodeManager, undoStack);
}

bool UndoHandler::Undo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler)
{
	if (undoStack.empty ()) {
		return false;
	}

	SaveAndAddState (targetNodeManager, redoStack);

	std::shared_ptr<NE::NodeManager> undoState = undoStack.back ();
	undoStack.pop_back ();

	return NE::NodeManagerMerge::UpdateNodeManager (*undoState.get (), targetNodeManager, eventHandler);
}

bool UndoHandler::Redo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler)
{
	if (redoStack.empty ()) {
		return false;
	}

	SaveAndAddState (targetNodeManager, undoStack);
	std::shared_ptr<NE::NodeManager> redoState = redoStack.back ();
	redoStack.pop_back ();
	return NE::NodeManagerMerge::UpdateNodeManager (*redoState.get (), targetNodeManager, eventHandler);
}

void UndoHandler::Clear ()
{
	undoStack.clear ();
	redoStack.clear ();
}

}
