#include "NE_NodeGroup.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

SERIALIZATION_INFO (NodeGroup, 1);

NodeGroup::NodeGroup ()
{

}

NodeGroup::~NodeGroup ()
{

}

Stream::Status NodeGroup::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status NodeGroup::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	return outputStream.GetStatus ();
}

NodeGroupPtr NodeGroup::Clone (const NodeGroupConstPtr& nodeGroup)
{
	MemoryOutputStream outputStream;
	if (DBGERROR (!WriteDynamicObject (outputStream, nodeGroup.get ()))) {
		return nullptr;
	}

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	NodeGroupPtr result (ReadDynamicObject<NodeGroup> (inputStream));
	if (DBGERROR (result == nullptr)) {
		return nullptr;
	}

	return result;
}

}
