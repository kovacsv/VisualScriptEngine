#include "NE_OutputSlot.hpp"
#include "NE_Node.hpp"
#include "NE_Debug.hpp"
#include "NE_NodeManager.hpp"

namespace NE
{

DYNAMIC_SERIALIZATION_INFO (OutputSlot, 1, "{1F40DBE0-C463-46ED-A2CC-17C779C16802}");

OutputSlot::OutputSlot () :
	Slot ()
{

}

OutputSlot::OutputSlot (const SlotId& id) :
	Slot (id)
{

}

OutputSlot::~OutputSlot ()
{

}

ValueConstPtr OutputSlot::Evaluate (EvaluationEnv& env) const
{
	return EvaluateOwnerNode (env);
}

Stream::Status OutputSlot::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	Slot::Read (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status OutputSlot::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	Slot::Write (outputStream);
	return outputStream.GetStatus ();
}

ValueConstPtr OutputSlot::EvaluateOwnerNode (EvaluationEnv& env) const
{
	if (DBGERROR (!HasOwnerNode ())) {
		return nullptr;
	}
	return ownerNode->Evaluate (env);
}

}
