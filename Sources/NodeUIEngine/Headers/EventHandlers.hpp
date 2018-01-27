#ifndef EVENTHANDLERS_HPP
#define EVENTHANDLERS_HPP

#include "UINode.hpp"
#include "CommandStructure.hpp"
#include "UINodeParameterAccessor.hpp"
#include <vector>

namespace NUIE
{

class NodeUIManager;

class EventHandlers
{
public:
	virtual NUIE::CommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const CommandStructure& commands) = 0;
	virtual NUIE::CommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UINodePtr& uiNode, const CommandStructure& commands) = 0;
	virtual NUIE::CommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const NE::OutputSlotPtr& inputSlot, const CommandStructure& commands) = 0;
	virtual NUIE::CommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const NE::InputSlotPtr& inputSlot, const CommandStructure& commands) = 0;

	virtual bool				OnParameterSettings (NodeParameterAccessorPtr paramAccessor) = 0;
};

}

#endif
