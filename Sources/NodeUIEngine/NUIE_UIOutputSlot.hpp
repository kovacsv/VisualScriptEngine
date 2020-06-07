#ifndef NUIE_UIOUTPUTSLOT_HPP
#define NUIE_UIOUTPUTSLOT_HPP

#include "NE_LocString.hpp"
#include "NE_OutputSlot.hpp"

namespace NUIE
{

class OutputSlotCommandRegistrator;

class UIOutputSlot : public NE::OutputSlot
{
	DYNAMIC_SERIALIZABLE (UIOutputSlot);

public:
	UIOutputSlot ();
	UIOutputSlot (const NE::SlotId& id, const NE::LocString& name);
	~UIOutputSlot ();

	const NE::LocString&		GetName () const;
	void						SetName (const std::wstring& newName);

	virtual void				RegisterCommands (OutputSlotCommandRegistrator& commandRegistrator) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	NE::LocString				name;
};

using UIOutputSlotPtr = std::shared_ptr<UIOutputSlot>;
using UIOutputSlotConstPtr = std::shared_ptr<const UIOutputSlot>;

}

#endif
