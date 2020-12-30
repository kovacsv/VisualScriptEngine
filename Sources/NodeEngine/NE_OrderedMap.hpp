#ifndef NE_ORDEREDMAP_HPP
#define NE_ORDEREDMAP_HPP

#include "NE_Debug.hpp"

#include <utility>
#include <list>
#include <unordered_map>
#include <functional>
#include <algorithm>

namespace NE
{

template <typename Key, typename Value>
class OrderedMap
{
public:
	OrderedMap ();
	OrderedMap (const OrderedMap& rhs);
	OrderedMap (OrderedMap&& rhs);
	~OrderedMap ();

	OrderedMap&		operator= (const OrderedMap& rhs);
	OrderedMap&		operator= (OrderedMap&& rhs);

	bool			IsEmpty () const;
	bool			Contains (const Key& key) const;
	size_t			Count () const;

	Value&			GetValue (const Key& key);
	const Value&	GetValue (const Key& key) const;

	bool			Insert (const Key& key, const Value& value);
	bool			InsertBefore (const Key& key, const Value& value, const Key& nextKey);
	bool			InsertAfter (const Key& key, const Value& value, const Key& prevKey);
	void			MakeSorted ();
	bool			Erase (const Key& key);
	void			Clear ();

	void			Enumerate (const std::function<bool (Value&)>& processor);
	void			Enumerate (const std::function<bool (const Value&)>& processor) const;

private:
	using KeyValue	= std::pair<Key, Value>;
	using List		= std::list<KeyValue>;
	using Iterator	= typename List::iterator;

	List								valueList;
	std::unordered_map<Key, Iterator>	keyToValueMap;
};

template <typename Key, typename Value>
OrderedMap<Key, Value>::OrderedMap () :
	valueList (),
	keyToValueMap ()
{

}

template <typename Key, typename Value>
OrderedMap<Key, Value>::OrderedMap (const OrderedMap& rhs) :
	valueList (rhs.valueList),
	keyToValueMap ()
{
	for (auto it = valueList.begin (); it != valueList.end (); ++it) {
		const KeyValue& keyValue = *it;
		keyToValueMap.insert ({ keyValue.first, it });
	}
}

template <typename Key, typename Value>
OrderedMap<Key, Value>::OrderedMap (OrderedMap&& rhs) :
	valueList (std::move (rhs.valueList)),
	keyToValueMap (std::move (rhs.keyToValueMap))
{
	
}

template <typename Key, typename Value>
OrderedMap<Key, Value>::~OrderedMap ()
{

}

template <typename Key, typename Value>
OrderedMap<Key, Value>& OrderedMap<Key, Value>::operator= (const OrderedMap& rhs)
{
	if (this != &rhs) {
		valueList = rhs.valueList;
		keyToValueMap.clear ();
		for (auto it = valueList.begin (); it != valueList.end (); ++it) {
			const KeyValue& keyValue = *it;
			keyToValueMap.insert ({ keyValue.first, it });
		}
	}
	return *this;
}

template <typename Key, typename Value>
OrderedMap<Key, Value>& OrderedMap<Key, Value>::operator= (OrderedMap&& rhs)
{
	if (this != &rhs) {
		valueList = std::move (rhs.valueList);
		keyToValueMap = std::move (rhs.keyToValueMap);
	}
	return *this;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::IsEmpty () const
{
	return keyToValueMap.empty ();
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::Contains (const Key& key) const
{
	return keyToValueMap.find (key) != keyToValueMap.end ();
}

template <typename Key, typename Value>
size_t OrderedMap<Key, Value>::Count () const
{
	return keyToValueMap.size ();
}

template <typename Key, typename Value>
Value& OrderedMap<Key, Value>::GetValue (const Key& key)
{
	Iterator& iterator = keyToValueMap.at (key);
	KeyValue& keyValue = *iterator;
	return keyValue.second;
}

template <typename Key, typename Value>
const Value& OrderedMap<Key, Value>::GetValue (const Key& key) const
{
	const Iterator& iterator = keyToValueMap.at (key);
	const KeyValue& keyValue = *iterator;
	return keyValue.second;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::Insert (const Key& key, const Value& value)
{
	if (DBGERROR (keyToValueMap.find (key) != keyToValueMap.end ())) {
		return false;
	}

	auto inserted = valueList.insert (valueList.end (), { key, value } );
	keyToValueMap.insert ({ key, inserted });
	return true;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::InsertBefore (const Key& key, const Value& value, const Key& nextKey)
{
	if (DBGERROR (keyToValueMap.find (key) != keyToValueMap.end ())) {
		return false;
	}

	auto foundNextValue = keyToValueMap.find (nextKey);
	if (DBGERROR (foundNextValue == keyToValueMap.end ())) {
		return false;
	}
	
	auto inserted = valueList.insert (foundNextValue->second, { key, value });
	keyToValueMap.insert ({ key, inserted });
	return true;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::InsertAfter (const Key& key, const Value& value, const Key& prevKey)
{
	if (DBGERROR (keyToValueMap.find (key) != keyToValueMap.end ())) {
		return false;
	}

	auto foundPrevValue = keyToValueMap.find (prevKey);
	if (DBGERROR (foundPrevValue == keyToValueMap.end ())) {
		return false;
	}

	auto inserted = valueList.insert (std::next (foundPrevValue->second), { key, value });
	keyToValueMap.insert ({ key, inserted });
	return true;
}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::MakeSorted ()
{
	valueList.sort ([&] (const KeyValue& a, const KeyValue& b) {
		return a.first < b.first;
	});
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::Erase (const Key& key)
{
	auto foundInMap = keyToValueMap.find (key);
	if (DBGERROR (foundInMap == keyToValueMap.end ())) {
		return false;
	}

	valueList.erase (foundInMap->second);
	keyToValueMap.erase (key);
	return true;
}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::Clear ()
{
	valueList.clear ();
	keyToValueMap.clear ();
}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::Enumerate (const std::function<bool (Value&)>& processor)
{
	for (KeyValue& keyValue : valueList) {
		if (!processor (keyValue.second)) {
			break;
		}
	}
}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::Enumerate (const std::function<bool (const Value&)>& processor) const
{
	for (const KeyValue& keyValue : valueList) {
		if (!processor (keyValue.second)) {
			break;
		}
	}
}

}

#endif
