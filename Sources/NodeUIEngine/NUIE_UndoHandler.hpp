#ifndef NUIE_UNDOHANDLER_HPP
#define NUIE_UNDOHANDLER_HPP

#include "NE_NodeManager.hpp"
#include "NE_NodeManagerMerge.hpp"

#include <vector>

namespace NUIE
{

class UndoHandler
{
public:
	UndoHandler ();

	void	SaveUndoState (const NE::NodeManager& nodeManager);
	bool	Undo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler);
	bool	Redo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler);
	void	Clear ();

private:
	std::vector<std::shared_ptr<NE::NodeManager>>	undoStack;
	std::vector<std::shared_ptr<NE::NodeManager>>	redoStack;
};

}

#endif
