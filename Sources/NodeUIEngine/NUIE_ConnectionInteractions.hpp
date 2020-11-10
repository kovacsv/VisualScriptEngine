#ifndef NUIE_CONNECTIONINTERACTIONS_HPP
#define NUIE_CONNECTIONINTERACTIONS_HPP

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UIEventHandlers.hpp"

namespace NUIE
{

class ConnectionStartOutputSlot
{
public:
	ConnectionStartOutputSlot (const UIOutputSlotConstPtr& slot, const Point& position);

	UIOutputSlotConstPtr	slot;
	Point					position;
};

class ConnectionStartInputSlot
{
public:
	ConnectionStartInputSlot (const UIInputSlotConstPtr& slot, const Point& position);

	UIInputSlotConstPtr		slot;
	Point					position;
};

class NodeOutputToInputConnectionHandlerBase : public MouseMoveHandler
{
public:
	NodeOutputToInputConnectionHandlerBase (NodeUIManager& uiManager);

	virtual void	HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position) override;
	virtual void	HandleAbort () override;

	virtual bool	CanConnectToInputSlot (const UIInputSlotConstPtr& inputSlot) const = 0;

protected:
	NodeUIManager&			uiManager;
	UIInputSlotConstPtr		endSlot;
};

class NodeOutputToInputConnectionHandler : public NodeOutputToInputConnectionHandlerBase
{
public:
	NodeOutputToInputConnectionHandler (NodeUIManager& uiManager, const ConnectionStartOutputSlot& startSlot);

	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const override;
	virtual void	HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&) override;
	virtual bool	CanConnectToInputSlot (const UIInputSlotConstPtr& inputSlot) const override;

private:
	ConnectionStartOutputSlot	startSlot;
};

class NodeOutputToInputReconnectionHandler : public NodeOutputToInputConnectionHandlerBase
{
public:
	NodeOutputToInputReconnectionHandler (NodeUIManager& uiManager, const std::vector<ConnectionStartOutputSlot>& startSlots, const UIInputSlotConstPtr& originalEndSlot);

	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const override;
	virtual bool	NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const override;
	virtual void	HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&) override;
	virtual bool	CanConnectToInputSlot (const UIInputSlotConstPtr& inputSlot) const override;

private:
	std::vector<ConnectionStartOutputSlot>	startSlots;
	UIInputSlotConstPtr						originalEndSlot;
};

class NodeInputToOutputConnectionHandlerBase : public MouseMoveHandler
{
public:
	NodeInputToOutputConnectionHandlerBase (NodeUIManager& uiManager);

	virtual void	HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position) override;
	virtual void	HandleAbort () override;

	virtual bool	CanConnectToOutputSlot (const UIOutputSlotConstPtr& outputSlot) const = 0;

protected:
	NodeUIManager&			uiManager;
	UIOutputSlotConstPtr	endSlot;
};

class NodeInputToOutputConnectionHandler : public NodeInputToOutputConnectionHandlerBase
{
public:
	NodeInputToOutputConnectionHandler (NodeUIManager& uiManager, const ConnectionStartInputSlot& startSlot);

	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const override;
	virtual void	HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&) override;
	virtual bool	CanConnectToOutputSlot (const UIOutputSlotConstPtr& outputSlot) const override;

private:
	ConnectionStartInputSlot	startSlot;
};

class NodeInputToOutputReconnectionHandler : public NodeInputToOutputConnectionHandlerBase
{
public:
	NodeInputToOutputReconnectionHandler (NodeUIManager& uiManager, const std::vector<ConnectionStartInputSlot>& startSlots, const UIOutputSlotConstPtr& originalEndSlot);

	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const override;
	virtual bool	NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const override;
	virtual void	HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&) override;
	virtual bool	CanConnectToOutputSlot (const UIOutputSlotConstPtr& outputSlot) const override;

private:
	std::vector<ConnectionStartInputSlot>	startSlots;
	UIOutputSlotConstPtr					originalEndSlot;
};

}

#endif
