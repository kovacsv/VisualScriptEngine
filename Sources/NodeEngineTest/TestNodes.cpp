#include "TestNodes.hpp"

DynamicSerializationInfo SerializableTestNode::serializationInfo (ObjectId ("{73A78FBB-6563-4009-A1B2-7DF56900F522}"), ObjectVersion (1), SerializableTestNode::CreateSerializableInstance);
DynamicSerializationInfo SerializableTestUINode::serializationInfo (ObjectId ("{93A78362-DFD9-46CB-B9F3-2F2DA9E1F964}"), ObjectVersion (1), SerializableTestUINode::CreateSerializableInstance);

SerializableTestNode::SerializableTestNode () :
	Node ()
{

}

void SerializableTestNode::Initialize ()
{
	DBGBREAK ();
}

ValuePtr SerializableTestNode::Calculate (NE::EvaluationEnv&) const
{
	DBGBREAK ();
	return nullptr;
}

Stream::Status SerializableTestNode::Read (InputStream&)
{
	DBGBREAK ();
	return Stream::Status::Error;
}

Stream::Status SerializableTestNode::Write (OutputStream&) const
{
	DBGBREAK ();
	return Stream::Status::Error;
}

SerializableTestUINode::SerializableTestUINode () :
	SerializableTestUINode (L"", Point ())
{

}

SerializableTestUINode::SerializableTestUINode (const std::wstring& nodeName, const Point& nodePosition) :
	UINode (nodeName, nodePosition)
{

}

void SerializableTestUINode::Initialize ()
{
	DBGBREAK ();
}

ValuePtr SerializableTestUINode::Calculate (NE::EvaluationEnv&) const
{
	DBGBREAK ();
	return nullptr;
}

void SerializableTestUINode::UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const
{
	DBGBREAK ();
}

Stream::Status SerializableTestUINode::Read (InputStream&)
{
	DBGBREAK ();
	return Stream::Status::Error;
}

Stream::Status SerializableTestUINode::Write (OutputStream&) const
{
	DBGBREAK ();
	return Stream::Status::Error;
}
