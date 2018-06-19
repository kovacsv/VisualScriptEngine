#ifndef EVENTHANDLERS_HPP
#define EVENTHANDLERS_HPP

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

	virtual NUIE::CommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const CommandStructure& commands) = 0;
	virtual NUIE::CommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UINodePtr& uiNode, const CommandStructure& commands) = 0;
	virtual NUIE::CommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UIOutputSlotPtr& inputSlot, const CommandStructure& commands) = 0;
	virtual NUIE::CommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UIInputSlotPtr& inputSlot, const CommandStructure& commands) = 0;
	virtual NUIE::CommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UINodeGroupPtr& group, const CommandStructure& commands) = 0;

	virtual bool				OnParameterSettings (ParameterInterfacePtr paramAccessor) = 0;
};

}

#endif
