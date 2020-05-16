#include "TestNodes.hpp"

DYNAMIC_SERIALIZATION_INFO (SerializableTestNode, 1, "{73A78FBB-6563-4009-A1B2-7DF56900F522}");
DYNAMIC_SERIALIZATION_INFO (SerializableTestUINode, 1, "{93A78362-DFD9-46CB-B9F3-2F2DA9E1F964}");

SerializableTestNode::SerializableTestNode () :
	Node ()
{

}

void SerializableTestNode::Initialize ()
{
	DBGBREAK ();
}

ValueConstPtr SerializableTestNode::Calculate (NE::EvaluationEnv&) const
{
	DBGBREAK ();
	return nullptr;
}

Stream::Status SerializableTestNode::Read (InputStream& inputStream)
{
	Node::Read (inputStream);
	return Stream::Status::NoError;
}

Stream::Status SerializableTestNode::Write (OutputStream& outputStream) const
{
	Node::Write (outputStream);
	return Stream::Status::NoError;
}

SerializableTestUINode::SerializableTestUINode () :
	SerializableTestUINode (String (), Point ())
{

}

SerializableTestUINode::SerializableTestUINode (const String& nodeName, const Point& nodePosition) :
	UINode (nodeName, nodePosition)
{

}

void SerializableTestUINode::Initialize ()
{
	DBGBREAK ();
}

ValueConstPtr SerializableTestUINode::Calculate (NE::EvaluationEnv&) const
{
	DBGBREAK ();
	return nullptr;
}

void SerializableTestUINode::UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const
{
	DBGBREAK ();
}

Stream::Status SerializableTestUINode::Read (InputStream& inputStream)
{
	UINode::Read (inputStream);
	return Stream::Status::NoError;
}

Stream::Status SerializableTestUINode::Write (OutputStream& outputStream) const
{
	UINode::Write (outputStream);
	return Stream::Status::NoError;
}
