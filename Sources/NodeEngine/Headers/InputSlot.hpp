#ifndef INPUTSLOT_HPP
#define INPUTSLOT_HPP

#include "NodeEngineTypes.hpp"
#include "Slot.hpp"

namespace NE
{

enum class OutputSlotConnectionMode
{
	Disabled,
	Single,
	Multiple
};

class InputSlot : public Slot
{
	DYNAMIC_SERIALIZABLE (InputSlot);

public:
	InputSlot ();
	InputSlot (const SlotId& id, const ValuePtr& defaultValue, OutputSlotConnectionMode outputSlotConnectionMode);
	virtual ~InputSlot ();

	OutputSlotConnectionMode	GetOutputSlotConnectionMode () const;
	ValuePtr					GetDefaultValue () const;
	void						SetDefaultValue (const ValuePtr& newDefaultValue);
	
	virtual Stream::Status		Read (InputStream& inputStream) override;
	virtual Stream::Status		Write (OutputStream& outputStream) const override;

private:
	ValuePtr					defaultValue;
	OutputSlotConnectionMode	outputSlotConnectionMode;
};

}

#endif
