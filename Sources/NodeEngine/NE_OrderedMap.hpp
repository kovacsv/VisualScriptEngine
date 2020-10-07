#ifndef NE_ORDEREDMAP_HPP
#define NE_ORDEREDMAP_HPP

#include "NE_Debug.hpp"

#include <vector>
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
	~OrderedMap ();

	bool			IsEmpty () const;
	bool			Contains (const Key& key) const;
	const Value&	GetValue (const Key& key) const;
	size_t			Count () const;

	bool			Insert (const Key& key, const Value& value);
	bool			InsertBefore (const Key& key, const Value& value, const Key& nextKey);
	bool			InsertAfter (const Key& key, const Value& value, const Key& prevKey);
	bool			Erase (const Key& key);
	void			Clear ();

	void			Enumerate (const std::function<bool (Value&)>& processor);
	void			Enumerate (const std::function<bool (const Value&)>& processor) const;

private:
	std::vector<Key>				keyList;
	std::unordered_map<Key, Value>	keyToValueTable;
};

template <typename Key, typename Value>
OrderedMap<Key, Value>::OrderedMap () :
	keyList (),
	keyToValueTable ()
{

}

template <typename Key, typename Value>
OrderedMap<Key, Value>::~OrderedMap ()
{

}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::IsEmpty () const
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	return keyList.empty ();
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::Contains (const Key& key) const
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	return keyToValueTable.find (key) != keyToValueTable.end ();
}

template <typename Key, typename Value>
const Value& OrderedMap<Key, Value>::GetValue (const Key& key) const
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	return keyToValueTable.at (key);
}

template <typename Key, typename Value>
size_t OrderedMap<Key, Value>::Count () const
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	return keyList.size ();
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::Insert (const Key& key, const Value& value)
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	if (DBGERROR (keyToValueTable.find (key) != keyToValueTable.end ())) {
		return false;
	}

	keyList.push_back (key);
	keyToValueTable.insert ({ key, value });
	return true;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::InsertBefore (const Key& key, const Value& value, const Key& nextKey)
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	if (DBGERROR (keyToValueTable.find (key) != keyToValueTable.end ())) {
		return false;
	}

	auto foundNextValue = keyToValueTable.find (nextKey);
	if (DBGERROR (foundNextValue == keyToValueTable.end ())) {
		return false;
	}

	auto foundNextKeyIndex = std::find (keyList.begin (), keyList.end (), nextKey);
	if (DBGERROR (foundNextKeyIndex == keyList.end ())) {
		return false;
	}

	keyList.insert (foundNextKeyIndex, key);
	keyToValueTable.insert ({ key, value });
	return true;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::InsertAfter (const Key& key, const Value& value, const Key& prevKey)
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	if (DBGERROR (keyToValueTable.find (key) != keyToValueTable.end ())) {
		return false;
	}

	auto foundPrevValue = keyToValueTable.find (prevKey);
	if (DBGERROR (foundPrevValue == keyToValueTable.end ())) {
		return false;
	}

	auto foundPrevKeyIndex = std::find (keyList.begin (), keyList.end (), prevKey);
	if (DBGERROR (foundPrevKeyIndex == keyList.end ())) {
		return false;
	}

	keyList.insert (foundPrevKeyIndex + 1, key);
	keyToValueTable.insert ({ key, value });
	return true;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::Erase (const Key& key)
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	auto foundInList = std::find (keyList.begin (), keyList.end (), key);
	if (DBGERROR (foundInList == keyList.end ())) {
		return false;
	}

	auto foundInTable = keyToValueTable.find (key);
	if (DBGERROR (foundInTable == keyToValueTable.end ())) {
		return false;
	}

	keyList.erase (foundInList);
	keyToValueTable.erase (foundInTable->first);
	return true;
}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::Clear ()
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	keyList.clear ();
	keyToValueTable.clear ();
}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::Enumerate (const std::function<bool (Value&)>& processor)
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	for (Key& key : keyList) {
		if (!processor (keyToValueTable.at (key))) {
			break;
		}
	}
}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::Enumerate (const std::function<bool (const Value&)>& processor) const
{
	DBGASSERT (keyList.size () == keyToValueTable.size ());
	for (const Key& key : keyList) {
		if (!processor (keyToValueTable.at (key))) {
			break;
		}
	}
}

}

#endif
