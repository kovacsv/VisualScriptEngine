#ifndef UIOUTPUTSLOT_HPP
#define UIOUTPUTSLOT_HPP

#include "NE_OutputSlot.hpp"

namespace NUIE
{

class OutputSlotCommandRegistrator;

class UIOutputSlot : public NE::OutputSlot
{
	DYNAMIC_SERIALIZABLE (UIOutputSlot);

public:
	UIOutputSlot ();
	UIOutputSlot (const NE::SlotId& id, const std::wstring& name);
	~UIOutputSlot ();

	const std::wstring&			GetName () const;
	void						SetName (const std::wstring& newName);

	virtual void				RegisterCommands (OutputSlotCommandRegistrator& commandRegistrator);

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	std::wstring				name;
};

typedef std::shared_ptr<UIOutputSlot> UIOutputSlotPtr;
typedef std::shared_ptr<const UIOutputSlot> UIOutputSlotConstPtr;

}

#endif
