#include "BI_BasicUINode.hpp"
#include "BI_UINodeLayouts.hpp"

#include <cmath>

namespace BI
{

SERIALIZATION_INFO (BasicUINode, 1);

BasicUINode::BasicUINode () :
	BasicUINode (NE::LocString (), NUIE::Point ())
{

}

BasicUINode::BasicUINode (const NE::LocString& name, const NUIE::Point& position) :
	BasicUINode (name, position, UINodeLayoutPtr (new HeaderWithSlotsLayout ()))
{

}

BasicUINode::BasicUINode (const NE::LocString& name, const NUIE::Point& position, const UINodeLayoutPtr& layout) :
	BasicUINode (name, position, layout, NUIE::InvalidIconId)
{

}
BasicUINode::BasicUINode (const NE::LocString& name, const NUIE::Point& position, const UINodeLayoutPtr& layout, const NUIE::IconId& iconId) :
	NUIE::UINode (name, position),
	layout (layout),
	iconId (iconId)
{

}

BasicUINode::~BasicUINode ()
{

}

bool BasicUINode::HasIconId () const
{
	return iconId != NUIE::InvalidIconId;
}

const NUIE::IconId& BasicUINode::GetIconId () const
{
	return iconId;
}

void BasicUINode::SetIconId (const NUIE::IconId& newIconId)
{
	iconId = newIconId;
}

bool BasicUINode::HasFeature (const FeatureId& featureId) const
{
	return nodeFeatureSet.HasFeature (featureId);
}

const NodeFeaturePtr& BasicUINode::GetFeature (const FeatureId& featureId) const
{
	return nodeFeatureSet.GetFeature (featureId);
}

NE::Stream::Status BasicUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::UINode::Read (inputStream);
	iconId.Read (inputStream);
	nodeFeatureSet.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status BasicUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::UINode::Write (outputStream);
	iconId.Write (outputStream);
	nodeFeatureSet.Write (outputStream);
	return outputStream.GetStatus ();
}

void BasicUINode::OnFeatureChange (const FeatureId&, NE::EvaluationEnv&) const
{

}

void BasicUINode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	UINode::RegisterParameters (parameterList);
	nodeFeatureSet.RegisterParameters (parameterList);
}

void BasicUINode::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	UINode::RegisterCommands (commandRegistrator);
	nodeFeatureSet.RegisterCommands (commandRegistrator);
}

bool BasicUINode::RegisterFeature (const NodeFeaturePtr& newFeature)
{
	nodeFeatureSet.AddFeature (newFeature->GetId (), newFeature);
	return true;
}

NUIE::EventHandlerResult BasicUINode::HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface)
{
	return layout->HandleMouseClick (*this, env, modifierKeys, mouseButton, position, commandInterface);
}

NUIE::EventHandlerResult BasicUINode::HandleMouseDoubleClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface)
{
	return HandleMouseClick (env, modifierKeys, mouseButton, position, commandInterface);
}

void BasicUINode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	layout->Draw (*this, env, drawingImage);
}

}
