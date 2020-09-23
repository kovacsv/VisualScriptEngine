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
	enum class ContextMenuType
	{
		EmptyArea,
		Node,
		Group,
		InputSlot,
		OutputSlot
	};

	enum class ParameterSettingsType
	{
		Node,
		Group
	};

	EventHandler ();
	virtual ~EventHandler ();

	virtual NUIE::MenuCommandPtr	OnContextMenu (ContextMenuType type, const Point& position, const MenuCommandStructure& commands) = 0;
	virtual bool					OnParameterSettings (ParameterSettingsType, ParameterInterfacePtr paramAccessor) = 0;
	virtual void					OnDoubleClick (const Point& position, MouseButton mouseButton) = 0;
};

using EventHandlerPtr = std::shared_ptr<EventHandler>;
using EventHandlerConstPtr = std::shared_ptr<const EventHandler>;

class NullEventHandler : public EventHandler
{
public:
	NullEventHandler ();
	virtual ~NullEventHandler ();

	virtual NUIE::MenuCommandPtr	OnContextMenu (ContextMenuType type, const Point& position, const MenuCommandStructure& commands) override;
	virtual bool					OnParameterSettings (ParameterSettingsType type, ParameterInterfacePtr paramAccessor) override;
	virtual void					OnDoubleClick (const Point& position, MouseButton mouseButton) override;
};

}

#endif
