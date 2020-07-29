#include "SimpleTest.hpp"
#include "NE_Cache.hpp"

#include <string>

using namespace NE;

namespace CacheTest
{

class TestController : public Cache<int, std::string>::Controller
{
public:
	TestController () :
		Cache<int, std::string>::Controller (),
		createCount (0),
		disposeCount (0)
	{

	}

	virtual std::string CreateValue (const int& key) override
	{
		createCount++;
		return std::to_string (key);
	}

	virtual void DisposeValue (std::string& /*value*/) override
	{
		disposeCount++;
	}

	int createCount;
	int disposeCount;
};

TEST (SimpleCacheTest)
{
	Cache<int, std::string> cache (3);
	ASSERT (!cache.Contains (1));
	ASSERT (!cache.Contains (2));
	ASSERT (cache.Add (1, "1"));
	ASSERT (cache.Add (2, "2"));
	ASSERT (!cache.Add (2, "2"));
	ASSERT (cache.Contains (1));
	ASSERT (cache.Contains (2));
	ASSERT (cache.Get (1) == "1");
	ASSERT (cache.Get (2) == "2");
}

TEST (CacheMaxSizeTest)
{
	Cache<int, std::string> cache (3);
	ASSERT (cache.Add (1, "1"));
	ASSERT (cache.Add (2, "2"));
	ASSERT (cache.Add (3, "3"));
	ASSERT (cache.Contains (1));
	ASSERT (cache.Contains (2));
	ASSERT (cache.Contains (3));
	ASSERT (cache.Add (4, "4"));
	ASSERT (!cache.Contains (1));
	ASSERT (cache.Contains (2));
	ASSERT (cache.Contains (3));
	ASSERT (cache.Contains (4));
}

TEST (CacheWithController)
{
	TestController controller;
	{
		Cache<int, std::string> cache (3, &controller);
		ASSERT (cache.Get (1) == "1");
		ASSERT (cache.Get (2) == "2");
		ASSERT (cache.Get (3) == "3");
		ASSERT (cache.Get (4) == "4");
		ASSERT (!cache.Contains (1));
		ASSERT (cache.Contains (2));
		ASSERT (cache.Contains (3));
		ASSERT (cache.Contains (4));
		ASSERT (controller.createCount == 4);
		ASSERT (controller.disposeCount == 1);
	}

	ASSERT (controller.createCount == 4);
	ASSERT (controller.disposeCount == 4);
}

TEST (CacheWithControllerLarge)
{
	TestController controller;
	{
		Cache<int, std::string> cache (3, &controller);
		for (int i = 1; i <= 100; i++) {
			ASSERT (cache.Get (i) == std::to_string (i));
		}
		ASSERT (cache.Contains (98));
		ASSERT (cache.Contains (99));
		ASSERT (cache.Contains (100));
		ASSERT (controller.createCount == 100);
		ASSERT (controller.disposeCount == 97);

		cache.Clear ();
		ASSERT (controller.createCount == 100);
		ASSERT (controller.disposeCount == 100);
	}

	ASSERT (controller.createCount == 100);
	ASSERT (controller.disposeCount == 100);
}

}
