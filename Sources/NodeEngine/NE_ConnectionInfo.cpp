#include "NE_ConnectionInfo.hpp"
#include "NE_Node.hpp"
#include "NE_Debug.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

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

bool SlotInfo::operator< (const SlotInfo& rhs) const
{
	return nodeId < rhs.nodeId && slotId < rhs.slotId;
}

bool SlotInfo::operator> (const SlotInfo& rhs) const
{
	return nodeId > rhs.nodeId && slotId > rhs.slotId;
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

bool ConnectionInfo::operator< (const ConnectionInfo& rhs) const
{
	return outputSlotInfo < rhs.outputSlotInfo && inputSlotInfo < rhs.inputSlotInfo;
}

bool ConnectionInfo::operator> (const ConnectionInfo& rhs) const
{
	return outputSlotInfo > rhs.outputSlotInfo && inputSlotInfo > rhs.inputSlotInfo;
}

}
