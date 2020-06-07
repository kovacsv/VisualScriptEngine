#ifndef NUIE_UIINPUTSLOT_HPP
#define NUIE_UIINPUTSLOT_HPP

#include "NE_LocString.hpp"
#include "NE_InputSlot.hpp"

namespace NUIE
{

class InputSlotCommandRegistrator;

class UIInputSlot : public NE::InputSlot
{
	DYNAMIC_SERIALIZABLE (UIInputSlot);

public:
	UIInputSlot ();
	UIInputSlot (const NE::SlotId& id, const NE::LocString& name, const NE::ValuePtr& defaultValue, NE::OutputSlotConnectionMode slotConnectionMode);
	~UIInputSlot ();

	const NE::LocString&		GetName () const;
	void						SetName (const std::wstring& newName);

	virtual void				RegisterCommands (InputSlotCommandRegistrator& commandRegistrator) const;
	
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	NE::LocString				name;
};

using UIInputSlotPtr = std::shared_ptr<UIInputSlot>;
using UIInputSlotConstPtr = std::shared_ptr<const UIInputSlot>;

}

#endif
