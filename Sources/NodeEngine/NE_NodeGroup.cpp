#include "NE_NodeGroup.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

SERIALIZATION_INFO (NodeGroup, 2);

NodeGroup::NodeGroup () :
	id (NullNodeGroupId)
{

}

NodeGroup::~NodeGroup ()
{

}

const NE::NodeGroupId& NodeGroup::GetId () const
{
	return id;
}

void NodeGroup::SetId (const NodeGroupId& newId)
{
	id = newId;
}

Stream::Status NodeGroup::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	if (header.GetVersion () > 1) {
		id.Read (inputStream);
	}
	return inputStream.GetStatus ();
}

Stream::Status NodeGroup::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	id.Write (outputStream);
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
