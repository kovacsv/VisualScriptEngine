#ifndef NE_NODEGROUPID_HPP
#define NE_NODEGROUPID_HPP

#include "NE_Serializable.hpp"

#include <functional>

namespace NE
{

using NodeGroupIdType = size_t;

class NodeGroupId
{
	SERIALIZABLE;

public:
	NodeGroupId ();
	explicit NodeGroupId (const NodeGroupIdType& uniqueId);
	~NodeGroupId ();

	NodeGroupIdType		GetUniqueId () const;
	void				SetUniqueId (NodeGroupIdType uniqueId);
	size_t				GenerateHashValue () const;

	bool				operator< (const NodeGroupId& rhs) const;
	bool				operator> (const NodeGroupId& rhs) const;
	bool				operator== (const NodeGroupId& rhs) const;
	bool				operator!= (const NodeGroupId& rhs) const;

	Stream::Status		Read (InputStream& inputStream);
	Stream::Status		Write (OutputStream& outputStream) const;

private:
	NodeGroupIdType	id;
};

extern const NodeGroupId NullNodeGroupId;

}

namespace std
{
	template <>
	struct hash<NE::NodeGroupId>
	{
		size_t operator() (const NE::NodeGroupId& id) const noexcept
		{
			return id.GenerateHashValue ();
		}
	};
}

#endif
