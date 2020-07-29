#ifndef NE_CACHE_HPP
#define NE_CACHE_HPP

#include <list>
#include <unordered_map>

namespace NE
{

template <typename KeyType, typename ValueType>
class Cache
{
public:
	class Controller
	{
	public:
		Controller ();
		virtual ~Controller ();
		
		virtual ValueType	CreateValue (const KeyType& key) = 0;
		virtual void		DisposeValue (ValueType& value) = 0;
	};

	Cache (size_t maxSize);
	Cache (size_t maxSize, Controller* controller);
	Cache (const Cache& rhs) = delete;
	virtual ~Cache ();

	Cache&	operator= (const Cache& rhs) = delete;

	bool				Contains (const KeyType& key) const;
	void				Clear ();

	bool				Add (const KeyType& key, const ValueType& value);
	const ValueType&	Get (const KeyType& key);

private:
	using KeyValuePair = std::pair<KeyType, ValueType>;
	using KeyValuePairIterator = typename std::list<KeyValuePair>::iterator;

	size_t												maxSize;
	Controller*											controller;
	std::list<KeyValuePair>								valueList;
	std::unordered_map<KeyType, KeyValuePairIterator>	valueMap;
};

template <typename KeyType, typename ValueType>
Cache<KeyType, ValueType>::Controller::Controller () 
{

}

template <typename KeyType, typename ValueType>
Cache<KeyType, ValueType>::Controller::~Controller ()
{

}

template <typename KeyType, typename ValueType>
Cache<KeyType, ValueType>::Cache (size_t maxSize) :
	Cache (maxSize, nullptr)
{

}

template <typename KeyType, typename ValueType>
Cache<KeyType, ValueType>::Cache (size_t maxSize, Controller* controller) :
	maxSize (maxSize),
	controller (controller),
	valueList (),
	valueMap ()
{

}

template <typename KeyType, typename ValueType>
Cache<KeyType, ValueType>::~Cache ()
{
	Clear ();
}

template <typename KeyType, typename ValueType>
bool Cache<KeyType, ValueType>::Contains (const KeyType& key) const
{
	return valueMap.find (key) != valueMap.end ();
}

template <typename KeyType, typename ValueType>
void Cache<KeyType, ValueType>::Clear ()
{
	if (controller != nullptr) {
		for (KeyValuePair& it : valueList) {
			controller->DisposeValue (it.second);
		}
	}
	valueList.clear ();
	valueMap.clear ();
}

template <typename KeyType, typename ValueType>
bool Cache<KeyType, ValueType>::Add (const KeyType& key, const ValueType& value)
{
	if (Contains (key)) {
		return false;
	}
	if (valueList.size () >= maxSize) {
		KeyValuePair& keyValuePair = valueList.front ();
		if (controller != nullptr) {
			controller->DisposeValue (keyValuePair.second);
		}
		valueMap.erase (keyValuePair.first);
		valueList.pop_front ();
	}
	valueList.push_back ({ key, value });
	valueMap.insert ({ key, std::prev (valueList.end ()) });
	return true;
}

template <typename KeyType, typename ValueType>
const ValueType& Cache<KeyType, ValueType>::Get (const KeyType& key)
{
	if (controller != nullptr && !Contains (key)) {
		Add (key, controller->CreateValue (key));
	}
	const KeyValuePair& pair = *valueMap.at (key);
	return pair.second;
}

}

#endif
