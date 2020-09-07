#ifndef NUIE_UINODE_HPP
#define NUIE_UINODE_HPP

#include "NE_LocString.hpp"
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

class UINodeCommandInterface
{
public:
	UINodeCommandInterface ();
	virtual ~UINodeCommandInterface ();

	virtual void RunUndoableCommand (const std::function<void ()>& func) = 0;
};

class UINode : public NE::Node
{
	SERIALIZABLE;

public:
	UINode ();
	UINode (const NE::LocString& nodeName, const Point& nodePosition);
	virtual ~UINode ();

	const NE::LocString&		GetName () const;
	void						SetName (const std::wstring& newNodeName);

	const Point&				GetPosition () const;
	void						SetPosition (const Point& newPosition);

	void						Draw (NodeUIDrawingEnvironment& env) const;

	Rect						GetRect (NodeUIDrawingEnvironment& env) const;
	void						InvalidateDrawing () const;

	Point						GetInputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;
	Point						GetOutputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;
	Rect						GetInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;
	Rect						GetOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const;
	Rect						GetSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const;

	UIInputSlotConstPtr			GetUIInputSlot (const NE::SlotId& slotId) const;
	UIOutputSlotConstPtr		GetUIOutputSlot (const NE::SlotId& slotId) const;

	void						EnumerateUIInputSlots (const std::function<bool (const UIInputSlotConstPtr&)>& processor) const;
	void						EnumerateUIOutputSlots (const std::function<bool (const UIOutputSlotConstPtr&)>& processor) const;

	NE::LocString				GetUIInputSlotName (const NE::SlotId& slotId) const;
	void						SetUIInputSlotName (const NE::SlotId& slotId, const std::wstring& newName);

	NE::LocString				GetUIOutputSlotName (const NE::SlotId& slotId) const;
	void						SetUIOutputSlotName (const NE::SlotId& slotId, const std::wstring& newName);

	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position, UINodeCommandInterface& commandInterface);
	virtual EventHandlerResult	HandleMouseDoubleClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position, UINodeCommandInterface& commandInterface);

	virtual void				RegisterParameters (NodeParameterList& parameterList) const;
	virtual void				RegisterCommands (NodeCommandRegistrator& commandRegistrator) const;

	virtual void				OnAdd (NE::EvaluationEnv& env) const;
	virtual void				OnDelete (NE::EvaluationEnv& env) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	template <class SlotType>
	Point GetSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	template <class SlotType>
	bool HasSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	template <class SlotType>
	Rect GetSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	template <class SlotConstType>
	void EnumerateUISlots (const std::function<bool (const SlotConstType&)>& processor) const;

protected:
	bool						RegisterUIInputSlot (const UIInputSlotPtr& newInputSlot);
	bool						RegisterUIOutputSlot (const UIOutputSlotPtr& newOutputSlot);

	virtual void				DrawInplace (NodeUIDrawingEnvironment& env) const;

private:
	UIInputSlotPtr				GetModifiableUIInputSlot (const NE::SlotId& slotId);
	UIOutputSlotPtr				GetModifiableUIOutputSlot (const NE::SlotId& slotId);

	const NodeDrawingImage&		GetNodeDrawingImage (NodeUIDrawingEnvironment& env) const;
	virtual void				UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const = 0;

	NE::LocString				nodeName;
	Point						nodePosition;
	mutable NodeDrawingImage	nodeDrawingImage;
};

using UINodePtr = std::shared_ptr<UINode>;
using UINodeConstPtr = std::shared_ptr<const UINode>;

}

#endif
