#ifndef NUIE_NODEEDITOR_HPP
#define NUIE_NODEEDITOR_HPP

#include "NUIE_DrawingContext.hpp"
#include "NUIE_EventHandlers.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeUIInteractionHandler.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_SkinParams.hpp"

namespace NUIE
{

class NodeEditor
{
public:
	enum class UpdateMode
	{
		Automatic,
		Manual
	};

	NodeEditor (NodeUIEnvironment& uiEnvironment);
	virtual ~NodeEditor ();

	void						OnMouseDown (const ModifierKeys& modifierKeys, MouseButton button, int x, int y);
	void						OnMouseUp (const ModifierKeys& modifierKeys, MouseButton button, int x, int y);
	void						OnMouseMove (const ModifierKeys& modifierKeys, int x, int y);
	void						OnMouseWheel (const ModifierKeys& modifierKeys, MouseWheelRotation rotation, int x, int y);
	void						OnMouseDoubleClick (const ModifierKeys& modifierKeys, MouseButton button, int x, int y);
	void						OnKeyPress (const Key& pressedKey);
	void						OnResize (int newWidth, int newHeight);
	
	UpdateMode					GetUpdateMode () const;
	void						SetUpdateMode (UpdateMode newUpdateMode);
	void						ManualUpdate ();

	void						Update ();
	void						Draw ();

	void						AddNode (const UINodePtr& uiNode);
	Point						GetCenterPoint () const;
	
	NodeUIManager&				GetNodeUIManager ();

	void						Clear ();
	bool						Load (const std::wstring& fileName);
	bool						Save (const std::wstring& fileName) const;
	void						Undo ();
	void						Redo ();

protected:
	NodeUIManager				uiManager;
	NodeUIInteractionHandler	uiInteractionHandler;
	MouseEventTranslator		mouseEventTranslator;
	NodeUIEnvironment&			uiEnvironment;
};

}

#endif
