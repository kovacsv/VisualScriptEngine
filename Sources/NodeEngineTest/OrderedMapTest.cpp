#include "SimpleTest.hpp"
#include "NE_OrderedMap.hpp"

using namespace NE;

namespace OrderedMapTest
{

OrderedMap<int, std::string> MoveTest ()
{
	OrderedMap<int, std::string> map;
	map.Insert (5, "five");
	map.Insert (6, "six");
	return map;
}

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

TEST (OrderedMapConstEnumerateTest)
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

TEST (OrderedMapNonConstTest)
{
	OrderedMap<int, std::string> map;
	std::vector<std::string> numbers = { "one", "two", "three", "four", "five" };
	for (int i = 1; i <= 5; i++) {
		ASSERT (map.Insert (i, numbers[i - 1]));
	}

	map.Enumerate ([&] (std::string& number) {
		number = "reset";
		return true;
	});

	for (int i = 1; i <= 5; i++) {
		ASSERT (map.Contains (i));
		ASSERT (map.GetValue (i) == "reset");
	}

	std::vector<std::string> enumeratedNumbers;
	map.Enumerate ([&] (const std::string& number) {
		enumeratedNumbers.push_back (number);
		return true;
	});

	ASSERT (enumeratedNumbers == std::vector<std::string> ({ "reset", "reset", "reset", "reset", "reset" }));

	std::string& threeVal = map.GetValue (3);
	threeVal = "newThreeVal";
	ASSERT (map.GetValue (3) == "newThreeVal");
	enumeratedNumbers.clear ();
	map.Enumerate ([&] (const std::string& number) {
		enumeratedNumbers.push_back (number);
		return true;
	});
	ASSERT (enumeratedNumbers == std::vector<std::string> ({ "reset", "reset", "newThreeVal", "reset", "reset" }));
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

TEST (OrderedMapOrderInsertBeforeTest)
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
}

TEST (OrderedMapOrderInsertAfterTest)
{
	OrderedMap<int, std::string> map;
	std::vector<std::string> numbers = { "one", "two", "three" };
	for (int i = 1; i <= 3; i++) {
		ASSERT (map.Insert (i, numbers[i - 1]));
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

TEST (OrderedMapCopyTest)
{
	OrderedMap<int, std::string> map;
	std::vector<std::string> numbers = { "one", "two", "three" };
	for (int i = 1; i <= 3; i++) {
		ASSERT (map.Insert (i, numbers[i - 1]));
	}

	OrderedMap<int, std::string> map2 = map;
	OrderedMap<int, std::string> map3;
	map3.Insert (5, "five");
	map3 = map;
	
	ASSERT (GetEnumeratedValues (map) == std::vector<std::string> ({ "one", "two", "three" }));
	ASSERT (GetEnumeratedValues (map2) == std::vector<std::string> ({ "one", "two", "three" }));
	ASSERT (GetEnumeratedValues (map3) == std::vector<std::string> ({ "one", "two", "three" }));
	ASSERT (map.GetValue (2) == "two");
	ASSERT (map2.GetValue (2) == "two");
	ASSERT (map3.GetValue (2) == "two");
}

TEST (OrderedMapMoveTest)
{
	OrderedMap<int, std::string> map = MoveTest ();
	ASSERT (GetEnumeratedValues (map) == std::vector<std::string> ({ "five", "six" }));
	ASSERT (map.GetValue (5) == "five");
	ASSERT (map.GetValue (6) == "six");

	OrderedMap<int, std::string> map2;
	map2.Insert (5, "five");
	map2 = std::move (map);

	ASSERT (GetEnumeratedValues (map2) == std::vector<std::string> ({ "five", "six" }));
	ASSERT (map2.GetValue (5) == "five");
	ASSERT (map2.GetValue (6) == "six");
}

TEST (OrderedMapMakeSortedTest)
{
	OrderedMap<int, std::string> map;
	std::vector<std::string> numbers = { "one", "two", "three", "four", "five" };
	ASSERT (map.Insert (3, numbers[2]));
	ASSERT (map.Insert (1, numbers[0]));
	ASSERT (map.Insert (4, numbers[3]));
	ASSERT (map.Insert (2, numbers[1]));
	ASSERT (map.Insert (5, numbers[4]));
	ASSERT (GetEnumeratedValues (map) == std::vector<std::string> ({ "three", "one", "four", "two", "five" }));

	map.MakeSorted ();
	ASSERT (GetEnumeratedValues (map) == std::vector<std::string> ({ "one", "two", "three", "four", "five" }));
}

}
