#ifndef UINODECOMMANDSTRUCTURE_HPP
#define UINODECOMMANDSTRUCTURE_HPP

#include "UINode.hpp"
#include "CommandStructure.hpp"

namespace NUIE
{

CommandStructure CreateNodeCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UINodePtr& uiNode);
CommandStructure CreateInputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIInputSlotPtr& inputSlot);
CommandStructure CreateOutputSlotCommandStructure (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const UIOutputSlotPtr& outputSlot);

}

#endif
