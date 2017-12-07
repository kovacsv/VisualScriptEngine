#ifndef OUTPUTSLOT_HPP
#define OUTPUTSLOT_HPP

#include "Slot.hpp"
#include "Value.hpp"

namespace NE
{

class OutputSlot : public Slot
{
	DYNAMIC_SERIALIZABLE (OutputSlot);

public:
	OutputSlot ();
	OutputSlot (const SlotId& id);
	virtual ~OutputSlot ();

	virtual ValuePtr		Evaluate (EvaluationEnv& env) const;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

protected:
	ValuePtr				EvaluateOwnerNode (EvaluationEnv& env) const;
};

}

#endif
