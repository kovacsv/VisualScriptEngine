#ifndef NODEUIINTERACTIONHANDLER_HPP
#define NODEUIINTERACTIONHANDLER_HPP

#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NUIE_EventHandlers.hpp"
#include "NUIE_CommandStructure.hpp"
#include "NUIE_UIEventHandlers.hpp"
#include <memory>

namespace NUIE
{

class NodeUIManager;

class NodeInputEventHandler : public InputEventHandler
{
public:
	NodeInputEventHandler (NodeUIManager& uiManager);

	virtual EventHandlerResult	HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, const Point& position) override;
	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseWheelRotation rotation, const Point& position) override;

private:
	NodeUIManager&	uiManager;
	UINodePtr		currentNode;
};

class NodeUIInteractionHandler : public InputEventHandler
{
public:
	NodeUIInteractionHandler (NodeUIManager& uiManager);
	~NodeUIInteractionHandler ();

	const NodeDrawingModfier*		GetDrawingModifier ();

private:
	virtual EventHandlerResult		HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, const Point& position) override;
	virtual EventHandlerResult		HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseWheelRotation rotation, const Point& position) override;

	NodeUIManager&					uiManager;
	MultiMouseMoveHandler			multiMouseMoveHandler;
	NodeInputEventHandler			nodeInputEventHandler;
};

}

#endif
