#include "BI_BasicUINode.hpp"
#include "BI_UINodeLayouts.hpp"

#include <cmath>

namespace BI
{

NE::SerializationInfo BasicUINode::serializationInfo (NE::ObjectVersion (1));

BasicUINode::BasicUINode () :
	BasicUINode (std::wstring (), NUIE::Point ())
{

}

BasicUINode::BasicUINode (const std::wstring& name, const NUIE::Point& position) :
	BasicUINode (name, position, NUIE::InvalidIconId, UINodeLayoutPtr (new HeaderWithSlotsLayout ()))
{

}

BasicUINode::BasicUINode (const std::wstring& name, const NUIE::Point& position, const NUIE::IconId& iconId) :
	BasicUINode (name, position, iconId, UINodeLayoutPtr (new HeaderWithSlotsLayout ()))
{

}
BasicUINode::BasicUINode (const std::wstring& name, const NUIE::Point& position, const NUIE::IconId& iconId, const UINodeLayoutPtr& layout) :
	NUIE::UINode (name, position),
	iconId (iconId),
	layout (layout)
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

NE::Stream::Status BasicUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::UINode::Read (inputStream);
	iconId.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status BasicUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::UINode::Write (outputStream);
	iconId.Write (outputStream);
	return outputStream.GetStatus ();
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
