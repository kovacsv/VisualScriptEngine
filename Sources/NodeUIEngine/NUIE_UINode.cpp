#include "NUIE_UINode.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_Localization.hpp"
#include "NE_Debug.hpp"
#include "NUIE_ContextDecorators.hpp"
#include "NUIE_EnvironmentDecorators.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeUIEnvironment.hpp"

namespace NUIE
{

SERIALIZATION_INFO (UINode, 1);

UINodeCommandInterface::UINodeCommandInterface ()
{

}

UINodeCommandInterface::~UINodeCommandInterface ()
{

}

UINode::UINode () :
	UINode (NE::LocString (), Point ())
{

}

UINode::UINode (const NE::LocString& nodeName, const Point& nodePosition) :
	Node (),
	nodeName (nodeName),
	nodePosition (nodePosition)
{

}

UINode::~UINode ()
{

}

const NE::LocString& UINode::GetName () const
{
	return nodeName;
}

void UINode::SetName (const std::wstring& newNodeName)
{
	nodeName.SetCustom (newNodeName);
}

const Point& UINode::GetPosition () const
{
	return nodePosition;
}

void UINode::SetPosition (const Point& newPosition)
{
	nodePosition = newPosition;
}

void UINode::Draw (NodeUIDrawingEnvironment& env) const
{
	ViewBox nodeViewBox (nodePosition, 1.0);
	ViewBoxContextDecorator nodeContext (env.GetDrawingContext (), nodeViewBox);
	DrawingEnvironmentContextDecorator nodeEnv (env, nodeContext);
	DrawInplace (nodeEnv);
}

Rect UINode::GetRect (NodeUIDrawingEnvironment& env) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	Rect nodeRect = drawingImage.GetNodeRect ();
	return nodeRect.Offset (nodePosition);
}

void UINode::InvalidateDrawing () const
{
	nodeDrawingImage.Reset ();
}

Point UINode::GetInputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	Point position = drawingImage.GetInputSlotConnPosition (slotId);
	return position + nodePosition;
}

Point UINode::GetOutputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	Point position = drawingImage.GetOutputSlotConnPosition (slotId);
	return position + nodePosition;
}

bool UINode::HasInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	return drawingImage.HasInputSlotRect (slotId);
}

Rect UINode::GetInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	Rect rect = drawingImage.GetInputSlotRect (slotId);
	return rect.Offset (nodePosition);
}

bool UINode::HasOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	return drawingImage.HasOutputSlotRect (slotId);
}

Rect UINode::GetOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	Rect rect = drawingImage.GetOutputSlotRect (slotId);
	return rect.Offset (nodePosition);
}

bool UINode::HasSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	return drawingImage.HasSpecialRect (rectId);
}

Rect UINode::GetSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	Rect rect = drawingImage.GetSpecialRect (rectId);
	return rect.Offset (nodePosition);
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

void UINode::EnumerateUIInputSlots (const std::function<bool (const UIInputSlotConstPtr&)>& processor) const
{
	EnumerateInputSlots ([&] (const NE::InputSlotConstPtr& inputSlot) {
		UIInputSlotConstPtr uiInputSlot = std::dynamic_pointer_cast<const UIInputSlot> (inputSlot);
		if (DBGERROR (uiInputSlot == nullptr)) {
			return false;
		}
		return processor (uiInputSlot);
	});
}

void UINode::EnumerateUIOutputSlots (const std::function<bool (const UIOutputSlotConstPtr&)>& processor) const
{
	EnumerateOutputSlots ([&] (const NE::OutputSlotConstPtr& outputSlot) {
		UIOutputSlotConstPtr uiOutputSlot = std::dynamic_pointer_cast<const UIOutputSlot> (outputSlot);
		if (DBGERROR (uiOutputSlot == nullptr)) {
			return false;
		}
		return processor (uiOutputSlot);
	});
}

NE::LocString UINode::GetUIInputSlotName (const NE::SlotId& slotId) const
{
	UIInputSlotConstPtr inputSlot = GetUIInputSlot (slotId);
	if (DBGERROR (inputSlot == nullptr)) {
		return NE::LocString ();
	}
	return inputSlot->GetName ();
}

void UINode::SetUIInputSlotName (const NE::SlotId& slotId, const std::wstring& newName)
{
	UIInputSlotPtr inputSlot = GetModifiableUIInputSlot (slotId);
	if (DBGERROR (inputSlot == nullptr)) {
		return;
	}
	inputSlot->SetName (newName);
}

NE::LocString UINode::GetUIOutputSlotName (const NE::SlotId& slotId) const
{
	UIOutputSlotConstPtr outputSlot = GetUIOutputSlot (slotId);
	if (DBGERROR (outputSlot == nullptr)) {
		return NE::LocString ();
	}
	return outputSlot->GetName ();
}

void UINode::SetUIOutputSlotName (const NE::SlotId& slotId, const std::wstring& newName)
{
	UIOutputSlotPtr outputSlot = GetModifiableUIOutputSlot (slotId);
	if (DBGERROR (outputSlot == nullptr)) {
		return;
	}
	outputSlot->SetName (newName);
}

EventHandlerResult UINode::HandleMouseClick (NodeUIEnvironment&, const ModifierKeys&, MouseButton, const Point&, UINodeCommandInterface&)
{
	return EventHandlerResult::EventNotHandled;
}

EventHandlerResult UINode::HandleMouseDoubleClick (NodeUIEnvironment&, const ModifierKeys&, MouseButton, const Point&, UINodeCommandInterface&)
{
	return EventHandlerResult::EventNotHandled;
}

void UINode::RegisterParameters (NodeParameterList& parameterList) const
{
	class NodeNameParameter : public NotEmptyStringNodeParameter<UINode>
	{
	public:
		NodeNameParameter () :
			NotEmptyStringNodeParameter<UINode> (NE::LocString (L"Node Name"))
		{
		
		}

		virtual NE::ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::StringValue (uiNode->GetName ().GetLocalized ()));
		}

		virtual bool SetValueInternal (UINodeInvalidator& invalidator, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			uiNode->SetName (NE::StringValue::Get (value));
			invalidator.InvalidateDrawing ();
			return true;
		}
	};

	parameterList.AddParameter (NodeParameterPtr (new NodeNameParameter ()));
}

void UINode::RegisterCommands (NodeCommandRegistrator&) const
{

}

void UINode::OnDelete (NE::EvaluationEnv&) const
{

}

NE::Stream::Status UINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	Node::Read (inputStream);
	nodeName.Read (inputStream);
	ReadPoint (inputStream, nodePosition);
	return inputStream.GetStatus ();
}

NE::Stream::Status UINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	Node::Write (outputStream);
	nodeName.Write (outputStream);
	WritePoint (outputStream, nodePosition);
	return outputStream.GetStatus ();
}

template <>
Point UINode::GetSlotConnPosition<UIInputSlotConstPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	return GetInputSlotConnPosition (env, slotId);
}

template <>
Point UINode::GetSlotConnPosition<UIOutputSlotConstPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	return GetOutputSlotConnPosition (env, slotId);
}

template <>
bool UINode::HasSlotRect <UIInputSlotConstPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	return HasInputSlotRect (env, slotId);
}

template <>
bool UINode::HasSlotRect<UIOutputSlotConstPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	return HasOutputSlotRect (env, slotId);
}

template <>
Rect UINode::GetSlotRect <UIInputSlotConstPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	return GetInputSlotRect (env, slotId);
}

template <>
Rect UINode::GetSlotRect<UIOutputSlotConstPtr> (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const
{
	return GetOutputSlotRect (env, slotId);
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
	if (!RegisterInputSlot (newInputSlot)) {
		return false;
	}
	return true;
}

bool UINode::RegisterUIOutputSlot (const UIOutputSlotPtr& newOutputSlot)
{
	if (!RegisterOutputSlot (newOutputSlot)) {
		return false;
	}
	return true;
}

void UINode::DrawInplace (NodeUIDrawingEnvironment& env) const
{
	const NodeDrawingImage& drawingImage = GetDrawingImage (env);
	drawingImage.Draw (env.GetDrawingContext ());
}

UIInputSlotPtr UINode::GetModifiableUIInputSlot (const NE::SlotId& slotId)
{
	NE::InputSlotPtr inputSlot = GetModifiableInputSlot (slotId);
	return std::dynamic_pointer_cast<UIInputSlot> (inputSlot);
}

UIOutputSlotPtr UINode::GetModifiableUIOutputSlot (const NE::SlotId& slotId)
{
	NE::OutputSlotPtr outputSlot = GetModifiableOutputSlot (slotId);
	return std::dynamic_pointer_cast<UIOutputSlot> (outputSlot);
}

const NodeDrawingImage& UINode::GetDrawingImage (NodeUIDrawingEnvironment& env) const
{
	if (nodeDrawingImage.IsEmpty ()) {
		UpdateDrawingImage (env, nodeDrawingImage);
	}
	return nodeDrawingImage;
}

}
