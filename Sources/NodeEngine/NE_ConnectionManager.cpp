#include "NE_ConnectionManager.hpp"
#include "NE_Debug.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"

namespace NE
{

ConnectionManager::ConnectionManager () :
	outputToInputConnections (),
	inputToOutputConnections ()
{

}

void ConnectionManager::Clear ()
{
	outputToInputConnections.Clear ();
	inputToOutputConnections.Clear ();
}

bool ConnectionManager::IsEmpty () const
{
	DBGASSERT (inputToOutputConnections.IsEmpty () == outputToInputConnections.IsEmpty ());
	return outputToInputConnections.IsEmpty ();
}

size_t ConnectionManager::GetConnectionCount () const
{
	DBGASSERT (inputToOutputConnections.GetConnectionCount () == outputToInputConnections.GetConnectionCount ());
	return outputToInputConnections.GetConnectionCount ();
}

bool ConnectionManager::HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const
{
	return inputToOutputConnections.HasConnection (inputSlot);
}

bool ConnectionManager::HasConnectedInputSlots (const OutputSlotConstPtr& outputSlot) const
{
	return outputToInputConnections.HasConnection (outputSlot);
}

size_t ConnectionManager::GetConnectedOutputSlotCount (const InputSlotConstPtr& inputSlot) const
{
	return inputToOutputConnections.GetConnectionCount (inputSlot);
}

size_t ConnectionManager::GetConnectedInputSlotCount (const OutputSlotConstPtr& outputSlot) const
{
	return outputToInputConnections.GetConnectionCount (outputSlot);
}

void ConnectionManager::EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const
{
	inputToOutputConnections.EnumerateConnections (inputSlot, processor);
}

void ConnectionManager::EnumerateConnectedInputSlots (const OutputSlotConstPtr& outputSlot, const std::function<void (const InputSlotConstPtr&)>& processor) const
{
	outputToInputConnections.EnumerateConnections (outputSlot, processor);
}

bool ConnectionManager::IsOutputSlotConnectedToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const
{
	if (outputSlot == nullptr || inputSlot == nullptr) {
		return false;
	}
	DBGASSERT (outputToInputConnections.HasConnection (outputSlot, inputSlot) == inputToOutputConnections.HasConnection (inputSlot, outputSlot));
	return outputToInputConnections.HasConnection (outputSlot, inputSlot);
}

bool ConnectionManager::CanConnectOutputSlotToInputSlot (const InputSlotConstPtr& inputSlot) const
{
	if (DBGERROR (inputSlot == nullptr)) {
		return false;
	}

	OutputSlotConnectionMode outputSlotConnectionMode = inputSlot->GetOutputSlotConnectionMode ();

	bool canConnect = false;
	switch (outputSlotConnectionMode) {
		case OutputSlotConnectionMode::Disabled:
			// it's not possible to connect to a disabled input slot
			canConnect = false;
			break;
		case OutputSlotConnectionMode::Single:
			// you can always connect to single input slot,
			// the previous connection will be disconnected
			canConnect = true;
			break;
		case OutputSlotConnectionMode::Multiple:
			// you can always connect to multiple input slot
			canConnect = true;
			break;
		default:
			DBGBREAK ();
			break;
	}

	return canConnect;
}

bool ConnectionManager::CanConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const
{
	if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
		return false;
	}

	if (!CanConnectOutputSlotToInputSlot (inputSlot)) {
		return false;
	}

	DBGASSERT (outputToInputConnections.HasConnection (outputSlot, inputSlot) == inputToOutputConnections.HasConnection (inputSlot, outputSlot));
	if (outputToInputConnections.HasConnection (outputSlot, inputSlot)) {
		return false;
	}

	return true;
}

bool ConnectionManager::ConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot)
{
	if (DBGERROR (!CanConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
		return false;
	}
	if (inputSlot->GetOutputSlotConnectionMode () == OutputSlotConnectionMode::Single) {
		DisconnectAllOutputSlotsFromInputSlot (inputSlot);
	}
	inputToOutputConnections.AddConnection (inputSlot, outputSlot);
	outputToInputConnections.AddConnection (outputSlot, inputSlot);
	return true;
}

bool ConnectionManager::DisconnectOutputSlotFromInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot)
{
	if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
		return false;
	}
	inputToOutputConnections.DeleteConnection (inputSlot, outputSlot);
	outputToInputConnections.DeleteConnection (outputSlot, inputSlot);
	return true;
}

bool ConnectionManager::DisconnectAllOutputSlotsFromInputSlot (const InputSlotConstPtr& inputSlot)
{
	if (DBGERROR (inputSlot == nullptr)) {
		return false;
	}
	std::unordered_set<OutputSlotConstPtr> outputSlotsToDisconnect;
	EnumerateConnectedOutputSlots (inputSlot, [&] (const OutputSlotConstPtr& outputSlot) {
		outputSlotsToDisconnect.insert (outputSlot);
	});
	for (const OutputSlotConstPtr& outputSlot : outputSlotsToDisconnect) {
		DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot);
	}
	return true;
}

bool ConnectionManager::DisconnectAllInputSlotsFromOutputSlot (const OutputSlotConstPtr& outputSlot)
{
	if (DBGERROR (outputSlot == nullptr)) {
		return false;
	}
	std::unordered_set<InputSlotConstPtr> inputSlotsToDisconnect;
	EnumerateConnectedInputSlots (outputSlot, [&] (const InputSlotConstPtr& inputSlot) {
		inputSlotsToDisconnect.insert (inputSlot);
	});
	for (const InputSlotConstPtr& inputSlot : inputSlotsToDisconnect) {
		DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot);
	}
	return true;
}

}
