#ifndef UNDOHANDLER_HPP
#define UNDOHANDLER_HPP

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
	bool	Undo (NE::NodeManager& targetNodeManager, NE::MergeEventHandler& eventHandler);
	bool	Redo (NE::NodeManager& targetNodeManager, NE::MergeEventHandler& eventHandler);

private:
	size_t											undoPosition;
	std::vector<std::shared_ptr<NE::NodeManager>>	undoQueue;
};

}

#endif
