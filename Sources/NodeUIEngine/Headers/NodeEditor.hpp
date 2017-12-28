#ifndef NODEEDITOR_HPP
#define NODEEDITOR_HPP

#include "DrawingContext.hpp"
#include "EventHandlers.hpp"
#include "NodeUIManager.hpp"
#include "NodeUIEnvironment.hpp"
#include "SkinParams.hpp"

namespace NUIE
{

class NodeEditorInterface
{
public:
	virtual DrawingContext&			GetDrawingContext () = 0;
	virtual EventHandlers&			GetEventHandlers () = 0;
	virtual NE::EvaluationEnv&		GetEvaluationEnv () = 0;
};

class NodeEditor
{
public:
	NodeEditor (NodeEditorInterface& editorInterface);
	virtual ~NodeEditor ();

	void							OnMouseDown (const KeySet& pressedKeys, MouseButton button, int x, int y);
	void							OnMouseUp (const KeySet& pressedKeys, MouseButton button, int x, int y);
	void							OnMouseMove (const KeySet& pressedKeys, int x, int y);
	void							OnMouseWheel (const KeySet& pressedKeys, MouseWheelRotation rotation, int x, int y);
	void							OnMouseDoubleClick (const KeySet& pressedKeys, MouseButton button, int x, int y);
	void							OnResize (int newWidth, int newHeight);
	void							Update ();
	void							Draw ();

	NodeUIManager&					GetNodeUIManager ();
	NodeUIEnvironment&				GetNodeUIEnvironment ();
	
	void							Clear ();
	bool							Load (const std::wstring& fileName);
	bool							Save (const std::wstring& fileName) const;

protected:
	NodeEditorInterface&	editorInterface;
	SkinParams				skinParams;
	NodeUIManager			uiManager;
	NodeUIEnvironment		uiEnvironment;
};

}

#endif
