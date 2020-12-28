#ifndef NE_INPUTSLOT_HPP
#define NE_INPUTSLOT_HPP

#include "NE_NodeEngineTypes.hpp"
#include "NE_Slot.hpp"

namespace NE
{

enum class OutputSlotConnectionMode
{
	Disabled	= 0,
	Single		= 1,
	Multiple	= 2
};

class InputSlot : public Slot
{
	DYNAMIC_SERIALIZABLE (InputSlot);

public:
	InputSlot ();
	InputSlot (const SlotId& id, const ValueConstPtr& defaultValue, OutputSlotConnectionMode outputSlotConnectionMode);
	virtual ~InputSlot ();

	OutputSlotConnectionMode	GetOutputSlotConnectionMode () const;
	ValueConstPtr				GetDefaultValue () const;
	void						SetDefaultValue (const ValueConstPtr& newDefaultValue);
	
	virtual Stream::Status		Read (InputStream& inputStream) override;
	virtual Stream::Status		Write (OutputStream& outputStream) const override;

private:
	ValueConstPtr				defaultValue;
	OutputSlotConnectionMode	outputSlotConnectionMode;
};

}

#endif
