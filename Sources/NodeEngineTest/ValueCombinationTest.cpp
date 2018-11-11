#include "SimpleTest.hpp"
#include "NE_NodeEngineUtilities.hpp"
#include "NE_ValueCombination.hpp"
#include "NE_SingleValues.hpp"

using namespace NE;

namespace ValueCombinationTest
{

TEST (DegenerateCaseTests)
{
	{
		std::vector<std::vector<size_t>> variations;
		bool success = EnumerateVariationIndices ({}, [&] (const std::vector<size_t>& variation) {
			variations.push_back (variation);
			return true;
		});
		ASSERT (!success);
		ASSERT (variations.size () == 0);
	}
}

TEST (SimpleCaseTest)
{
	{
		std::vector<std::vector<size_t>> variations;
		bool success = EnumerateVariationIndices ({0}, [&] (const std::vector<size_t>& variation) {
			variations.push_back (variation);
			return true;
		});
		ASSERT (success);
		ASSERT (variations.size () == 1);
		ASSERT (variations[0] == std::vector<size_t> ({0}));
	}
	{
		std::vector<std::vector<size_t>> variations;
		bool success = EnumerateVariationIndices ({0, 0}, [&] (const std::vector<size_t>& variation) {
			variations.push_back (variation);
			return true;
		});
		ASSERT (success);
		ASSERT (variations.size () == 1);
		ASSERT (variations[0] == std::vector<size_t> ({ 0, 0 }));
	}
}

TEST (StoppingTest)
{
	std::vector<std::vector<size_t>> variations;
	bool success = EnumerateVariationIndices ({1, 1}, [&] (const std::vector<size_t>& variation) {
		variations.push_back (variation);
		return false;
	});
	ASSERT (!success);
	ASSERT (variations.size () == 1);
}

TEST (MultipleCaseTest)
{
	{
		std::vector<std::vector<size_t>> variations;
		bool success = EnumerateVariationIndices ({1}, [&] (const std::vector<size_t>& variation) {
			variations.push_back (variation);
			return true;
		});
		ASSERT (success);
		ASSERT (variations.size () == 2);
		ASSERT (variations[0] == std::vector<size_t> ({ 0 }));
		ASSERT (variations[1] == std::vector<size_t> ({ 1 }));
	}
	{
		std::vector<std::vector<size_t>> variations;
		bool success = EnumerateVariationIndices ({1, 1}, [&] (const std::vector<size_t>& variation) {
			variations.push_back (variation);
			return true;
		});
		ASSERT (success);
		ASSERT (variations.size () == 4);
		ASSERT (variations[0] == std::vector<size_t> ({ 0, 0 }));
		ASSERT (variations[1] == std::vector<size_t> ({ 0, 1 }));
		ASSERT (variations[2] == std::vector<size_t> ({ 1, 0 }));
		ASSERT (variations[3] == std::vector<size_t> ({ 1, 1 }));
	}
}

TEST (MultipleCaseTest2)
{
	std::vector<std::vector<size_t>> variations;
	bool success = EnumerateVariationIndices ({2, 2, 2}, [&] (const std::vector<size_t>& variation) {
		variations.push_back (variation);
		return true;
	});
	ASSERT (success);
	ASSERT (variations.size () == 27);
	ASSERT (variations[0] == std::vector<size_t> ({ 0, 0, 0 }));
	ASSERT (variations[1] == std::vector<size_t> ({ 0, 0, 1 }));
	ASSERT (variations[2] == std::vector<size_t> ({ 0, 0, 2 }));
	ASSERT (variations[3] == std::vector<size_t> ({ 0, 1, 0 }));
	ASSERT (variations[4] == std::vector<size_t> ({ 0, 1, 1 }));
	ASSERT (variations[5] == std::vector<size_t> ({ 0, 1, 2 }));
	ASSERT (variations[6] == std::vector<size_t> ({ 0, 2, 0 }));
	ASSERT (variations[26] == std::vector<size_t> ({ 2, 2, 2 }));
}

TEST (DifferentMaximumsTest)
{
	std::vector<std::vector<size_t>> variations;
	bool success = EnumerateVariationIndices ({0, 1, 2},
		[&] (const std::vector<size_t>& variation) {
			variations.push_back (variation);
			return true;
		}
	);
	ASSERT (success);
	ASSERT (variations.size () == 6);
	ASSERT (variations[0] == std::vector<size_t> ({ 0, 0, 0 }));
	ASSERT (variations[1] == std::vector<size_t> ({ 0, 0, 1 }));
	ASSERT (variations[2] == std::vector<size_t> ({ 0, 0, 2 }));
	ASSERT (variations[3] == std::vector<size_t> ({ 0, 1, 0 }));
	ASSERT (variations[4] == std::vector<size_t> ({ 0, 1, 1 }));
	ASSERT (variations[5] == std::vector<size_t> ({ 0, 1, 2 }));
}

TEST (DifferentMaximumsTest2)
{
	std::vector<std::vector<size_t>> variations;
	bool success = EnumerateVariationIndices({2, 3},
		[&] (const std::vector<size_t>& variation) {
			variations.push_back (variation);
			return true;
		}
	);
	ASSERT (success);
	ASSERT (variations.size () == 12);
	ASSERT (variations[0] == std::vector<size_t> ({ 0, 0 }));
	ASSERT (variations[1] == std::vector<size_t> ({ 0, 1 }));
	ASSERT (variations[2] == std::vector<size_t> ({ 0, 2 }));
	ASSERT (variations[3] == std::vector<size_t> ({ 0, 3 }));
	ASSERT (variations[4] == std::vector<size_t> ({ 1, 0 }));
	ASSERT (variations[5] == std::vector<size_t> ({ 1, 1 }));
	ASSERT (variations[6] == std::vector<size_t> ({ 1, 2 }));
	ASSERT (variations[7] == std::vector<size_t> ({ 1, 3 }));
	ASSERT (variations[8] == std::vector<size_t> ({ 2, 0 }));
	ASSERT (variations[9] == std::vector<size_t> ({ 2, 1 }));
	ASSERT (variations[10] == std::vector<size_t> ({ 2, 2 }));
	ASSERT (variations[11] == std::vector<size_t> ({ 2, 3 }));
}

TEST (DifferentMaximumsTest3)
{
	std::vector<size_t> maxIndices;
	for (int i = 0; i < 100; ++i) {
		if (i == 10) {
			maxIndices.push_back (99);
		} else {
			maxIndices.push_back (0);
		}
	}
	std::vector<std::vector<size_t>> variations;
	bool success = EnumerateVariationIndices (maxIndices,
		[&] (const std::vector<size_t>& variation) {
			variations.push_back (variation);
			return true;
		}
	);
	ASSERT (success);
	ASSERT (variations.size () == 100);
	for (size_t i = 0; i < variations.size (); ++i) {
		for (size_t j = 0; j < variations[i].size (); ++j) {
			if (j == 10) {
				ASSERT (variations[i][j] == i);
			} else {
				ASSERT (variations[i][j] == 0);
			}
		}
	}
}

TEST (ValueCombinationTest)
{
	ListValuePtr list (new ListValue ());
	list->Push (ValuePtr (new IntValue (1)));
	list->Push (ValuePtr (new IntValue (2)));
	list->Push (ValuePtr (new IntValue (3)));
	ValuePtr single (new IntValue (4));

	std::vector<ValuePtr> values = { list, single };

	{
		std::vector<std::vector<int>> combinations;
		bool success = CombineValues (ValueCombinationMode::Shortest, values, [&] (const ValueCombination& current) {
			ASSERT (current.GetSize () == 2);
			combinations.push_back ({ IntValue::Get (current.GetValue (0)), IntValue::Get (current.GetValue (1)) });
			return true;
		});
	
		ASSERT (success);
		ASSERT (combinations.size () == 1);
		ASSERT (combinations[0] == std::vector<int> ({ 1, 4 }));
	}
	{
		std::vector<std::vector<int>> combinations;
		bool success = CombineValues (ValueCombinationMode::Longest, values, [&] (const ValueCombination& current) {
			ASSERT (current.GetSize () == 2);
			combinations.push_back ({ IntValue::Get (current.GetValue (0)), IntValue::Get (current.GetValue (1)) });
			return true;
		});
	
		ASSERT (success);
		ASSERT (combinations.size () == 3);
		ASSERT (combinations[0] == std::vector<int> ({ 1, 4 }));
		ASSERT (combinations[1] == std::vector<int> ({ 2, 4 }));
		ASSERT (combinations[2] == std::vector<int> ({ 3, 4 }));
	}
	{
		std::vector<std::vector<int>> combinations;
		bool success = CombineValues (ValueCombinationMode::CrossProduct, values, [&] (const ValueCombination& current) {
			ASSERT (current.GetSize () == 2);
			combinations.push_back ({ IntValue::Get (current.GetValue (0)), IntValue::Get (current.GetValue (1)) });
			return true;
		});

		ASSERT (success);
		ASSERT (combinations.size () == 3);
		ASSERT (combinations[0] == std::vector<int> ({ 1, 4 }));
		ASSERT (combinations[1] == std::vector<int> ({ 2, 4 }));
		ASSERT (combinations[2] == std::vector<int> ({ 3, 4 }));
	}
}

TEST (ValueCombinationTest2)
{
	ListValuePtr list (new ListValue ());
	list->Push (ValuePtr (new IntValue (1)));
	list->Push (ValuePtr (new IntValue (2)));
	list->Push (ValuePtr (new IntValue (3)));
	ListValuePtr list2 (new ListValue ());
	list2->Push (ValuePtr (new IntValue (5)));
	list2->Push (ValuePtr (new IntValue (6)));

	std::vector<ValuePtr> values = { list, list2 };
	{
		std::vector<std::vector<int>> combinations;
		bool success = CombineValues (ValueCombinationMode::Shortest, values, [&] (const ValueCombination& current) {
			ASSERT (current.GetSize () == 2);
			combinations.push_back ({ IntValue::Get (current.GetValue (0)), IntValue::Get (current.GetValue (1)) });
			return true;
		});

		ASSERT (success);
		ASSERT (combinations.size () == 2);
		ASSERT (combinations[0] == std::vector<int> ({ 1, 5 }));
		ASSERT (combinations[1] == std::vector<int> ({ 2, 6 }));
	}
	{
		std::vector<std::vector<int>> combinations;
		bool success = CombineValues (ValueCombinationMode::Longest, values, [&] (const ValueCombination& current) {
			ASSERT (current.GetSize () == 2);
			combinations.push_back ({ IntValue::Get (current.GetValue (0)), IntValue::Get (current.GetValue (1)) });
			return true;
		});

		ASSERT (success);
		ASSERT (combinations.size () == 3);
		ASSERT (combinations[0] == std::vector<int> ({ 1, 5 }));
		ASSERT (combinations[1] == std::vector<int> ({ 2, 6 }));
		ASSERT (combinations[2] == std::vector<int> ({ 3, 6 }));
	}
	{
		std::vector<std::vector<int>> combinations;
		bool success = CombineValues (ValueCombinationMode::CrossProduct, values, [&] (const ValueCombination& current) {
			ASSERT (current.GetSize () == 2);
			combinations.push_back ({ IntValue::Get (current.GetValue (0)), IntValue::Get (current.GetValue (1)) });
			return true;
		});

		ASSERT (success);
		ASSERT (combinations.size () == 6);
		ASSERT (combinations[0] == std::vector<int> ({ 1, 5 }));
		ASSERT (combinations[1] == std::vector<int> ({ 1, 6 }));
		ASSERT (combinations[2] == std::vector<int> ({ 2, 5 }));
		ASSERT (combinations[3] == std::vector<int> ({ 2, 6 }));
		ASSERT (combinations[4] == std::vector<int> ({ 3, 5 }));
		ASSERT (combinations[5] == std::vector<int> ({ 3, 6 }));
	}
}

TEST (ValueCombinationTest3)
{
	ListValuePtr list (new ListValue ());
	list->Push (ValuePtr (new IntValue (1)));
	list->Push (ValuePtr (new IntValue (2)));
	list->Push (ValuePtr (new IntValue (3)));
	ValuePtr single (new IntValue (4));
	ListValuePtr list2 (new ListValue ());
	list2->Push (ValuePtr (new IntValue (5)));
	list2->Push (ValuePtr (new IntValue (6)));

	std::vector<ValuePtr> values = { list, single, list2 };
	{
		std::vector<std::vector<int>> combinations;
		bool success = CombineValues (ValueCombinationMode::Shortest, values, [&] (const ValueCombination& current) {
			ASSERT (current.GetSize () == 3);
			combinations.push_back ({ IntValue::Get (current.GetValue (0)), IntValue::Get (current.GetValue (1)), IntValue::Get (current.GetValue (2)) });
			return true;
		});

		ASSERT (success);
		ASSERT (combinations.size () == 1);
		ASSERT (combinations[0] == std::vector<int> ({ 1, 4, 5 }));
	}
	{
		std::vector<std::vector<int>> combinations;
		bool success = CombineValues (ValueCombinationMode::Longest, values, [&] (const ValueCombination& current) {
			ASSERT (current.GetSize () == 3);
			combinations.push_back ({ IntValue::Get (current.GetValue (0)), IntValue::Get (current.GetValue (1)), IntValue::Get (current.GetValue (2)) });
			return true;
		});

		ASSERT (success);
		ASSERT (combinations.size () == 3);
		ASSERT (combinations[0] == std::vector<int> ({ 1, 4, 5 }));
		ASSERT (combinations[1] == std::vector<int> ({ 2, 4, 6 }));
		ASSERT (combinations[2] == std::vector<int> ({ 3, 4, 6 }));
	}
	{
		std::vector<std::vector<int>> combinations;
		bool success = CombineValues (ValueCombinationMode::CrossProduct, values, [&] (const ValueCombination& current) {
			ASSERT (current.GetSize () == 3);
			combinations.push_back ({ IntValue::Get (current.GetValue (0)), IntValue::Get (current.GetValue (1)), IntValue::Get (current.GetValue (2)) });
			return true;
		});

		ASSERT (success);
		ASSERT (combinations.size () == 6);
		ASSERT (combinations[0] == std::vector<int> ({ 1, 4, 5 }));
		ASSERT (combinations[1] == std::vector<int> ({ 1, 4, 6 }));
		ASSERT (combinations[2] == std::vector<int> ({ 2, 4, 5 }));
		ASSERT (combinations[3] == std::vector<int> ({ 2, 4, 6 }));
		ASSERT (combinations[4] == std::vector<int> ({ 3, 4, 5 }));
		ASSERT (combinations[5] == std::vector<int> ({ 3, 4, 6 }));
	}
}

}
