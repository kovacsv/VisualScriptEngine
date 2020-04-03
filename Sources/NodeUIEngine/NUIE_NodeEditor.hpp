#ifndef NUIE_NODEEDITOR_HPP
#define NUIE_NODEEDITOR_HPP

#include "NUIE_DrawingContext.hpp"
#include "NUIE_EventHandlers.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_InteractionHandler.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_SkinParams.hpp"

namespace NUIE
{

class ExternalHeaderIO
{
public:
	virtual ~ExternalHeaderIO ();

	virtual bool	Read (NE::InputStream& inputStream) const = 0;
	virtual void	Write (NE::OutputStream& outputStream) const = 0;
};

class ExternalFileIO
{
public:
	virtual ~ExternalFileIO ();

	virtual bool	ReadBufferFromFile (const std::wstring& fileName, std::vector<char>& buffer) const = 0;
	virtual bool	WriteBufferToFile (const std::wstring& fileName, const std::vector<char>& buffer) const = 0;
};

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
	void						AlignToWindow (double scale);
	void						CenterToWindow ();
	void						CenterToWindow (double scale);
	void						FitToWindow ();

	const ViewBox&				GetViewBox () const;
	void						SetViewBox (const ViewBox& newViewBox);

	const NE::NodeCollection&	GetSelectedNodes () const;
	void						SetSelectedNodes (const NE::NodeCollection& newSelectedNodes);

	void						New ();
	bool						Open (const std::wstring& fileName, const ExternalFileIO* externalFileIO, const ExternalHeaderIO* externalHeader);
	bool						Open (NE::InputStream& inputStream, const ExternalHeaderIO* externalHeader);
	bool						Save (const std::wstring& fileName, const ExternalFileIO* externalFileIO, const ExternalHeaderIO* externalHeader) const;
	bool						Save (NE::OutputStream& outputStream, const ExternalHeaderIO* externalHeader) const;
	bool						NeedToSave () const;
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
