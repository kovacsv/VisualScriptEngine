#ifndef NUIE_UIITEMFINDER_HPP
#define NUIE_UIITEMFINDER_HPP

#include "NUIE_NodeUIManager.hpp"

namespace NUIE
{

UINodePtr				FindNodeUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition);
UINodeGroupPtr			FindNodeGroupUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition);
UIInputSlotConstPtr		FindInputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition);
UIOutputSlotConstPtr	FindOutputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition);

bool FindItemUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition,
							const std::function<void (const UIInputSlotConstPtr&)>& inputSlotFound,
							const std::function<void (const UIOutputSlotConstPtr&)>& outputSlotFound,
							const std::function<void (const UINodePtr&)>& nodeFound,
							const std::function<void (const UINodeGroupPtr&)>& nodeGroupFound);

}

#endif
