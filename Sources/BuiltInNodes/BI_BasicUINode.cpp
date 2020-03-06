#include "BI_BasicUINode.hpp"

#include <cmath>

namespace BI
{

NE::SerializationInfo BasicUINode::serializationInfo (NE::ObjectVersion (1));

BasicUINode::BasicUINode () :
	BasicUINode (std::wstring (), NUIE::Point ())
{

}

BasicUINode::BasicUINode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::UINode (name, position),
	layout ()
{

}

BasicUINode::~BasicUINode ()
{

}

NE::Stream::Status BasicUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::UINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status BasicUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::UINode::Write (outputStream);
	return outputStream.GetStatus ();
}

void BasicUINode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	layout.Draw (*this, env, drawingImage);
}

}
