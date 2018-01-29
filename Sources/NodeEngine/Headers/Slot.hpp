#ifndef SLOT_HPP
#define SLOT_HPP

#include "NodeEngineTypes.hpp"
#include "Serializable.hpp"
#include "NodeId.hpp"
#include "SlotId.hpp"
#include "Value.hpp"

namespace NE
{

class Slot : public Serializable
{
	SERIALIZABLE;

public:
	Slot ();
	Slot (const SlotId& slotId);
	Slot (const Slot& src) = delete;
	virtual ~Slot ();

	const SlotId&			GetId () const;

	bool					HasOwnerNode () const;
	NodeId					GetOwnerNodeId () const;
	bool					SetOwnerNode (Node* newOwnerNode);

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

protected:
	SlotId	slotId;
	Node*	ownerNode;
};

}

#endif
