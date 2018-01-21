#ifndef NODEUIINTERACTIONHANDLER_HPP
#define NODEUIINTERACTIONHANDLER_HPP

#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "EventHandlers.hpp"
#include "CommandStructure.hpp"
#include "UIEventHandlers.hpp"
#include <memory>

namespace NUIE
{

class NodeUIManager;

class NodeMouseEventHandler : public MouseEventHandler
{
public:
	NodeMouseEventHandler (NodeUIManager& uiManager);

	virtual EventHandlerResult	HandleMouseDragStart (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDragStop (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDrag (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position) override;
	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseWheel (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseWheelRotation rotation, const Point& position) override;

private:
	NodeUIManager&	uiManager;
	UINodePtr		currentNode;
};

class NodeUIInteractionHandler : public MouseEventHandler
{
public:
	NodeUIInteractionHandler (NodeUIManager& uiManager);
	~NodeUIInteractionHandler ();

	const NodeDrawingExtension*		GetDrawingExtension ();

private:
	virtual EventHandlerResult		HandleMouseDragStart (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDragStop (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDrag (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position) override;
	virtual EventHandlerResult		HandleMouseClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseWheel (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseWheelRotation rotation, const Point& position) override;

	NodeUIManager&					uiManager;
	MultiMouseMoveHandler			multiMouseMoveHandler;
	NodeMouseEventHandler			nodeMouseEventHandler;
};

}

#endif
