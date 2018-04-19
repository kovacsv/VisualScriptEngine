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

class PastePositionCalculator
{
public:
	PastePositionCalculator ();

	Point	CalculatePastePosition (NodeUIManager& uiManager, NodeUIEnvironment& env);

private:
	Point	lastPastePosition;
	int		samePositionPasteCounter;
};

class NodeInputEventHandler : public InputEventHandler
{
public:
	NodeInputEventHandler (NodeUIManager& uiManager);

	virtual EventHandlerResult	HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position) override;
	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position) override;
	virtual EventHandlerResult	HandleKeyPress (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Key& pressedKey) override;

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

	virtual EventHandlerResult		HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position) override;
	virtual EventHandlerResult		HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position) override;
	virtual EventHandlerResult		HandleKeyPress (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Key& pressedKey) override;

private:
	NodeUIManager&					uiManager;
	MultiMouseMoveHandler			multiMouseMoveHandler;
	NodeInputEventHandler			nodeInputEventHandler;
	PastePositionCalculator			pastePositionCalculator;
};

}

#endif
