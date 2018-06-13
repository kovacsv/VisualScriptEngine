#ifndef UNDOHANDLER_HPP
#define UNDOHANDLER_HPP

#include "NE_NodeManager.hpp"

#include <stack>

namespace NUIE
{

class UndoHandler
{
public:
	UndoHandler ();

	void	StoreState (const NE::NodeManager& nodeManager);
	bool	RestoreState (NE::NodeManager& targetNodeManager);

private:
	std::stack<std::shared_ptr<NE::NodeManager>>	undoStack;
};

}

#endif
