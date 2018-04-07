#ifndef UINODECOMMANDSTRUCTURE_HPP
#define UINODECOMMANDSTRUCTURE_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_CommandStructure.hpp"
#include "NUIE_NodeUIManager.hpp"

namespace NUIE
{

NodeCollection		GetNodesForCommand (const NodeUIManager& uiManager, const UINodePtr& uiNode);

CommandStructure	CreateEmptyAreaCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position);
CommandStructure	CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode);
CommandStructure	CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot);
CommandStructure	CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot);

}

#endif
