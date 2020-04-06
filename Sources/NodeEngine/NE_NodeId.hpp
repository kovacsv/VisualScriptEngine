#ifndef NE_NODEID_HPP
#define NE_NODEID_HPP

#include "NE_Serializable.hpp"

#include <atomic>
#include <functional>

namespace NE
{

using NodeIdType = size_t;

class NodeIdGenerator
{
	SERIALIZABLE;

public:
	NodeIdGenerator ();
	~NodeIdGenerator ();

	NodeIdType		GenerateUniqueId ();

	Stream::Status	Read (InputStream& inputStream);
	Stream::Status	Write (OutputStream& outputStream) const;

private:
	std::atomic<size_t> nextId;
};

class NodeId
{
	SERIALIZABLE;

public:
	NodeId ();
	explicit NodeId (const NodeIdType& uniqueId);
	~NodeId ();

	NodeIdType		GetUniqueId () const;
	void			SetUniqueId (NodeIdType uniqueId);
	size_t			GenerateHashValue () const;

	bool			operator< (const NodeId& rhs) const;
	bool			operator> (const NodeId& rhs) const;
	bool			operator== (const NodeId& rhs) const;
	bool			operator!= (const NodeId& rhs) const;

	Stream::Status	Read (InputStream& inputStream);
	Stream::Status	Write (OutputStream& outputStream) const;

private:
	NodeIdType	id;
};

extern const NodeId NullNodeId;

}

namespace std
{
	template <>
	struct hash<NE::NodeId>
	{
		size_t operator() (const NE::NodeId& id) const noexcept
		{
			return id.GenerateHashValue ();
		}
	};
}

#endif
