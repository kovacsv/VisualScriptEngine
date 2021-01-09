#include "NE_ConnectionInfo.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

SERIALIZATION_INFO (SlotInfo, 1);
SERIALIZATION_INFO (ConnectionInfo, 1);

SlotInfo::SlotInfo () :
	SlotInfo (NullNodeId, NullSlotId)
{

}

SlotInfo::SlotInfo (const NodeId& nodeId, const SlotId& slotId) :
	nodeId (nodeId),
	slotId (slotId)
{

}

size_t SlotInfo::GenerateHashValue () const
{
	return nodeId.GenerateHashValue () + 49157 * slotId.GenerateHashValue ();
}

const NodeId& SlotInfo::GetNodeId () const
{
	return nodeId;
}

const SlotId& SlotInfo::GetSlotId () const
{
	return slotId;
}

bool SlotInfo::operator== (const SlotInfo& rhs) const
{
	return nodeId == rhs.nodeId && slotId == rhs.slotId;
}

bool SlotInfo::operator!= (const SlotInfo& rhs) const
{
	return !operator== (rhs);
}

bool SlotInfo::operator< (const SlotInfo& rhs) const
{
	return nodeId < rhs.nodeId && slotId < rhs.slotId;
}

bool SlotInfo::operator> (const SlotInfo& rhs) const
{
	return nodeId > rhs.nodeId && slotId > rhs.slotId;
}

Stream::Status SlotInfo::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	nodeId.Read (inputStream);
	slotId.Read (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status SlotInfo::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	nodeId.Write (outputStream);
	slotId.Write (outputStream);
	return outputStream.GetStatus ();
}

ConnectionInfo::ConnectionInfo ()
{

}

ConnectionInfo::ConnectionInfo (const SlotInfo& outputSlotInfo, const SlotInfo& inputSlotInfo) :
	outputSlotInfo (outputSlotInfo),
	inputSlotInfo (inputSlotInfo)
{

}

const SlotInfo& ConnectionInfo::GetOutputSlotInfo () const
{
	return outputSlotInfo;
}

const SlotInfo& ConnectionInfo::GetInputSlotInfo () const
{
	return inputSlotInfo;
}

const NodeId& ConnectionInfo::GetOutputNodeId () const
{
	return outputSlotInfo.GetNodeId ();
}

const SlotId& ConnectionInfo::GetOutputSlotId () const
{
	return outputSlotInfo.GetSlotId ();
}

const NodeId& ConnectionInfo::GetInputNodeId () const
{
	return inputSlotInfo.GetNodeId ();
}

const SlotId& ConnectionInfo::GetInputSlotId () const
{
	return inputSlotInfo.GetSlotId ();
}

bool ConnectionInfo::operator== (const ConnectionInfo& rhs) const
{
	return outputSlotInfo == rhs.outputSlotInfo && inputSlotInfo == rhs.inputSlotInfo;
}

bool ConnectionInfo::operator!= (const ConnectionInfo& rhs) const
{
	return !operator== (rhs);
}

bool ConnectionInfo::operator< (const ConnectionInfo& rhs) const
{
	return outputSlotInfo < rhs.outputSlotInfo && inputSlotInfo < rhs.inputSlotInfo;
}

bool ConnectionInfo::operator> (const ConnectionInfo& rhs) const
{
	return outputSlotInfo > rhs.outputSlotInfo && inputSlotInfo > rhs.inputSlotInfo;
}

Stream::Status ConnectionInfo::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	outputSlotInfo.Read (inputStream);
	inputSlotInfo.Read (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status ConnectionInfo::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	outputSlotInfo.Write (outputStream);
	inputSlotInfo.Write (outputStream);
	return outputStream.GetStatus ();
}

}
