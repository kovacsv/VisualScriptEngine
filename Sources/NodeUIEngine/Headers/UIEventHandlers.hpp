#ifndef UIEVENTHANDLERS_HPP
#define UIEVENTHANDLERS_HPP

#include "Geometry.hpp"
#include "MouseEventHandler.hpp"
#include "NodeUIEnvironment.hpp"
#include "NodeDrawingExtension.hpp"
#include <unordered_map>
#include <memory>
#include <functional>

namespace NUIE
{

class MouseMoveHandler : public NodeDrawingExtension
{
public:
	MouseMoveHandler ();
	virtual ~MouseMoveHandler ();

	void			OnMouseDown (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position);
	void			OnMouseMove (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position);
	void			OnMouseUp (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position);

	virtual bool	AreOtherHandlersAllowed () const;

	virtual void	EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const override;
	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&)>&) const override;

protected:
	virtual void	HandleMouseDown (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position);
	virtual void	HandleMouseMove (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position);
	virtual void	HandleMouseUp (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position);

	Point	startPosition;
	Point	prevPosition;
};

class MultiMouseMoveHandler : public NodeDrawingExtension
{
public:
	MultiMouseMoveHandler ();

	bool								HasHandler (MouseButton mouseButton) const;
	std::shared_ptr<MouseMoveHandler>&	GetHandler (MouseButton mouseButton);
	bool								AreOtherHandlersAllowed () const;

	void								AddHandler (MouseButton mouseButton, MouseMoveHandler* newHandler);
	void								RemoveHandler (MouseButton mouseButton);
	void								EnumerateHandlers (const std::function<void (std::shared_ptr<MouseMoveHandler>&)>& processor);
	bool								HasHandler () const;

	virtual void						EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const override;
	virtual void						EnumerateTemporaryConnections (const std::function<void (const Point& beg, const Point& end)>& processor) const override;

private:
	std::unordered_map<MouseButton, std::shared_ptr<MouseMoveHandler>> handlers;
};

}

#endif
