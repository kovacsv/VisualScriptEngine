#ifndef NUIE_UIEVENTHANDLERS_HPP
#define NUIE_UIEVENTHANDLERS_HPP

#include "NUIE_Geometry.hpp"
#include "NUIE_InputEventHandler.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeDrawingModifier.hpp"
#include <unordered_map>
#include <memory>
#include <functional>

namespace NUIE
{

class MouseMoveHandler : public NodeDrawingModifier
{
public:
	MouseMoveHandler ();
	virtual ~MouseMoveHandler ();

	void			OnMouseDown (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position);
	void			OnMouseMove (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position);
	void			OnMouseUp (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position);
	void			OnAbort ();

	virtual bool	AreOtherHandlersAllowed () const;

	virtual void	EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const override;
	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&)>&) const override;
	virtual void	EnumerateDuplicatedNodes (const std::function<void (const NE::NodeId&, const Point&)>& processor) const override;
	virtual bool	NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const override;
	virtual Point	GetNodeOffset (const NE::NodeId& nodeId) const override;

protected:
	virtual void	HandleMouseDown (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position);
	virtual void	HandleMouseMove (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position);
	virtual void	HandleMouseUp (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position);
	virtual void	HandleAbort ();

	Point	startPosition;
	Point	currentPosition;
	Point	prevPosition;
};

class MultiMouseMoveHandler : public NodeDrawingModifier
{
public:
	MultiMouseMoveHandler ();

	void								Clear ();

	bool								HasHandler (MouseButton mouseButton) const;
	std::shared_ptr<MouseMoveHandler>&	GetHandler (MouseButton mouseButton);
	void								AbortHandlers ();
	bool								AreOtherHandlersAllowed () const;

	void								AddHandler (MouseButton mouseButton, MouseMoveHandler* newHandler);
	void								RemoveHandler (MouseButton mouseButton);
	void								EnumerateHandlers (const std::function<void (std::shared_ptr<MouseMoveHandler>&)>& processor);
	bool								HasHandler () const;

	virtual void						EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const override;
	virtual void						EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end)>& processor) const override;
	virtual void						EnumerateDuplicatedNodes (const std::function<void (const NE::NodeId&, const Point&)>& processor) const override;
	virtual bool						NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const override;
	virtual Point						GetNodeOffset (const NE::NodeId& nodeId) const override;

private:
	std::unordered_map<MouseButton, std::shared_ptr<MouseMoveHandler>> handlers;
};

}

#endif
