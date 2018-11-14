#ifndef NUIE_EVENTHANDLERS_HPP
#define NUIE_EVENTHANDLERS_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_UINodeGroup.hpp"
#include "NUIE_CommandStructure.hpp"
#include "NUIE_ParameterInterface.hpp"
#include <vector>

namespace NUIE
{

class NodeUIManager;

class EventHandlers
{
public:
	EventHandlers ();
	virtual ~EventHandlers ();

	virtual NUIE::UICommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UICommandStructure& commands) = 0;
	virtual NUIE::UICommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UINodePtr& uiNode, const UICommandStructure& commands) = 0;
	virtual NUIE::UICommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UIOutputSlotPtr& inputSlot, const UICommandStructure& commands) = 0;
	virtual NUIE::UICommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UIInputSlotPtr& inputSlot, const UICommandStructure& commands) = 0;
	virtual NUIE::UICommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UINodeGroupPtr& group, const UICommandStructure& commands) = 0;

	virtual bool				OnParameterSettings (ParameterInterfacePtr paramAccessor) = 0;
};

}

#endif
