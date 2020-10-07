#include "SimpleTest.hpp"
#include "NE_OrderedMap.hpp"

using namespace NE;

namespace OrderedMapTest
{

static std::vector<std::string> GetEnumeratedValues (const OrderedMap<int, std::string>& map)
{
	std::vector<std::string> enumeratedValues;
	map.Enumerate ([&] (const std::string& value) {
		enumeratedValues.push_back (value);
		return true;
	});
	return enumeratedValues;
}

TEST (OrderedMapEmptyTest)
{
	OrderedMap<int, std::string> map;
	ASSERT (map.IsEmpty ());
	ASSERT (map.Count () == 0);
}

TEST (OrderedMapInsertClearTest)
{
	OrderedMap<int, std::string> map;
	ASSERT (map.Insert (5, "five"));
	ASSERT (!map.IsEmpty ());
	ASSERT (map.Count () == 1);
	ASSERT (!map.Contains (4));
	ASSERT (map.Contains (5));
	map.Clear ();
	ASSERT (!map.Contains (5));
	ASSERT (map.IsEmpty ());
	ASSERT (map.Count () == 0);
}

TEST (OrderedMapInsertEraseTest)
{
	OrderedMap<int, std::string> map;
	ASSERT (map.Insert (4, "four"));
	ASSERT (map.Insert (5, "five"));
	ASSERT (map.Count () == 2);
	ASSERT (map.Contains (4));
	ASSERT (map.Contains (5));
	ASSERT (map.Erase (4));
	ASSERT (!map.Contains (4));
	ASSERT (map.Contains (5));
	ASSERT (map.Count () == 1);
}

TEST (OrderedMapEnumerateTest)
{
	OrderedMap<int, std::string> map;
	std::vector<std::string> numbers = { "one", "two", "three", "four", "five" };
	for (int i = 1; i <= 5; i++) {
		ASSERT (map.Insert (i, numbers[i - 1]));
	}
	for (int i = 1; i <= 5; i++) {
		ASSERT (map.Contains (i));
		ASSERT (map.GetValue (i) == numbers[i - 1]);
	}

	std::vector<std::string> enumeratedNumbers;
	map.Enumerate ([&] (const std::string& number) {
		enumeratedNumbers.push_back (number);
		return true;
	});

	ASSERT (numbers == enumeratedNumbers);
}

TEST (OrderedMapOrderAfterDeleteTest)
{
	OrderedMap<int, std::string> map;
	std::vector<std::string> numbers = { "one", "two", "three", "four", "five" };
	std::vector<std::string> numbersAfterDelete = { "one", "two", "four", "five" };
	for (int i = 1; i <= 5; i++) {
		ASSERT (map.Insert (i, numbers[i - 1]));
	}
	map.Erase (3);
	std::vector<std::string> enumeratedNumbers;
	map.Enumerate ([&] (const std::string& number) {
		enumeratedNumbers.push_back (number);
		return true;
	});

	ASSERT (numbersAfterDelete == enumeratedNumbers);
}

TEST (OrderedMapOrderInsertBeforeAfterTest)
{
	OrderedMap<int, std::string> map;
	std::vector<std::string> numbers = { "one", "two", "three" };
	for (int i = 1; i <= 3; i++) {
		ASSERT (map.Insert (i, numbers[i - 1]));
	}

	{
		OrderedMap<int, std::string> newMap = map;
		ASSERT (newMap.InsertBefore (4, "four", 1));
		ASSERT (GetEnumeratedValues (newMap) == std::vector<std::string> ({ "four", "one", "two", "three" }));
	}

	{
		OrderedMap<int, std::string> newMap = map;
		ASSERT (newMap.InsertBefore (4, "four", 3));
		ASSERT (GetEnumeratedValues (newMap) == std::vector<std::string> ({ "one", "two", "four", "three" }));
	}

	{
		OrderedMap<int, std::string> newMap = map;
		ASSERT (newMap.InsertAfter (4, "four", 1));
		ASSERT (GetEnumeratedValues (newMap) == std::vector<std::string> ({ "one", "four", "two", "three" }));
	}

	{
		OrderedMap<int, std::string> newMap = map;
		ASSERT (newMap.InsertAfter (4, "four", 3));
		ASSERT (GetEnumeratedValues (newMap) == std::vector<std::string> ({ "one", "two", "three", "four" }));
	}
}

}
