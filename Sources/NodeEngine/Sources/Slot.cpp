#include "Slot.hpp"
#include "Node.hpp"
#include "Debug.hpp"

namespace NE
{

SerializationInfo Slot::serializationInfo (ObjectId ("{A9EBC8F6-5708-42F0-B5CD-960964A2C353}"), ObjectVersion (1));

Slot::Slot () :
	slotId (""),
	ownerNode (nullptr)
{

}

Slot::Slot (const SlotId& slotId) :
	slotId (slotId),
	ownerNode (nullptr)
{

}

Slot::~Slot ()
{

}

const SlotId& Slot::GetId () const
{
	return slotId;
}

bool Slot::HasOwnerNode () const
{
	return ownerNode != nullptr;
}

NodeId Slot::GetOwnerNodeId () const
{
	if (DBGERROR (ownerNode == nullptr)) {
		return NullNodeId;
	}
	return ownerNode->GetId ();
}

bool Slot::SetOwnerNode (Node* newOwnerNode)
{
	if (DBGERROR (HasOwnerNode ())) {
		return false;
	}
	ownerNode = newOwnerNode;
	return true;
}

Stream::Status Slot::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	slotId.Read (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status Slot::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	slotId.Write (outputStream);
	return outputStream.GetStatus ();
}

}
