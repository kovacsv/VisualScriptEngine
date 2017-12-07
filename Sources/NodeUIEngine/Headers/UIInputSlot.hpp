#ifndef UIINPUTSLOT_HPP
#define UIINPUTSLOT_HPP

#include "InputSlot.hpp"

namespace NUIE
{

class InputSlotCommandRegistrator;

class UIInputSlot : public NE::InputSlot
{
	DYNAMIC_SERIALIZABLE (UIInputSlot);

public:
	UIInputSlot ();
	UIInputSlot (const NE::SlotId& id, const std::wstring& name, const NE::ValuePtr& defaultValue, NE::OutputSlotConnectionMode slotConnectionMode);
	~UIInputSlot ();

	const std::wstring&			GetName () const;
	void						SetName (const std::wstring& newName);

	virtual void				RegisterCommands (InputSlotCommandRegistrator& commandRegistrator);
	
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	std::wstring				name;
};

typedef std::shared_ptr<UIInputSlot> UIInputSlotPtr;
typedef std::shared_ptr<const UIInputSlot> UIInputSlotConstPtr;

}

#endif
