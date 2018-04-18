#ifndef UINODE_HPP
#define UINODE_HPP

#include "NE_Node.hpp"
#include "NUIE_NodeDrawingImage.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_InputEventHandler.hpp"
#include "NUIE_UIInputSlot.hpp"
#include "NUIE_UIOutputSlot.hpp"
#include <string>

namespace NUIE
{

class NodeUIEnvironment;
class NodeParameterList;
class NodeCommandRegistrator;

class UINode :	public NE::Node
{
	SERIALIZABLE;

public:
	UINode ();
	UINode (const std::wstring& nodeName, const Point& nodePosition);
	virtual ~UINode ();

	const std::wstring&			GetNodeName () const;
	void						SetNodeName (const std::wstring& newNodeName);

	const Point&				GetNodePosition () const;
	void						SetNodePosition (const Point& newPosition);

	void						Draw (NodeUIDrawingEnvironment& env) const;

	Rect						GetNodeRect (NodeUIDrawingEnvironment& env) const;
	void						InvalidateDrawing () const;

	Point						GetInputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;
	Point						GetOutputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;
	Rect						GetInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;
	Rect						GetOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const;
	Rect						GetSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const;

	UIInputSlotPtr				GetUIInputSlot (const NE::SlotId& slotId);
	UIOutputSlotPtr				GetUIOutputSlot (const NE::SlotId& slotId);

	UIInputSlotConstPtr			GetUIInputSlot (const NE::SlotId& slotId) const;
	UIOutputSlotConstPtr		GetUIOutputSlot (const NE::SlotId& slotId) const;

	void						EnumerateUIInputSlots (const std::function<bool (const UIInputSlotPtr&)>& processor);
	void						EnumerateUIOutputSlots (const std::function<bool (const UIOutputSlotPtr&)>& processor);

	void						EnumerateUIInputSlots (const std::function<bool (const UIInputSlotConstPtr&)>& processor) const;
	void						EnumerateUIOutputSlots (const std::function<bool (const UIOutputSlotConstPtr&)>& processor) const;

	virtual EventHandlerResult	HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position);
	virtual EventHandlerResult	HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position);
	virtual EventHandlerResult	HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, const Point& position);
	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseButton mouseButton, const Point& position);
	virtual EventHandlerResult	HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& pressedKeys, MouseWheelRotation rotation, const Point& position);

	virtual void				RegisterParameters (NodeParameterList& parameterList) const;
	virtual void				RegisterCommands (NodeCommandRegistrator& commandRegistrator) const;
	virtual void				OnCreated (NE::EvaluationEnv& env) const;
	virtual void				OnDeleted (NE::EvaluationEnv& env) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	template <class SlotType>
	Point GetSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId);

	template <class SlotType>
	bool HasSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId);

	template <class SlotType>
	Rect GetSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId);

	template <class SlotType>
	void EnumerateUISlots (const std::function<bool (const SlotType&)>& processor);

	template <class SlotConstType>
	void EnumerateUISlots (const std::function<bool (const SlotConstType&)>& processor) const;

protected:
	virtual void				DrawInplace (NodeUIDrawingEnvironment& env) const;
	bool						RegisterUIInputSlot (const UIInputSlotPtr& newInputSlot);
	bool						RegisterUIOutputSlot (const UIOutputSlotPtr& newOutputSlot);

private:
	const NodeDrawingImage&		GetNodeDrawingImage (NodeUIDrawingEnvironment& env) const;
	virtual void				UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const = 0;

	std::wstring				nodeName;
	Point						nodePosition;
	mutable NodeDrawingImage	nodeDrawingImage;
};

typedef std::shared_ptr<UINode> UINodePtr;
typedef std::shared_ptr<const UINode> UINodeConstPtr;

}

#endif
