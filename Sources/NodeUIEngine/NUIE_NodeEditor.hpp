#ifndef NUIE_NODEEDITOR_HPP
#define NUIE_NODEEDITOR_HPP

#include "NUIE_DrawingContext.hpp"
#include "NUIE_EventHandler.hpp"
#include "NUIE_ClipboardHandler.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_InteractionHandler.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_FileIO.hpp"

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
	void						ConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);

	Point						ViewToModel (const Point& viewPoint) const;
	void						AlignToWindow ();
	void						CenterToWindow ();
	void						FitToWindow ();

	const ViewBox&				GetViewBox () const;
	void						SetViewBox (const ViewBox& newViewBox);

	const NE::NodeCollection&	GetSelectedNodes () const;
	void						SetSelectedNodes (const NE::NodeCollection& newSelectedNodes);

	void						New ();
	bool						Open (const std::wstring& fileName);
	bool						Open (NE::InputStream& inputStream);
	bool						Save (const std::wstring& fileName);
	bool						Save (NE::OutputStream& outputStream);
	bool						NeedToSave () const;

	void						ExecuteMenuCommand (const MenuCommandPtr& command);
	void						ApplyParameterChanges (const ParameterInterfacePtr& parameters);

	void						SetSelectedNodesParameters ();
	void						GroupSelectedNodes ();

	void						Undo ();
	void						Redo ();

private:
	NodeUIManager				uiManager;
	InteractionHandler			interactionHandler;
	MouseEventTranslator		mouseEventTranslator;
	NodeUIEnvironment&			uiEnvironment;
};

}

#endif
