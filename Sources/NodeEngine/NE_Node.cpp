#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_Debug.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

SERIALIZATION_INFO (Node, 1);

NodeEvaluator::NodeEvaluator ()
{

}

NodeEvaluator::~NodeEvaluator ()
{

}

NodeEvaluatorSetter::NodeEvaluatorSetter ()
{

}

NodeEvaluatorSetter::~NodeEvaluatorSetter ()
{

}

Node::Node () :
	nodeId (NullNodeId),
	nodeEvaluator (nullptr)
{

}

Node::~Node ()
{

}

bool Node::IsEmpty () const
{
	return nodeId == NullNodeId && inputSlots.IsEmpty () && outputSlots.IsEmpty ();
}

const NodeId& Node::GetId () const
{
	return nodeId;
}

bool Node::HasInputSlot (const SlotId& slotId) const
{
	return inputSlots.Contains (slotId);
}

bool Node::HasOutputSlot (const SlotId& slotId) const
{
	return outputSlots.Contains (slotId);
}

InputSlotConstPtr Node::GetInputSlot (const SlotId& slotId) const
{
	return inputSlots.Get (slotId);
}

OutputSlotConstPtr Node::GetOutputSlot (const SlotId& slotId) const
{
	return outputSlots.Get (slotId);
}

size_t Node::GetInputSlotCount () const
{
	return inputSlots.Count ();
}

size_t Node::GetOutputSlotCount () const
{
	return outputSlots.Count ();
}

void Node::EnumerateInputSlots (const std::function<bool (const InputSlotConstPtr&)>& processor) const
{
	inputSlots.Enumerate (processor);
}

void Node::EnumerateOutputSlots (const std::function<bool (const OutputSlotConstPtr&)>& processor) const
{
	outputSlots.Enumerate (processor);
}

ValueConstPtr Node::Evaluate (EvaluationEnv& env) const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return nullptr;
	}

	CalculationStatus calcStatus = GetCalculationStatus ();
	if (calcStatus == CalculationStatus::Calculated) {
		return nodeEvaluator->GetCalculatedNodeValue (nodeId);
	}

	if (calcStatus != CalculationStatus::NeedToCalculate) {
		return nullptr;
	}

	ValueConstPtr value = Calculate (env);
	nodeEvaluator->SetCalculatedNodeValue (nodeId, value);
	ProcessCalculatedValue (value, env);

	return value;
}

ValueConstPtr Node::GetCalculatedValue () const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return nullptr;
	}

	if (DBGERROR (!nodeEvaluator->HasCalculatedNodeValue (nodeId))) {
		return nullptr;
	}

	return nodeEvaluator->GetCalculatedNodeValue (nodeId);
}

bool Node::HasCalculatedValue () const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return false;
	}
	return nodeEvaluator->HasCalculatedNodeValue (GetId ());
}

Node::CalculationStatus Node::GetCalculationStatus () const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return CalculationStatus::NeedToCalculate;
	}

	if (nodeEvaluator->HasCalculatedNodeValue (nodeId)) {
		return CalculationStatus::Calculated;
	}

	if (nodeEvaluator->IsCalculationEnabled () || IsForceCalculated ()) {
		return CalculationStatus::NeedToCalculate;
	} else {
		return CalculationStatus::NeedToCalculateButDisabled;
	}
}

void Node::InvalidateValue () const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return;
	}
	nodeEvaluator->InvalidateNodeValue (GetId ());	
}

Stream::Status Node::Read (InputStream& inputStream)
{
	if (DBGERROR (!IsEmpty ())) {
		return Stream::Status::Error;
	}

	ObjectHeader header (inputStream);
	nodeId.Read (inputStream);
	size_t inputSlotCount = 0;
	inputStream.Read (inputSlotCount);
	for (size_t i = 0; i < inputSlotCount; ++i) {
		InputSlotPtr inputSlot (ReadDynamicObject<InputSlot> (inputStream));
		if (DBGERROR (inputSlot == nullptr)) {
			return Stream::Status::Error;
		}
		if (DBGERROR (!RegisterInputSlot (inputSlot))) {
			return Stream::Status::Error;
		}
	}
	size_t outputSlotCount = 0;
	inputStream.Read (outputSlotCount);
	for (size_t i = 0; i < outputSlotCount; ++i) {
		OutputSlotPtr outputSlot (ReadDynamicObject<OutputSlot> (inputStream));
		if (DBGERROR (outputSlot == nullptr)) {
			return Stream::Status::Error;
		}
		if (DBGERROR (!RegisterOutputSlot (outputSlot))) {
			return Stream::Status::Error;
		}
	}
	return inputStream.GetStatus ();
}

Stream::Status Node::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	nodeId.Write (outputStream);
	outputStream.Write (inputSlots.Count ());
	inputSlots.Enumerate ([&] (const InputSlotConstPtr& inputSlot) {
		WriteDynamicObject (outputStream, inputSlot.get ());
		return true;
	});
	outputStream.Write (outputSlots.Count ());
	outputSlots.Enumerate ([&] (const OutputSlotConstPtr& outputSlot) {
		WriteDynamicObject (outputStream, outputSlot.get ());
		return true;
	});
	return outputStream.GetStatus ();
}

ValueConstPtr Node::GetInputSlotDefaultValue (const SlotId& slotId) const
{
	InputSlotConstPtr inputSlot = inputSlots.Get (slotId);
	if (DBGERROR (inputSlot == nullptr)) {
		return nullptr;
	}
	return inputSlot->GetDefaultValue ();
}

void Node::SetInputSlotDefaultValue (const SlotId& slotId, const ValueConstPtr& newDefaultValue)
{
	InputSlotPtr inputSlot = inputSlots.Get (slotId);
	if (DBGERROR (inputSlot == nullptr)) {
		return;
	}
	inputSlot->SetDefaultValue (newDefaultValue);
}

bool Node::RegisterInputSlot (const InputSlotPtr& newInputSlot)
{
	if (DBGERROR (inputSlots.Contains (newInputSlot->GetId ()))) {
		return false;
	}
	if (DBGERROR (newInputSlot->HasOwnerNode ())) {
		return false;
	}
	if (DBGERROR (!inputSlots.Insert (newInputSlot))) {
		return false;
	}
	if (DBGERROR (!newInputSlot->SetOwnerNode (this))) {
		return false;
	}
	return true;
}

bool Node::RegisterOutputSlot (const OutputSlotPtr& newOutputSlot)
{
	if (DBGERROR (outputSlots.Contains (newOutputSlot->GetId ()))) {
		return false;
	}
	if (DBGERROR (newOutputSlot->HasOwnerNode ())) {
		return false;
	}
	if (DBGERROR (!outputSlots.Insert (newOutputSlot))) {
		return false;
	}
	if (DBGERROR (!newOutputSlot->SetOwnerNode (this))) {
		return false;
	}
	return true;
}

ValueConstPtr Node::EvaluateInputSlot (const SlotId& slotId, EvaluationEnv& env) const
{
	if (DBGERROR (!HasInputSlot (slotId))) {
		return nullptr;
	}

	InputSlotConstPtr inputSlot = GetInputSlot (slotId);
	if (DBGERROR (inputSlot == nullptr)) {
		return nullptr;
	}

	return EvaluateInputSlot (inputSlot, env);
}

InputSlotPtr Node::GetModifiableInputSlot (const SlotId& slotId)
{
	return inputSlots.Get (slotId);
}

OutputSlotPtr Node::GetModifiableOutputSlot (const SlotId& slotId)
{
	return outputSlots.Get (slotId);
}

void Node::SetNodeEvaluator (const NodeEvaluatorSetter& evaluatorSetter)
{
	nodeId = evaluatorSetter.GetNodeId ();
	nodeEvaluator = evaluatorSetter.GetNodeEvaluator ();
	if (evaluatorSetter.GetInitializationMode () == InitializationMode::Initialize) {
		Initialize ();
	}
}

bool Node::HasNodeEvaluator () const
{
	return nodeEvaluator != nullptr;
}

void Node::ClearNodeEvaluator ()
{
	nodeId = NullNodeId;
	nodeEvaluator = nullptr;
}

bool Node::IsForceCalculated () const
{
	return false;
}

void Node::ProcessCalculatedValue (const ValueConstPtr&, EvaluationEnv&) const
{

}

ValueConstPtr Node::EvaluateInputSlot (const InputSlotConstPtr& inputSlot, EvaluationEnv& env) const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return nullptr;
	}

	if (!nodeEvaluator->HasConnectedOutputSlots (inputSlot)) {
		return inputSlot->GetDefaultValue ();
	}

	std::vector<OutputSlotConstPtr> connectedOutputSlots;
	nodeEvaluator->EnumerateConnectedOutputSlots (inputSlot, [&] (const OutputSlotConstPtr& outputSlot) {
		connectedOutputSlots.push_back (outputSlot);
	});

	OutputSlotConnectionMode outputSlotConnectionMode = inputSlot->GetOutputSlotConnectionMode ();
	if (outputSlotConnectionMode == OutputSlotConnectionMode::Single) {
		DBGASSERT (connectedOutputSlots.size () == 1);
		return connectedOutputSlots[0]->Evaluate (env);
	} else if (inputSlot->GetOutputSlotConnectionMode () == OutputSlotConnectionMode::Multiple) {
		ListValuePtr result (new ListValue ());
		for (const OutputSlotConstPtr& outputSlot : connectedOutputSlots) {
			result->Push (outputSlot->Evaluate (env));
		}
		return result;
	}

	DBGBREAK ();
	return nullptr;
}

NodePtr Node::Clone (const NodeConstPtr& node)
{
	MemoryOutputStream outputStream;
	if (DBGERROR (!WriteDynamicObject (outputStream, node.get ()))) {
		return nullptr;
	}

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	NodePtr result (ReadDynamicObject<Node> (inputStream));
	if (DBGERROR (result == nullptr)) {
		return nullptr;
	}

	return result;
}

bool Node::IsEqual (const NodeConstPtr& aNode, const NodeConstPtr& bNode)
{
	MemoryOutputStream aStream;
	MemoryOutputStream bStream;

	aNode->Write (aStream);
	bNode->Write (bStream);

	return aStream.GetBuffer () == bStream.GetBuffer ();
}

template <>
void Node::EnumerateSlots (const std::function<bool (const InputSlotConstPtr&)>& processor) const
{
	EnumerateInputSlots (processor);
}

template <>
void Node::EnumerateSlots (const std::function<bool (const OutputSlotConstPtr&)>& processor) const
{
	EnumerateOutputSlots (processor);
}

}
