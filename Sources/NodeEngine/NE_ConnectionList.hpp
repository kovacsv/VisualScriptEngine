#ifndef NE_CONNECTIONLIST_HPP
#define NE_CONNECTIONLIST_HPP

#include "NE_Debug.hpp"
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>

namespace NE
{

template <class BegSlotType, class EndSlotType>
class ConnectionList
{
public:
	ConnectionList ();

	void	Clear ();
	bool	IsEmpty () const;
	size_t	GetConnectionCount () const;
	size_t	GetConnectionCount (const BegSlotType& begSlot) const;

	bool	HasConnection (const BegSlotType& begSlot) const;
	bool	HasConnection (const BegSlotType& begSlot, const EndSlotType& endSlot) const;
	void	EnumerateConnections (const BegSlotType& begSlot, const std::function<void (const EndSlotType&)>& processor) const;

	void	AddConnection (const BegSlotType& begSlot, const EndSlotType& endSlot);
	void	DeleteConnection (const BegSlotType& begSlot, const EndSlotType& endSlot);

private:
	std::unordered_map<BegSlotType, std::vector<EndSlotType>> connections;
};

template <class BegSlotType, class EndSlotType>
ConnectionList<BegSlotType, EndSlotType>::ConnectionList ()
{

}

template <class BegSlotType, class EndSlotType>
void ConnectionList<BegSlotType, EndSlotType>::Clear ()
{
	connections.clear ();
}

template <class BegSlotType, class EndSlotType>
bool ConnectionList<BegSlotType, EndSlotType>::IsEmpty () const
{
	return GetConnectionCount () == 0;
}

template <class BegSlotType, class EndSlotType>
size_t ConnectionList<BegSlotType, EndSlotType>::GetConnectionCount () const
{
	size_t result = 0;
	for (const auto& connection : connections) {
		result += connection.second.size ();
	}
	return result;
}

template <class BegSlotType, class EndSlotType>
size_t ConnectionList<BegSlotType, EndSlotType>::GetConnectionCount (const BegSlotType& begSlot) const
{
	auto foundEndSlots = connections.find (begSlot);
	if (foundEndSlots == connections.end ()) {
		return 0;
	}
	const std::vector<EndSlotType>& endSlots = foundEndSlots->second;
	return endSlots.size ();
}

template <class BegSlotType, class EndSlotType>
bool ConnectionList<BegSlotType, EndSlotType>::HasConnection (const BegSlotType& begSlot) const
{
	auto foundEndSlots = connections.find (begSlot);
	if (foundEndSlots == connections.end ()) {
		return false;
	}
	return !foundEndSlots->second.empty ();
}

template <class BegSlotType, class EndSlotType>
bool ConnectionList<BegSlotType, EndSlotType>::HasConnection (const BegSlotType& begSlot, const EndSlotType& endSlot) const
{
	auto foundEndSlots = connections.find (begSlot);
	if (foundEndSlots == connections.end ()) {
		return false;
	}
	const std::vector<EndSlotType>& endSlots = foundEndSlots->second;
	return std::find (endSlots.begin (), endSlots.end (), endSlot) != endSlots.end ();
}

template <class BegSlotType, class EndSlotType>
void ConnectionList<BegSlotType, EndSlotType>::EnumerateConnections (const BegSlotType& begSlot, const std::function<void (const EndSlotType&)>& processor) const
{
	auto foundEndSlots = connections.find (begSlot);
	if (foundEndSlots == connections.end ()) {
		return;
	}
	const std::vector<EndSlotType>& endSlots = foundEndSlots->second;
	for (const EndSlotType& endSlot : endSlots) {
		processor (endSlot);
	}
}

template <class BegSlotType, class EndSlotType>
void ConnectionList<BegSlotType, EndSlotType>::AddConnection (const BegSlotType& begSlot, const EndSlotType& endSlot)
{
	DBGASSERT (!HasConnection (begSlot, endSlot));
	auto foundEndSlots = connections.find (begSlot);
	if (foundEndSlots == connections.end ()) {
		connections.insert ({ begSlot, { endSlot } });
	} else {
		foundEndSlots->second.push_back (endSlot);
	}
}

template <class BegSlotType, class EndSlotType>
void ConnectionList<BegSlotType, EndSlotType>::DeleteConnection (const BegSlotType& begSlot, const EndSlotType& endSlot)
{
	DBGASSERT (HasConnection (begSlot, endSlot));
	auto foundEndSlots = connections.find (begSlot);
	if (DBGVERIFY (foundEndSlots != connections.end ())) {
		auto foundEndSlot = std::find (foundEndSlots->second.begin (), foundEndSlots->second.end (), endSlot);
		foundEndSlots->second.erase (foundEndSlot);
		if (foundEndSlots->second.empty ()) {
			connections.erase (begSlot);
		}
	}
}

}

#endif
