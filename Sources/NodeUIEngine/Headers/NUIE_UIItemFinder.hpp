#ifndef UIITEMFINDER_HPP
#define UIITEMFINDER_HPP

#include "NUIE_NodeUIManager.hpp"

namespace NUIE
{

UINodePtr			FindNodeUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition);
UINodeGroupPtr		FindNodeGroupUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition);
UIInputSlotPtr		FindInputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition);
UIOutputSlotPtr		FindOutputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition);

bool FindItemUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition,
							const std::function<void (UINodePtr&)>& nodeFound,
							const std::function<void (UIOutputSlotPtr&)>& outputSlotFound,
							const std::function<void (UIInputSlotPtr&)>& inputSlotFound,
							const std::function<void (UINodeGroupPtr&)>& nodeGroupFound);

}

#endif
