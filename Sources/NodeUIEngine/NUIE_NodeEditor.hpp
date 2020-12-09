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
#include "NUIE_NodeEditorInfo.hpp"

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

	void						OnMouseDown (const ModifierKeys& keys, MouseButton button, int posX, int posY);
	void						OnMouseUp (const ModifierKeys& keys, MouseButton button, int posX, int posY);
	void						OnMouseMove (const ModifierKeys& keys, int posX, int posY);
	void						OnMouseWheel (const ModifierKeys& keys, MouseWheelRotation rotation, int posX, int posY);
	void						OnMouseSwipe (const ModifierKeys& keys, int offsetX, int offsetY);
	void						OnMouseDoubleClick (const ModifierKeys& keys, MouseButton button, int posX, int posY);
	void						OnContextMenuRequest (int posX, int posY);
	void						OnResize (int newWidth, int newHeight);

	UpdateMode					GetUpdateMode () const;
	void						SetUpdateMode (UpdateMode newUpdateMode);
	void						ManualUpdate ();

	void						Update ();
	void						Draw ();

	void						AddNode (const UINodePtr& uiNode);
	void						ConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot);
	void						InvalidateAllDrawings ();

	Point						ViewToModel (const Point& viewPoint) const;
	void						AlignToWindow ();
	void						CenterToWindow ();
	void						FitToWindow ();

	const ViewBox&				GetViewBox () const;
	void						SetViewBox (const ViewBox& newViewBox);

	const Selection&			GetSelection () const;
	void						SetSelection (const Selection& newSelection);

	void						New ();
	bool						Open (const std::wstring& fileName);
	bool						Open (NE::InputStream& inputStream);
	bool						Save (const std::wstring& fileName);
	bool						Save (NE::OutputStream& outputStream);
	bool						NeedToSave () const;

	void						ExecuteCommand (CommandCode command);
	void						ExecuteMenuCommand (const MenuCommandPtr& command);
	void						ApplyParameterChanges (const ParameterInterfacePtr& parameters);

	void						Undo ();
	void						Redo ();

	NodeEditorInfo				GetInfo () const;

private:
	NodeUIManager				uiManager;
	InteractionHandler			interactionHandler;
	MouseEventTranslator		mouseEventTranslator;
	NodeUIEnvironment&			uiEnvironment;
};

}

#endif
