#ifndef NE_SLOTLIST_HPP
#define NE_SLOTLIST_HPP

#include "NE_Debug.hpp"
#include "NE_OrderedMap.hpp"
#include "NE_SlotId.hpp"

#include <vector>
#include <unordered_map>

#include <memory>
#include <functional>
#include <algorithm>

namespace NE
{

template <class SlotType>
class SlotList
{
public:
	SlotList ();

	bool								Insert (const std::shared_ptr<SlotType>& slot);
	bool								InsertBefore (const std::shared_ptr<SlotType>& slot, const SlotId& nextSlotId);
	bool								InsertAfter (const std::shared_ptr<SlotType>& slot, const SlotId& prevSlotId);

	std::shared_ptr<SlotType>			Get (const SlotId& slotId);
	std::shared_ptr<const SlotType>		Get (const SlotId& slotId) const;
	bool								Contains (const SlotId& slotId) const;
	size_t								Count () const;
	bool								IsEmpty () const;

	void								Enumerate (const std::function<bool (const std::shared_ptr<SlotType>&)>& processor);
	void								Enumerate (const std::function<bool (const std::shared_ptr<const SlotType>&)>& processor) const;

private:
	OrderedMap<SlotId, std::shared_ptr<SlotType>>	slots;
};

template <class SlotType>
SlotList<SlotType>::SlotList () :
	slots ()
{

}

template <class SlotType>
bool SlotList<SlotType>::Insert (const std::shared_ptr<SlotType>& slot)
{
	return slots.Insert (slot->GetId (), slot);
}

template <class SlotType>
bool SlotList<SlotType>::InsertBefore (const std::shared_ptr<SlotType>& slot, const SlotId& nextSlotId)
{
	return slots.InsertBefore (slot->GetId (), slot, nextSlotId);
}

template <class SlotType>
bool SlotList<SlotType>::InsertAfter (const std::shared_ptr<SlotType>& slot, const SlotId& prevSlotId)
{
	return slots.InsertAfter (slot->GetId (), slot, prevSlotId);
}

template <class SlotType>
std::shared_ptr<SlotType> SlotList<SlotType>::Get (const SlotId& slotId)
{
	if (DBGERROR (!slots.Contains (slotId))) {
		return nullptr;
	}
	return slots.GetValue (slotId);
}

template <class SlotType>
std::shared_ptr<const SlotType> SlotList<SlotType>::Get (const SlotId& slotId) const
{
	if (DBGERROR (!slots.Contains (slotId))) {
		return nullptr;
	}
	return slots.GetValue (slotId);
}

template <class SlotType>
bool SlotList<SlotType>::Contains (const SlotId& slotId) const
{
	return slots.Contains (slotId);
}

template <class SlotType>
size_t SlotList<SlotType>::Count () const
{
	return slots.Count ();
}

template <class SlotType>
bool SlotList<SlotType>::IsEmpty () const
{
	return slots.IsEmpty ();
}

template <class SlotType>
void SlotList<SlotType>::Enumerate (const std::function<bool (const std::shared_ptr<SlotType>&)>& processor)
{
	slots.Enumerate ([&] (const std::shared_ptr<SlotType>& slot) {
		return processor (slot);
	});
}

template <class SlotType>
void SlotList<SlotType>::Enumerate (const std::function<bool (const std::shared_ptr<const SlotType>&)>& processor) const
{
	slots.Enumerate ([&] (const std::shared_ptr<SlotType>& slot) {
		return processor (slot);
	});
}

}

#endif
