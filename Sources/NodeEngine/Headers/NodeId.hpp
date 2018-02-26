#ifndef NODEID_HPP
#define NODEID_HPP

#include "Serializable.hpp"

#include <atomic>
#include <functional>

namespace NE
{

typedef size_t NodeIdType;

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
	NodeId (const NodeIdType& uniqueId);
	~NodeId ();

	void			SetUniqueId (const NodeIdType& uniqueId);
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
