#ifndef NODEID_HPP
#define NODEID_HPP

#include "Serializable.hpp"

#include <atomic>
#include <functional>

namespace NE
{

typedef size_t NodeIdType;

class NodeIdGenerator : public Serializable
{
	SERIALIZABLE;

public:
	NodeIdGenerator ();
	~NodeIdGenerator ();

	NodeIdType				GenerateUniqueId ();

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

private:
	std::atomic<size_t> nextId;
};

class NodeId : public Serializable
{
	SERIALIZABLE;

public:
	NodeId ();
	NodeId (const NodeIdType& uniqueId);
	~NodeId ();

	void					SetUniqueId (const NodeIdType& uniqueId);
	size_t					GenerateHashValue () const;

	bool					operator< (const NodeId& rhs) const;
	bool					operator> (const NodeId& rhs) const;
	bool					operator== (const NodeId& rhs) const;
	bool					operator!= (const NodeId& rhs) const;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

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
