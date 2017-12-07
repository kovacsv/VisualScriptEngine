#ifndef SLOTID_HPP
#define SLOTID_HPP

#include "Serializable.hpp"

#include <string>

namespace NE
{

class SlotId : public Serializable
{
	SERIALIZABLE;

public:
	SlotId ();
	SlotId (const std::string& id);
	~SlotId ();

	size_t					GenerateHashValue () const;

	bool					operator== (const SlotId& rhs) const;
	bool					operator!= (const SlotId& rhs) const;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

private:
	std::string id;
};

}

namespace std
{
	template <>
	struct hash<NE::SlotId>
	{
		size_t operator() (const NE::SlotId& id) const noexcept
		{
			return id.GenerateHashValue ();
		}
	};
}

#endif
