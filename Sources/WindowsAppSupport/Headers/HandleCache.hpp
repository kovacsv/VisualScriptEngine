#ifndef HANDLECACHE_HPP
#define HANDLECACHE_HPP

#include <windows.h>
#include <unordered_map>

template <typename KeyType>
class HandleCache
{
public:
	HandleCache ()
	{
	
	}

	~HandleCache ()
	{
		for (auto it : cache) {
			DeleteObject (it.second);
		}	
	}

	HANDLE Get (const KeyType& key)
	{
		KeyType cacheKey (key);
		auto found = cache.find (cacheKey);
		if (found != cache.end ()) {
			return found->second;
		}
		HANDLE keyHandle = cacheKey.CreateHandle ();
		cache.insert ({ cacheKey, keyHandle });
		return keyHandle;
	}

private:
	std::unordered_map<KeyType, HANDLE> cache;
};

#endif
