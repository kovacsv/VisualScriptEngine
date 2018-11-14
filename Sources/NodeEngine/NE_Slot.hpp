#ifndef NE_SLOT_HPP
#define NE_SLOT_HPP

#include "NE_NodeEngineTypes.hpp"
#include "NE_Serializable.hpp"
#include "NE_NodeId.hpp"
#include "NE_SlotId.hpp"
#include "NE_Value.hpp"

namespace NE
{

class Slot : public DynamicSerializable
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
