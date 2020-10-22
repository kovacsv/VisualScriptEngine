#ifndef NUIE_INTERACTIONHANDLER_HPP
#define NUIE_INTERACTIONHANDLER_HPP

#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NUIE_EventHandler.hpp"
#include "NUIE_MenuCommands.hpp"
#include "NUIE_UIEventHandlers.hpp"
#include <memory>

namespace NUIE
{

class NodeUIManager;

enum class CommandCode
{
	Undefined,
	Escape,
	SelectAll,
	SetParameters,
	Delete,
	Copy,
	Paste,
	Group,
	Ungroup,
	Undo,
	Redo
};

class PastePositionCalculator
{
public:
	PastePositionCalculator ();

	void	Clear ();

	Point	CalculatePastePosition (const NodeUIManager& uiManager, NodeUIEnvironment& env);

private:
	Point	lastPastePosition;
	int		samePositionPasteCounter;
};

class NodeInputEventHandler : public InputEventHandler
{
public:
	NodeInputEventHandler (NodeUIManager& uiManager, UINodePtr& uiNode);

	virtual EventHandlerResult	HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position) override;
	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDoubleClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position) override;
	virtual EventHandlerResult	HandleKeyPress (NodeUIEnvironment& env, KeyCode pressedKey) override;

private:
	EventHandlerResult			ForwardEventToNode (const std::function<EventHandlerResult ()>& forwardEvent);

	NodeUIManager&				uiManager;
	UINodePtr&					uiNode;
};

class InteractionHandler : public InputEventHandler
{
public:
	InteractionHandler (NodeUIManager& uiManager);
	~InteractionHandler ();

	void							Clear ();

	const NodeDrawingModifier*		GetDrawingModifier ();
	void							ExecuteCommand (NodeUIEnvironment& uiEnvironment, NUIE::CommandCode command);

	virtual EventHandlerResult		HandleMouseDragStart (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDragStop (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDrag (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, const Point& position) override;
	virtual EventHandlerResult		HandleMouseClick (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDoubleClick (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseWheel (NodeUIEnvironment& uiEnvironment, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position) override;
	virtual EventHandlerResult		HandleKeyPress (NodeUIEnvironment& uiEnvironment, KeyCode pressedKey) override;

private:
	NodeUIManager&					uiManager;
	MultiMouseMoveHandler			multiMouseMoveHandler;
	PastePositionCalculator			pastePositionCalculator;
};

}

#endif
