#ifndef NE_CONNECTIONINFO_HPP
#define NE_CONNECTIONINFO_HPP

#include "NE_NodeId.hpp"
#include "NE_SlotId.hpp"

namespace NE
{

class SlotInfo
{
public:
	SlotInfo (const NodeId& nodeId, const SlotId& slotId);

	size_t			GenerateHashValue () const;

	const NodeId&	GetNodeId () const;
	const SlotId&	GetSlotId () const;

	bool			operator== (const SlotInfo& rhs) const;
	bool			operator< (const SlotInfo& rhs) const;
	bool			operator> (const SlotInfo& rhs) const;

private:
	NodeId	nodeId;
	SlotId	slotId;
};

class ConnectionInfo
{
public:
	ConnectionInfo (const SlotInfo& outputSlotInfo, const SlotInfo& inputSlotInfo);

	const SlotInfo&		GetOutputSlotInfo () const;
	const SlotInfo&		GetInputSlotInfo () const;

	const NodeId&		GetOutputNodeId () const;
	const SlotId&		GetOutputSlotId () const;
	const NodeId&		GetInputNodeId () const;
	const SlotId&		GetInputSlotId () const;

	bool				operator== (const ConnectionInfo& rhs) const;
	bool				operator< (const ConnectionInfo& rhs) const;
	bool				operator> (const ConnectionInfo& rhs) const;

private:
	SlotInfo	outputSlotInfo;
	SlotInfo	inputSlotInfo;
};

}

namespace std
{
	template <>
	struct hash<NE::SlotInfo>
	{
		size_t operator() (const NE::SlotInfo& info) const noexcept
		{
			return info.GenerateHashValue ();
		}
	};
}

#endif
