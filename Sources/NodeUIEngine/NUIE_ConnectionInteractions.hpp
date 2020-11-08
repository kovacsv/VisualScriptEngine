#ifndef NUIE_CONNECTIONINTERACTIONS_HPP
#define NUIE_CONNECTIONINTERACTIONS_HPP

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UIEventHandlers.hpp"

namespace NUIE
{

template <class StartSlotType, class EndSlotType>
class NodeConnectionHandler : public MouseMoveHandler
{
public:
	NodeConnectionHandler (NodeUIManager& uiManager, const StartSlotType& startSlot, const Point& startSlotPosition) :
		MouseMoveHandler (),
		uiManager (uiManager),
		startSlot (startSlot),
		endSlot (nullptr),
		startSlotPosition (startSlotPosition)
	{
	
	}

	virtual void HandleAbort () override
	{
		uiManager.RequestRedraw ();
	}

protected:
	NodeUIManager&	uiManager;
	StartSlotType	startSlot;
	EndSlotType		endSlot;
	Point			startSlotPosition;
};

class NodeOutputToInputConnectionHandler : public NodeConnectionHandler<UIOutputSlotConstPtr, UIInputSlotConstPtr>
{
public:
	NodeOutputToInputConnectionHandler (NodeUIManager& uiManager, const UIOutputSlotConstPtr& startSlot, const Point& startSlotPosition);

	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const override;
	virtual void	HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position) override;
	virtual void	HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&) override;
	virtual bool	CanConnectToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;
};

class NodeOutputToInputReconnectionHandler : public NodeOutputToInputConnectionHandler
{
public:
	NodeOutputToInputReconnectionHandler (NodeUIManager& uiManager, const UIOutputSlotConstPtr& startSlot, const UIInputSlotConstPtr& originalEndSlot, const Point& startSlotPosition);

	virtual bool	NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const override;
	virtual void	HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&) override;
	virtual bool	CanConnectToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const override;

private:
	UIInputSlotConstPtr originalEndSlot;
};

class NodeInputToOutputConnectionHandler : public NodeConnectionHandler<UIInputSlotConstPtr, UIOutputSlotConstPtr>
{
public:
	NodeInputToOutputConnectionHandler (NodeUIManager& uiManager, const UIInputSlotConstPtr& startSlot, const Point& startSlotPosition);

	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end, Direction dir)>& processor) const override;
	virtual void	HandleMouseMove (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point& position) override;
	virtual void	HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&) override;
	virtual bool	CanConnectToOutputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const;
};

class NodeInputToOutputReconnectionHandler : public NodeInputToOutputConnectionHandler
{
public:
	NodeInputToOutputReconnectionHandler (NodeUIManager& uiManager, const UIInputSlotConstPtr& startSlot, const UIOutputSlotConstPtr& originalEndSlot, const Point& startSlotPosition);

	virtual bool	NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const override;
	virtual void	HandleMouseUp (NodeUIEnvironment& uiEnvironment, const ModifierKeys&, const Point&) override;
	virtual bool	CanConnectToOutputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot) const override;

private:
	UIOutputSlotConstPtr originalEndSlot;
};


}

#endif
