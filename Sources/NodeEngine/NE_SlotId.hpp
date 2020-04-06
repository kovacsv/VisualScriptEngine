#ifndef NE_SLOTID_HPP
#define NE_SLOTID_HPP

#include "NE_Serializable.hpp"

#include <string>

namespace NE
{

class SlotId
{
	SERIALIZABLE;

public:
	SlotId ();
	explicit SlotId (const std::string& id);
	~SlotId ();

	size_t			GenerateHashValue () const;

	bool			operator< (const SlotId& rhs) const;
	bool			operator> (const SlotId& rhs) const;
	bool			operator== (const SlotId& rhs) const;
	bool			operator!= (const SlotId& rhs) const;

	Stream::Status	Read (InputStream& inputStream);
	Stream::Status	Write (OutputStream& outputStream) const;

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
