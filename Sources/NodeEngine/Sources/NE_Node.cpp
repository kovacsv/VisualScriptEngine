#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_Debug.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

SerializationInfo Node::serializationInfo (ObjectVersion (1));

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

InputSlotPtr Node::GetInputSlot (const SlotId& slotId)
{
	return inputSlots.Get (slotId);
}

OutputSlotPtr Node::GetOutputSlot (const SlotId& slotId)
{
	return outputSlots.Get (slotId);
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

void Node::EnumerateInputSlots (const std::function<bool (const InputSlotPtr&)>& processor)
{
	inputSlots.Enumerate (processor);
}

void Node::EnumerateOutputSlots (const std::function<bool (const OutputSlotPtr&)>& processor)
{
	outputSlots.Enumerate (processor);
}

void Node::EnumerateInputSlots (const std::function<bool (const InputSlotConstPtr&)>& processor) const
{
	inputSlots.Enumerate (processor);
}

void Node::EnumerateOutputSlots (const std::function<bool (const OutputSlotConstPtr&)>& processor) const
{
	outputSlots.Enumerate (processor);
}

ValuePtr Node::Evaluate (EvaluationEnv& env) const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return nullptr;
	}

	if (nodeEvaluator->IsNodeValueCalculated (nodeId)) {
		return nodeEvaluator->GetCalculatedNodeValue (nodeId);
	}

	ValuePtr value = Calculate (env);
	nodeEvaluator->SetCalculatedNodeValue (nodeId, value);

	CalculationPostProcess (value, env);
	return value;
}

ValuePtr Node::GetCalculatedValue () const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return nullptr;
	}

	if (DBGERROR (!nodeEvaluator->IsNodeValueCalculated (nodeId))) {
		return nullptr;
	}

	return nodeEvaluator->GetCalculatedNodeValue (nodeId);
}

void Node::InvalidateValue () const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return;
	}
	nodeEvaluator->InvalidateNodeValue (GetId ());	
}

bool Node::ValueIsCalculated () const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return false;
	}
	return nodeEvaluator->IsNodeValueCalculated (GetId ());
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

bool Node::RegisterInputSlot (const InputSlotPtr& newInputSlot)
{
	if (DBGERROR (newInputSlot->HasOwnerNode ())) {
		return false;
	}
	if (DBGERROR (HasInputSlot (newInputSlot->GetId ()))) {
		return false;
	}
	newInputSlot->SetOwnerNode (this);
	inputSlots.Push (newInputSlot);
	return true;
}

bool Node::RegisterOutputSlot (const OutputSlotPtr& newOutputSlot)
{
	if (DBGERROR (newOutputSlot->HasOwnerNode ())) {
		return false;
	}
	if (DBGERROR (HasOutputSlot (newOutputSlot->GetId ()))) {
		return false;
	}
	newOutputSlot->SetOwnerNode (this);
	outputSlots.Push (newOutputSlot);
	return true;
}

ValuePtr Node::EvaluateSingleInputSlot (const SlotId& slotId, EvaluationEnv& env) const
{
	if (DBGERROR (!HasInputSlot (slotId))) {
		return nullptr;
	}

	InputSlotConstPtr inputSlot = GetInputSlot (slotId);
	if (DBGERROR (inputSlot == nullptr)) {
		return nullptr;
	}

	OutputSlotConnectionMode outputSlotConnectionMode = inputSlot->GetOutputSlotConnectionMode ();
	if (DBGERROR (outputSlotConnectionMode == OutputSlotConnectionMode::Multiple)) {
		return nullptr;
	}

	ListValuePtr result = EvaluateInputSlot (inputSlot, env);
	if (DBGERROR (result->GetSize () != 1)) {
		return nullptr;
	}

	return result->GetValue (0);
}

ListValuePtr Node::EvaluateInputSlot (const SlotId& slotId, EvaluationEnv& env) const
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

void Node::Initialize ()
{
	RegisterSlots ();
}

void Node::CalculationPostProcess (const ValuePtr&, EvaluationEnv&) const
{

}

ListValuePtr Node::EvaluateInputSlot (const InputSlotConstPtr& inputSlot, EvaluationEnv& env) const
{
	if (DBGERROR (nodeEvaluator == nullptr)) {
		return nullptr;
	}

	ListValuePtr result (new ListValue ());
	if (!nodeEvaluator->HasConnectedOutputSlots (inputSlot)) {
		result->Push (inputSlot->GetDefaultValue ());
		return result;
	}

	nodeEvaluator->EnumerateConnectedOutputSlots (inputSlot, [&] (const OutputSlotConstPtr& outputSlot) {
		result->Push (outputSlot->Evaluate (env));
	});
	return result;
}

NodePtr Node::Clone (const NodeConstPtr& node)
{
	MemoryOutputStream outputStream;
	WriteDynamicObject (outputStream, node.get ());
	if (DBGERROR (node->Write (outputStream) != Stream::Status::NoError)) {
		return nullptr;
	}

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	NodePtr result (ReadDynamicObject<Node> (inputStream));
	if (DBGERROR (result == nullptr)) {
		return nullptr;
	}

	return result;
}

template <>
void Node::EnumerateSlots (const std::function<bool (const InputSlotPtr&)>& processor)
{
	EnumerateInputSlots (processor);
}

template <>
void Node::EnumerateSlots (const std::function<bool (const OutputSlotPtr&)>& processor)
{
	EnumerateOutputSlots (processor);
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
