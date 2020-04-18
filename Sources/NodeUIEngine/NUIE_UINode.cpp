#include "NUIE_UINode.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_Localization.hpp"
#include "NE_Debug.hpp"
#include "NUIE_ContextDecorators.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "NUIE_NodeMenuCommandRegistrator.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeUIEnvironment.hpp"

namespace NUIE
{

NE::SerializationInfo UINode::serializationInfo (NE::ObjectVersion (1));

UINodeCommandInterface::UINodeCommandInterface ()
{

}

UINodeCommandInterface::~UINodeCommandInterface ()
{

}

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

bool UINode::HasFeature (const FeatureId& featureId) const
{
	return nodeFeatureSet.HasFeature (featureId);
}

const NodeFeaturePtr& UINode::GetFeature (const FeatureId& featureId) const
{
	return nodeFeatureSet.GetFeature (featureId);
}

NUIE::EventHandlerResult UINode::HandleMouseClick (NodeUIEnvironment&, const ModifierKeys&, MouseButton, const Point&, UINodeCommandInterface&)
{
	return EventHandlerResult::EventNotHandled;
}

NUIE::EventHandlerResult UINode::HandleMouseDoubleClick (NodeUIEnvironment&, const ModifierKeys&, MouseButton, const Point&, UINodeCommandInterface&)
{
	return EventHandlerResult::EventNotHandled;
}

void UINode::RegisterParameters (NodeParameterList& parameterList) const
{
	class NodeNameParameter : public NotEmptyStringNodeParameter<UINode>
	{
	public:
		NodeNameParameter () :
			NotEmptyStringNodeParameter<UINode> (NE::Localize (L"Node Name"))
		{
		
		}

		virtual NE::ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::StringValue (uiNode->GetNodeName ()));
		}

		virtual bool SetValueInternal (UINodeInvalidator& invalidator, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			uiNode->SetNodeName (NE::StringValue::Get (value));
			invalidator.InvalidateDrawing ();
			return true;
		}
	};

	parameterList.AddParameter (NodeParameterPtr (new NodeNameParameter ()));
	nodeFeatureSet.RegisterParameters (parameterList);
}

void UINode::RegisterCommands (NodeCommandRegistrator& commandRegistrator) const
{
	nodeFeatureSet.RegisterCommands (commandRegistrator);
}

void UINode::OnFeatureChange (const FeatureId&, NE::EvaluationEnv&) const
{

}

void UINode::OnAdd (NE::EvaluationEnv&) const
{

}

void UINode::OnDelete (NE::EvaluationEnv&) const
{

}

NE::Stream::Status UINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	Node::Read (inputStream);
	inputStream.Read (nodeName);
	ReadPoint (inputStream, nodePosition);
	nodeFeatureSet.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status UINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	Node::Write (outputStream);
	outputStream.Write (nodeName);
	WritePoint (outputStream, nodePosition);
	nodeFeatureSet.Write (outputStream);
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

void UINode::DrawInplace (NodeUIDrawingEnvironment& env) const
{
	GetNodeDrawingImage (env).Draw (env.GetDrawingContext ());
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

bool UINode::RegisterFeature (const NodeFeaturePtr& newFeature)
{
	nodeFeatureSet.AddFeature (newFeature->GetId (), newFeature);
	return true;
}

const NodeDrawingImage& UINode::GetNodeDrawingImage (NodeUIDrawingEnvironment& env) const
{
	if (nodeDrawingImage.IsEmpty ()) {
		UpdateNodeDrawingImage (env, nodeDrawingImage);
	}
	return nodeDrawingImage;
}

bool UINode::RegisterInputSlot (const NE::InputSlotPtr& newInputSlot)
{
	return Node::RegisterInputSlot (newInputSlot);
}

bool UINode::RegisterOutputSlot (const NE::OutputSlotPtr& newOutputSlot)
{
	return Node::RegisterOutputSlot (newOutputSlot);
}

}
