#ifndef NODEEDITOR_HPP
#define NODEEDITOR_HPP

#include "DrawingContext.hpp"
#include "EventHandlers.hpp"
#include "NodeUIManager.hpp"
#include "NodeUIInteractionHandler.hpp"
#include "NodeUIEnvironment.hpp"
#include "UINodeParameters.hpp"
#include "SkinParams.hpp"

namespace NUIE
{

class NodeParameterAccessor
{
public:
	NodeParameterAccessor ();
	virtual ~NodeParameterAccessor ();

	virtual size_t					GetParameterCount () const = 0;
	virtual const std::wstring&		GetParameterName (size_t index) const = 0;
	virtual NE::ValuePtr			GetParameterValue (size_t index) const = 0;
	virtual bool					SetParameterValue (size_t index, const NE::ValuePtr& value) = 0;
};

typedef std::shared_ptr<NodeParameterAccessor> NodeParameterAccessorPtr;
typedef std::shared_ptr<const NodeParameterAccessor> NodeParameterAccessorConstPtr;

class NodeEditor
{
public:
	NodeEditor (NodeUIEnvironment& uiEnvironment);
	virtual ~NodeEditor ();

	void						OnMouseDown (const KeySet& pressedKeys, MouseButton button, int x, int y);
	void						OnMouseUp (const KeySet& pressedKeys, MouseButton button, int x, int y);
	void						OnMouseMove (const KeySet& pressedKeys, int x, int y);
	void						OnMouseWheel (const KeySet& pressedKeys, MouseWheelRotation rotation, int x, int y);
	void						OnMouseDoubleClick (const KeySet& pressedKeys, MouseButton button, int x, int y);
	void						OnResize (int newWidth, int newHeight);
	
	void						InvalidateAllNodesDrawing ();
	void						Update ();
	void						Draw ();

	NodeParameterAccessorPtr	GetSelectionParameters ();
	NodeUIManager&				GetNodeUIManager ();
	
	void						Clear ();
	bool						Load (const std::wstring& fileName);
	bool						Save (const std::wstring& fileName) const;

protected:
	NodeUIManager				uiManager;
	NodeUIInteractionHandler	uiInteractionHandler;
	MouseEventTranslator		mouseEventTranslator;
	NodeUIEnvironment&			uiEnvironment;
};

}

#endif
