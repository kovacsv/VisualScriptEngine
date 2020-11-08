#ifndef NE_CONNECTIONMANAGER_HPP
#define NE_CONNECTIONMANAGER_HPP

#include "NE_NodeEngineTypes.hpp"
#include "NE_ConnectionList.hpp"
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace NE
{

class ConnectionManager
{
public:
	ConnectionManager ();

	void	Clear ();
	bool	IsEmpty () const;
	size_t	GetConnectionCount () const;

	bool	HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const;
	bool	HasConnectedInputSlots (const OutputSlotConstPtr& outputSlot) const;

	size_t	GetConnectedOutputSlotCount (const InputSlotConstPtr& inputSlot) const;
	size_t	GetConnectedInputSlotCount (const OutputSlotConstPtr& outputSlot) const;

	void	EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const;
	void	EnumerateConnectedInputSlots (const OutputSlotConstPtr& outputSlot, const std::function<void (const InputSlotConstPtr&)>& processor) const;

	bool	IsOutputSlotConnectedToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	bool	CanConnectOutputSlotToInputSlot (const InputSlotConstPtr& inputSlot) const;
	bool	CanConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	bool	ConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);
	
	bool	DisconnectOutputSlotFromInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);
	bool	DisconnectAllOutputSlotsFromInputSlot (const InputSlotConstPtr& inputSlot);
	bool	DisconnectAllInputSlotsFromOutputSlot (const OutputSlotConstPtr& outputSlot);

private:
	ConnectionList<OutputSlotConstPtr, InputSlotConstPtr>	outputToInputConnections;
	ConnectionList<InputSlotConstPtr, OutputSlotConstPtr>	inputToOutputConnections;
};

}

#endif
