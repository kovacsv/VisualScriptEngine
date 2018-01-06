#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "Debug.hpp"
#include "ContextDecorators.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommands.hpp"
#include "NodeUIEnvironment.hpp"
#include "SingleValues.hpp"

namespace NUIE
{

NE::SerializationInfo UINode::serializationInfo (NE::ObjectId ("{26A833A1-8FFD-48BA-8A4C-94BD02084BEA}"), NE::ObjectVersion (1));

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

void UINode::Draw (NodeUIDrawingEnvironment& env) const
{
	ViewBox nodeViewBox (nodePosition, 1.0);
	ViewBoxContextDecorator nodeContext (env.GetDrawingContext (), nodeViewBox);
	NodeUIDrawingEnvironmentContextDecorator nodeEnv (env, nodeContext);
	DrawInplace (nodeEnv);
}

Rect UINode::GetNodeRect (NodeUIDrawingEnvironment& env) const
{
	Rect nodeRect = GetNodeDrawingImage (env).GetNodeRect ();
	return nodeRect.Offset (nodePosition);
}

void UINode::InvalidateDrawing () const
{
	nodeDrawingImage.Reset ();
}

Point UINode::GetInputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	Point position = GetNodeDrawingImage (env).GetInputSlotConnPosition (slotId);
	return position + nodePosition;
}

Point UINode::GetOutputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	Point position = GetNodeDrawingImage (env).GetOutputSlotConnPosition (slotId);
	return position + nodePosition;
}

bool UINode::HasInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	return GetNodeDrawingImage (env).HasInputSlotRect (slotId);
}

Rect UINode::GetInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	Rect rect = GetNodeDrawingImage (env).GetInputSlotRect (slotId);
	return rect.Offset (nodePosition);
}

bool UINode::HasOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	return GetNodeDrawingImage (env).HasOutputSlotRect (slotId);
}

Rect UINode::GetOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	Rect rect = GetNodeDrawingImage (env).GetOutputSlotRect (slotId);
	return rect.Offset (nodePosition);
}

bool UINode::HasSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const
{
	return GetNodeDrawingImage (env).HasSpecialRect (rectId);
}

Rect UINode::GetSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const
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

void UINode::RegisterParameters (NodeParameterList& parameterList) const
{
	class NodeNameParameter : public NodeParameter
	{
	public:
		NodeNameParameter () :
			NodeParameter (L"Name")
		{
		
		}

		NE::ValuePtr GetValue (const UINodePtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::StringValue (uiNode->GetNodeName ()));
		}

		bool IsApplicableTo (const UINodePtr&) const override
		{
			return true;
		}

		bool SetValue (NodeUIManager&, NE::EvaluationEnv&, UINodePtr& uiNode, NE::ValuePtr& value) override
		{
			if (DBGERROR (!NE::Value::IsType<NE::StringValue> (value))) {
				return false;
			}
			uiNode->SetNodeName (NE::StringValue::Get (value));
			uiNode->InvalidateDrawing ();
			return true;
		}
	};

	parameterList.AddParameter (NodeParameterPtr (new NodeNameParameter ()));
}

void UINode::RegisterCommands (NodeCommandRegistrator&) const
{

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
Point UINode::GetSlotConnPosition<UIInputSlotPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId)
{
	return GetInputSlotConnPosition (env, slotId);
}

template <>
Point UINode::GetSlotConnPosition<UIOutputSlotPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId)
{
	return GetOutputSlotConnPosition (env, slotId);
}

template <>
bool UINode::HasSlotRect <UIInputSlotPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId)
{
	return HasInputSlotRect (env, slotId);
}

template <>
bool UINode::HasSlotRect<UIOutputSlotPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId)
{
	return HasOutputSlotRect (env, slotId);
}

template <>
Rect UINode::GetSlotRect <UIInputSlotPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId)
{
	return GetInputSlotRect (env, slotId);
}

template <>
Rect UINode::GetSlotRect<UIOutputSlotPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId)
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

void UINode::DrawInplace (NodeUIDrawingEnvironment& env) const
{
	GetNodeDrawingImage (env).Draw (env.GetDrawingContext ());
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

const NodeDrawingImage& UINode::GetNodeDrawingImage (NodeUIDrawingEnvironment& env) const
{
	if (nodeDrawingImage.IsEmpty ()) {
		UpdateNodeDrawingImage (env, nodeDrawingImage);
	}
	return nodeDrawingImage;
}

}
