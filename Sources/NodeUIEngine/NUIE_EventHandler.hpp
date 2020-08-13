#ifndef NUIE_EVENTHANDLER_HPP
#define NUIE_EVENTHANDLER_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_UINodeGroup.hpp"
#include "NUIE_MenuCommands.hpp"
#include "NUIE_ParameterInterface.hpp"
#include <vector>

namespace NUIE
{

class NodeUIManager;

class EventHandler
{
public:
	EventHandler ();
	virtual ~EventHandler ();

	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const MenuCommandStructure& commands) = 0;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const UINodePtr& uiNode, const MenuCommandStructure& commands) = 0;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const UIOutputSlotConstPtr& outputSlot, const MenuCommandStructure& commands) = 0;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const UIInputSlotConstPtr& inputSlot, const MenuCommandStructure& commands) = 0;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const UINodeGroupPtr& group, const MenuCommandStructure& commands) = 0;
	virtual void					OnDoubleClick (const Point& position, MouseButton mouseButton) = 0;
	virtual bool					OnParameterSettings (ParameterInterfacePtr paramAccessor, const UINodePtr& uiNode) = 0;
	virtual bool					OnParameterSettings (ParameterInterfacePtr paramAccessor, const UINodeGroupPtr& uiGroup) = 0;
};

using EventHandlerPtr = std::shared_ptr<EventHandler>;
using EventHandlerConstPtr = std::shared_ptr<const EventHandler>;

class NullEventHandler : public EventHandler
{
public:
	NullEventHandler ();
	virtual ~NullEventHandler ();

	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const UINodePtr& uiNode, const MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const UIOutputSlotConstPtr& outputSlot, const MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const UIInputSlotConstPtr& inputSlot, const MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const Point& position, const UINodeGroupPtr& group, const MenuCommandStructure& commands) override;
	virtual void					OnDoubleClick (const Point& position, MouseButton mouseButton) override;
	virtual bool					OnParameterSettings (ParameterInterfacePtr paramAccessor, const UINodePtr& uiNode) override;
	virtual bool					OnParameterSettings (ParameterInterfacePtr paramAccessor, const UINodeGroupPtr& uiGroup) override;
};

}

#endif
