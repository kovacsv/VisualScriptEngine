#ifndef SLOTLIST_HPP
#define SLOTLIST_HPP

#include "Debug.hpp"
#include "Stream.hpp"
#include "Serializable.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

namespace NE
{

template <class SlotType>
class SlotList
{
public:
	SlotList ();

	void								Push (const std::shared_ptr<SlotType>& slot);
	
	std::shared_ptr<SlotType>			Get (const SlotId& slotId);
	std::shared_ptr<const SlotType>		Get (const SlotId& slotId) const;
	bool								Contains (const SlotId& slotId) const;
	size_t								Count () const;
	bool								IsEmpty () const;

	void								Enumerate (const std::function<bool (const std::shared_ptr<SlotType>&)>& processor);
	void								Enumerate (const std::function<bool (const std::shared_ptr<const SlotType>&)>& processor) const;

private:
	std::vector<SlotId>										slotIdList;
	std::unordered_map<SlotId, std::shared_ptr<SlotType>>	slotIdMap;
};

template <class SlotType>
SlotList<SlotType>::SlotList ()
{

}

template <class SlotType>
void SlotList<SlotType>::Push (const std::shared_ptr<SlotType>& slot)
{
	slotIdList.push_back (slot->GetId ());
	slotIdMap.insert ({ slot->GetId (), slot });
}

template <class SlotType>
std::shared_ptr<SlotType> SlotList<SlotType>::Get (const SlotId& slotId)
{
	auto foundSlot = slotIdMap.find (slotId);
	if (DBGERROR (foundSlot == slotIdMap.end ())) {
		return nullptr;
	}
	return foundSlot->second;
}

template <class SlotType>
std::shared_ptr<const SlotType> SlotList<SlotType>::Get (const SlotId& slotId) const
{
	auto foundSlot = slotIdMap.find (slotId);
	if (DBGERROR (foundSlot == slotIdMap.end ())) {
		return nullptr;
	}
	return foundSlot->second;
}

template <class SlotType>
bool SlotList<SlotType>::Contains (const SlotId& slotId) const
{
	return slotIdMap.find (slotId) != slotIdMap.end ();
}

template <class SlotType>
size_t SlotList<SlotType>::Count () const
{
	return slotIdList.size ();
}

template <class SlotType>
bool SlotList<SlotType>::IsEmpty () const
{
	return slotIdList.size () == 0;
}

template <class SlotType>
void SlotList<SlotType>::Enumerate (const std::function<bool (const std::shared_ptr<SlotType>&)>& processor)
{
	for (const SlotId& slotId : slotIdList) {
		if (!processor (Get (slotId))) {
			break;
		}
	}
}

template <class SlotType>
void SlotList<SlotType>::Enumerate (const std::function<bool (const std::shared_ptr<const SlotType>&)>& processor) const
{
	for (const SlotId& slotId : slotIdList) {
		if (!processor (Get (slotId))) {
			break;
		}
	}
}

}

#endif
