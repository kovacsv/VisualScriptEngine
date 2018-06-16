#include "NUIE_UndoHandler.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

UndoHandler::UndoHandler () :
	undoPosition (0)
{

}

void UndoHandler::SaveUndoState (const NE::NodeManager& nodeManager)
{
	std::shared_ptr<NE::NodeManager> undoState (new NE::NodeManager ());
	NE::NodeManager::Clone (nodeManager, *undoState.get ());

	undoQueue.resize (undoPosition);
	undoQueue.push_back (undoState);
	undoPosition = undoQueue.size ();
}

bool UndoHandler::Undo (NE::NodeManager& targetNodeManager, NE::MergeEventHandler& eventHandler)
{
	if (undoQueue.empty () || undoPosition == 0) {
		return false;
	}

	if (undoPosition == undoQueue.size ()) {
		SaveUndoState (targetNodeManager);
		undoPosition = undoPosition - 1;
	}

	undoPosition = undoPosition - 1;
	std::shared_ptr<NE::NodeManager> undoState = undoQueue[undoPosition];
	return NE::NodeManagerMerge::UpdateNodeManager (*undoState.get (), targetNodeManager, eventHandler);
}

bool UndoHandler::Redo (NE::NodeManager& targetNodeManager, NE::MergeEventHandler& eventHandler)
{
	if (undoQueue.empty () || undoPosition >= undoQueue.size () - 1) {
		return false;
	}

	undoPosition = undoPosition + 1;
	std::shared_ptr<NE::NodeManager> undoState = undoQueue[undoPosition];
	return NE::NodeManagerMerge::UpdateNodeManager (*undoState.get (), targetNodeManager, eventHandler);
}

}
