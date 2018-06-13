#include "NUIE_UndoHandler.hpp"
#include "NE_NodeManagerMerge.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

UndoHandler::UndoHandler ()
{

}

void UndoHandler::StoreState (const NE::NodeManager& nodeManager)
{
	std::shared_ptr<NE::NodeManager> undoState (new NE::NodeManager ());
	NE::NodeManager::Clone (nodeManager, *undoState.get ());
	undoStack.push (undoState);
}

bool UndoHandler::RestoreState (NE::NodeManager& targetNodeManager)
{
	if (undoStack.empty ()) {
		return false ;
	}

	std::shared_ptr<NE::NodeManager> undoState = undoStack.top ();
	undoStack.pop ();

	return NE::NodeManagerMerge::UpdateNodeManager (*undoState.get (), targetNodeManager);
}


}
