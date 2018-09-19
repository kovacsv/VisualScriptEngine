#include "BI_BasicUINode.hpp"
#include "BI_UINodeLayouts.hpp"

#include <cmath>

namespace BI
{

NE::SerializationInfo BasicUINode::serializationInfo (NE::ObjectVersion (1));

BasicUINode::BasicUINode () :
	BasicUINode (L"", NUIE::Point ())
{

}

BasicUINode::BasicUINode (const std::wstring& name, const NUIE::Point& position) :
	UINode (name, position)
{

}

BasicUINode::~BasicUINode ()
{

}

NE::Stream::Status BasicUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status BasicUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	return outputStream.GetStatus ();
}

void BasicUINode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	DrawStatusHeaderWithSlotsLayout (*this, env, drawingImage);
}

}
