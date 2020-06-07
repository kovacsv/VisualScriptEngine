#ifndef NUIE_UIDISPATCHEROUTPUTSLOT_HPP
#define NUIE_UIDISPATCHEROUTPUTSLOT_HPP

#include "NUIE_UIOutputSlot.hpp"

namespace NUIE
{

class UIDispatcherOutputSlot : public UIOutputSlot
{
	DYNAMIC_SERIALIZABLE (UIDispatcherOutputSlot);

public:
	UIDispatcherOutputSlot ();
	UIDispatcherOutputSlot (const NE::SlotId& id, const NE::LocString& name, size_t listIndex);
	~UIDispatcherOutputSlot ();

	virtual NE::ValueConstPtr	Evaluate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	size_t listIndex;
};

}

#endif
