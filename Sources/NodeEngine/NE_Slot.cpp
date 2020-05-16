#include "NE_Slot.hpp"
#include "NE_Node.hpp"
#include "NE_Debug.hpp"

namespace NE
{

SERIALIZATION_INFO (Slot, 1);

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
