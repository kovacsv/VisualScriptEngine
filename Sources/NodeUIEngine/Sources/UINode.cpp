#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "Debug.hpp"
#include "ContextDecorators.hpp"
#include "UINodeCommands.hpp"
#include "NodeUIManager.hpp"
#include "NodeUIEnvironment.hpp"

namespace NUIE
{

NE::SerializationInfo UINode::serializationInfo (NE::ObjectId ("{26A833A1-8FFD-48BA-8A4C-94BD02084BEA}"), NE::ObjectVersion (1));

static std::wstring DefaultSlotName = L"";

class DeleteNodeCommand : public NodeCommand
{
public:
	DeleteNodeCommand (const std::wstring& name) :
		NodeCommand (name, false)
	{

	}

	virtual bool IsApplicableTo (const UINodePtr&) override
	{
		return true;
	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UINodePtr& uiNode) override
	{
		uiManager.DeleteNode (uiNode, uiEnvironment.GetEvaluationEnv ());
	}
};

UINode::UINode () :
	Node ()
{

}

UINode::UINode (const std::wstring& nodeName, const Point& nodePosition) :
	Node (),
	nodeName (nodeName),
	nodePosition (nodePosition)
{

}

UINode::~UINode ()
{

}

const std::wstring& UINode::GetNodeName () const
{
	return nodeName;
}

void UINode::SetNodeName (const std::wstring& newNodeName)
{
	nodeName = newNodeName;
	InvalidateDrawing ();
}

const Point& UINode::GetNodePosition () const
{
	return nodePosition;
}

void UINode::SetNodePosition (const Point& newPosition)
{
	nodePosition = newPosition;
}

void UINode::Draw (NodeUIEnvironment& env) const
{
	ViewBox nodeViewBox (nodePosition, 1.0);
	ViewBoxContextDecorator nodeContext (env.GetDrawingContext (), nodeViewBox);
	GetNodeDrawingImage (env).Draw (nodeContext);
}

NE::Checksum UINode::GetDrawingImageChecksum (NodeUIEnvironment& env) const
{
	// TODO: Can be cacheable
	return GetNodeDrawingImage (env).GetChecksum ();
}

UINode::Status UINode::GetStatus (NE::EvaluationEnv& env) const
{
	NE::ValuePtr value = Evaluate (env);
	if (value != nullptr) {
		return Status::HasValue;
	}
	return Status::HasNoValue;
}

Rect UINode::GetNodeRect (NodeUIEnvironment& env) const
{
	Rect nodeRect = GetNodeDrawingImage (env).GetNodeRect ();
	return nodeRect.Offset (nodePosition);
}

void UINode::InvalidateDrawing () const
{
	nodeDrawingImage.Reset ();
}

Point UINode::GetInputSlotConnPosition (NodeUIEnvironment& env, const NE::SlotId& slotId) const
{
	Point position = GetNodeDrawingImage (env).GetInputSlotConnPosition (slotId);
	return position + nodePosition;
}

Point UINode::GetOutputSlotConnPosition (NodeUIEnvironment& env, const NE::SlotId& slotId) const
{
	Point position = GetNodeDrawingImage (env).GetOutputSlotConnPosition (slotId);
	return position + nodePosition;
}

bool UINode::HasInputSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId) const
{
	return GetNodeDrawingImage (env).HasInputSlotRect (slotId);
}

Rect UINode::GetInputSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId) const
{
	Rect rect = GetNodeDrawingImage (env).GetInputSlotRect (slotId);
	return rect.Offset (nodePosition);
}

bool UINode::HasOutputSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId) const
{
	return GetNodeDrawingImage (env).HasOutputSlotRect (slotId);
}

Rect UINode::GetOutputSlotRect (NodeUIEnvironment& env, const NE::SlotId& slotId) const
{
	Rect rect = GetNodeDrawingImage (env).GetOutputSlotRect (slotId);
	return rect.Offset (nodePosition);
}

bool UINode::HasSpecialRect (NodeUIEnvironment& env, const std::string& rectId) const
{
	return GetNodeDrawingImage (env).HasSpecialRect (rectId);
}

Rect UINode::GetSpecialRect (NodeUIEnvironment& env, const std::string& rectId) const
{
	Rect rect = GetNodeDrawingImage (env).GetSpecialRect (rectId);
	return rect.Offset (nodePosition);
}

UIInputSlotPtr UINode::GetUIInputSlot (const NE::SlotId& slotId)
{
	NE::InputSlotPtr inputSlot = GetInputSlot (slotId);
	return std::dynamic_pointer_cast<UIInputSlot> (inputSlot);
}

UIOutputSlotPtr UINode::GetUIOutputSlot (const NE::SlotId& slotId)
{
	NE::OutputSlotPtr outputSlot = GetOutputSlot (slotId);
	return std::dynamic_pointer_cast<UIOutputSlot> (outputSlot);
}

UIInputSlotConstPtr UINode::GetUIInputSlot (const NE::SlotId& slotId) const
{
	NE::InputSlotConstPtr inputSlot = GetInputSlot (slotId);
	return std::dynamic_pointer_cast<const UIInputSlot> (inputSlot);
}

UIOutputSlotConstPtr UINode::GetUIOutputSlot (const NE::SlotId& slotId) const
{
	NE::OutputSlotConstPtr outputSlot = GetOutputSlot (slotId);
	return std::dynamic_pointer_cast<const UIOutputSlot> (outputSlot);
}

void UINode::EnumerateUIInputSlots (const std::function<bool (const UIInputSlotPtr&)>& processor)
{
	EnumerateInputSlots ([&] (const NE::InputSlotPtr& inputSlot) -> bool {
		UIInputSlotPtr uiInputSlot = std::dynamic_pointer_cast<UIInputSlot> (inputSlot);
		if (DBGERROR (uiInputSlot == nullptr)) {
			return false;
		}
		return processor (uiInputSlot);
	});
}

void UINode::EnumerateUIOutputSlots (const std::function<bool (const UIOutputSlotPtr&)>& processor)
{
	EnumerateOutputSlots ([&] (const NE::OutputSlotPtr& outputSlot) -> bool {
		UIOutputSlotPtr uiOutputSlot = std::dynamic_pointer_cast<UIOutputSlot> (outputSlot);
		if (DBGERROR (uiOutputSlot == nullptr)) {
			return false;
		}
		return processor (uiOutputSlot);
	});
}

void UINode::EnumerateUIInputSlots (const std::function<bool (const UIInputSlotConstPtr&)>& processor) const
{
	EnumerateInputSlots ([&] (const NE::InputSlotConstPtr& inputSlot) -> bool {
		UIInputSlotConstPtr uiInputSlot = std::dynamic_pointer_cast<const UIInputSlot> (inputSlot);
		if (DBGERROR (uiInputSlot == nullptr)) {
			return false;
		}
		return processor (uiInputSlot);
	});
}

void UINode::EnumerateUIOutputSlots (const std::function<bool (const UIOutputSlotConstPtr&)>& processor) const
{
	EnumerateOutputSlots ([&] (const NE::OutputSlotConstPtr& outputSlot) -> bool {
		UIOutputSlotConstPtr uiOutputSlot = std::dynamic_pointer_cast<const UIOutputSlot> (outputSlot);
		if (DBGERROR (uiOutputSlot == nullptr)) {
			return false;
		}
		return processor (uiOutputSlot);
	});
}

NUIE::EventHandlerResult UINode::HandleMouseDragStart (NodeUIEnvironment&, const KeySet&, MouseButton, const Point&)
{
	// TODO: Never called
	return EventHandlerResult::EventNotHandled;
}

NUIE::EventHandlerResult UINode::HandleMouseDragStop (NodeUIEnvironment&, const KeySet&, MouseButton, const Point&)
{
	// TODO: Never called
	return EventHandlerResult::EventNotHandled;
}

NUIE::EventHandlerResult UINode::HandleMouseDrag (NodeUIEnvironment&, const KeySet&, const Point&)
{
	// TODO: Never called
	return EventHandlerResult::EventNotHandled;
}

NUIE::EventHandlerResult UINode::HandleMouseClick (NodeUIEnvironment&, const KeySet&, MouseButton, const Point&)
{
	return EventHandlerResult::EventNotHandled;
}

NUIE::EventHandlerResult UINode::HandleMouseWheel (NodeUIEnvironment&, const KeySet&, MouseWheelRotation, const Point&)
{
	// TODO: Never called
	return EventHandlerResult::EventNotHandled;
}

void UINode::RegisterCommands (NodeCommandRegistrator& commandRegistrator) const
{
	commandRegistrator.RegisterNodeCommand (NodeCommandPtr (new DeleteNodeCommand (L"Delete Node")));
}

void UINode::OnCreated (NE::EvaluationEnv&) const
{

}

void UINode::OnDeleted (NE::EvaluationEnv&) const
{

}

NE::Stream::Status UINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	Node::Read (inputStream);
	inputStream.Read (nodeName);
	ReadPoint (inputStream, nodePosition);
	return inputStream.GetStatus ();
}

NE::Stream::Status UINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	Node::Write (outputStream);
	outputStream.Write (nodeName);
	WritePoint (outputStream, nodePosition);
	return outputStream.GetStatus ();
}

template <>
Point UINode::GetSlotConnPosition<UIInputSlotPtr> (NodeUIEnvironment& env, const NE::SlotId& slotId)
{
	return GetInputSlotConnPosition (env, slotId);
}

template <>
Point UINode::GetSlotConnPosition<UIOutputSlotPtr> (NodeUIEnvironment& env, const NE::SlotId& slotId)
{
	return GetOutputSlotConnPosition (env, slotId);
}

template <>
bool UINode::HasSlotRect <UIInputSlotPtr> (NodeUIEnvironment& env, const NE::SlotId& slotId)
{
	return HasInputSlotRect (env, slotId);
}

template <>
bool UINode::HasSlotRect<UIOutputSlotPtr> (NodeUIEnvironment& env, const NE::SlotId& slotId)
{
	return HasOutputSlotRect (env, slotId);
}

template <>
Rect UINode::GetSlotRect <UIInputSlotPtr> (NodeUIEnvironment& env, const NE::SlotId& slotId)
{
	return GetInputSlotRect (env, slotId);
}

template <>
Rect UINode::GetSlotRect<UIOutputSlotPtr> (NodeUIEnvironment& env, const NE::SlotId& slotId)
{
	return GetOutputSlotRect (env, slotId);
}

template <>
void UINode::EnumerateUISlots (const std::function<bool (const UIInputSlotPtr&)>& processor)
{
	EnumerateUIInputSlots (processor);
}

template <>
void UINode::EnumerateUISlots (const std::function<bool (const UIOutputSlotPtr&)>& processor)
{
	EnumerateUIOutputSlots (processor);
}

template <>
void UINode::EnumerateUISlots (const std::function<bool (const UIInputSlotConstPtr&)>& processor) const
{
	EnumerateUIInputSlots (processor);
}

template <>
void UINode::EnumerateUISlots (const std::function<bool (const UIOutputSlotConstPtr&)>& processor) const
{
	EnumerateUIOutputSlots (processor);
}

bool UINode::RegisterUIInputSlot (const UIInputSlotPtr& newInputSlot)
{
	if (!Node::RegisterInputSlot (newInputSlot)) {
		return false;
	}
	return true;
}

bool UINode::RegisterUIOutputSlot (const UIOutputSlotPtr& newOutputSlot)
{
	if (!Node::RegisterOutputSlot (newOutputSlot)) {
		return false;
	}
	return true;
}

bool UINode::RegisterInputSlot (const NE::InputSlotPtr& newInputSlot)
{
	return Node::RegisterInputSlot (newInputSlot);
}

bool UINode::RegisterOutputSlot (const NE::OutputSlotPtr& newOutputSlot)
{
	return Node::RegisterOutputSlot (newOutputSlot);
}

const NodeDrawingImage& UINode::GetNodeDrawingImage (NodeUIEnvironment& env) const
{
	if (nodeDrawingImage.IsEmpty ()) {
		UpdateNodeDrawingImage (env, nodeDrawingImage);
	}
	return nodeDrawingImage;
}

}
