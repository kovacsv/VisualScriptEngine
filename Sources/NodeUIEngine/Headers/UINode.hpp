#ifndef UINODE_HPP
#define UINODE_HPP

#include "Node.hpp"
#include "NodeDrawingImage.hpp"
#include "MouseEventHandler.hpp"
#include "UIInputSlot.hpp"
#include "UIOutputSlot.hpp"
#include <string>

namespace NUIE
{

class NodeUIManager;
class NodeUIEnvironment;
class NodeCommandRegistrator;

class UINode :	public NE::Node,
				public MouseEventHandler
{
	SERIALIZABLE;

public:
	enum class Status
	{
		HasValue,
		HasNoValue
	};

	UINode ();
	UINode (const std::wstring& nodeName, const Point& nodePosition);
	virtual ~UINode ();

	const std::wstring&			GetNodeName () const;
	void						SetNodeName (const std::wstring& newNodeName);

	const Point&				GetNodePosition () const;
	void						SetNodePosition (const Point& newPosition);

	void						Draw (NodeUIEnvironment& env) const;

	Status						GetStatus (NE::EvaluationEnv& env) const;
	Rect						GetNodeRect (NodeUIEnvironment& env) const;
	void						InvalidateDrawing () const;

	Point						GetInputSlotConnPosition (NodeUIEnvironment& env, const NE::SlotId& slotId) const;
	Point						GetOutputSlotConnPosition (NodeUIEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasInputSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId) const;
	Rect						GetInputSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasOutputSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId) const;
	Rect						GetOutputSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasSpecialRect (NodeUIEnvironment& env, const std::string& rectId) const;
	Rect						GetSpecialRect (NodeUIEnvironment& env, const std::string& rectId) const;

	UIInputSlotPtr				GetUIInputSlot (const NE::SlotId& slotId);
	UIOutputSlotPtr				GetUIOutputSlot (const NE::SlotId& slotId);

	UIInputSlotConstPtr			GetUIInputSlot (const NE::SlotId& slotId) const;
	UIOutputSlotConstPtr		GetUIOutputSlot (const NE::SlotId& slotId) const;

	void						EnumerateUIInputSlots (const std::function<bool (const UIInputSlotPtr&)>& processor);
	void						EnumerateUIOutputSlots (const std::function<bool (const UIOutputSlotPtr&)>& processor);

	void						EnumerateUIInputSlots (const std::function<bool (const UIInputSlotConstPtr&)>& processor) const;
	void						EnumerateUIOutputSlots (const std::function<bool (const UIOutputSlotConstPtr&)>& processor) const;

	virtual EventHandlerResult	HandleMouseDragStart (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDragStop (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDrag (NodeUIEnvironment& env, const KeySet& pressedKeys, const Point& position) override;
	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseWheel (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseWheelRotation rotation, const Point& position) override;

	virtual void				RegisterCommands (NodeCommandRegistrator& commandRegistrator) const;
	virtual void				OnCreated (NE::EvaluationEnv& env) const;
	virtual void				OnDeleted (NE::EvaluationEnv& env) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	template <class SlotType>
	Point GetSlotConnPosition (NodeUIEnvironment& env, const NE::SlotId& slotId);

	template <class SlotType>
	bool HasSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId);

	template <class SlotType>
	Rect GetSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId);

	template <class SlotType>
	void EnumerateUISlots (const std::function<bool (const SlotType&)>& processor);

	template <class SlotConstType>
	void EnumerateUISlots (const std::function<bool (const SlotConstType&)>& processor) const;

protected:
	virtual void				DrawInplace (NodeUIEnvironment& env) const;
	bool						RegisterUIInputSlot (const UIInputSlotPtr& newInputSlot);
	bool						RegisterUIOutputSlot (const UIOutputSlotPtr& newOutputSlot);

private:
	virtual bool				RegisterInputSlot (const NE::InputSlotPtr& newInputSlot) override;
	virtual bool				RegisterOutputSlot (const NE::OutputSlotPtr& newOutputSlot) override;

	const NodeDrawingImage&		GetNodeDrawingImage (NodeUIEnvironment& env) const;
	virtual void				UpdateNodeDrawingImage (NodeUIEnvironment& env, NodeDrawingImage& drawingImage) const = 0;
	virtual void				CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const = 0;

	std::wstring				nodeName;
	Point						nodePosition;
	mutable NodeDrawingImage	nodeDrawingImage;
};

typedef std::shared_ptr<UINode> UINodePtr;
typedef std::shared_ptr<const UINode> UINodeConstPtr;

}

#endif
