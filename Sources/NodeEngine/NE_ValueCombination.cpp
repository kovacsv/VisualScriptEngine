#include "NE_ValueCombination.hpp"
#include "NE_NodeEngineUtilities.hpp"
#include "NE_Value.hpp"
#include "NE_Debug.hpp"

#include <algorithm>

namespace NE
{

ValueCombination::ValueCombination ()
{

}

ValueCombination::~ValueCombination ()
{

}

static bool EnumerateShortestCombinations (	const std::vector<IListValueConstPtr>& values,
											const std::function<bool (const ValueCombination&)>& processor)
{
	class ShortestPairValueCombination : public ValueCombination
	{
	public:
		ShortestPairValueCombination (size_t combinationIndex, const std::vector<IListValueConstPtr>& values) :
			combinationIndex (combinationIndex),
			values (values)
		{

		}

		virtual size_t GetSize () const override
		{
			return values.size ();
		}

		virtual const ValueConstPtr& GetValue (size_t valueIndex) const override
		{
			return values[valueIndex]->GetValue (combinationIndex);
		}

	private:
		size_t									combinationIndex;
		const std::vector<IListValueConstPtr>&	values;
	};

	size_t minSize = std::numeric_limits<size_t>::max ();
	for (const IListValueConstPtr& value : values) {
		size_t currentSize = value->GetSize ();
		minSize = std::min (currentSize, minSize);
	}

	for (size_t combinationIndex = 0; combinationIndex < minSize; ++combinationIndex) {
		ShortestPairValueCombination valueCombination (combinationIndex, values);
		if (!processor (valueCombination)) {
			return false;
		}
	}

	return true;
}

static bool EnumerateLongestCombinations (	const std::vector<IListValueConstPtr>& values,
											const std::function<bool (const ValueCombination&)>& processor)
{
	class LongestPairValueCombination : public ValueCombination
	{
	public:
		LongestPairValueCombination (size_t combinationIndex, const std::vector<IListValueConstPtr>& values) :
			combinationIndex (combinationIndex),
			values (values)
		{

		}

		virtual size_t GetSize () const override
		{
			return values.size ();
		}

		virtual const ValueConstPtr& GetValue (size_t valueIndex) const override
		{
			const IListValueConstPtr& value = values[valueIndex];
			if (combinationIndex < value->GetSize ()) {
				return value->GetValue (combinationIndex);
			} else {
				return value->GetValue (value->GetSize () - 1);
			}
		}

	private:
		size_t									combinationIndex;
		const std::vector<IListValueConstPtr>&	values;
	};

	size_t maxSize = 0;
	for (const IListValueConstPtr& value : values) {
		size_t currentSize = value->GetSize ();
		maxSize = std::max (currentSize, maxSize);
	}
	
	for (size_t combinationIndex = 0; combinationIndex < maxSize; ++combinationIndex) {
		LongestPairValueCombination valueCombination (combinationIndex, values);
		if (!processor (valueCombination)) {
			return false;
		}
	}

	return true;
}

static bool EnumerateCrossProductCombinations (	const std::vector<IListValueConstPtr>& values,
												const std::function<bool (const ValueCombination&)>& processor)
{
	class CrossProductValueCombination : public ValueCombination
	{
	public:
		CrossProductValueCombination (const std::vector<IListValueConstPtr>& values, const std::vector<size_t>& indices) :
			values (values),
			indices (indices)
		{

		}

		virtual size_t GetSize () const override
		{
			return values.size ();
		}

		virtual const ValueConstPtr& GetValue (size_t valueIndex) const override
		{
			return values[valueIndex]->GetValue (indices[valueIndex]);
		}

	private:
		const std::vector<IListValueConstPtr>&	values;
		const std::vector<size_t>&				indices;
	};

	std::vector<size_t> maxIndices;
	for (const IListValueConstPtr& value : values) {
		if (value->GetSize () == 0) {
			return false;
		}
		maxIndices.push_back (value->GetSize () - 1);
	}

	bool result = EnumerateVariationIndices (maxIndices,
		[&] (const std::vector<size_t>& indices) {
			CrossProductValueCombination valueCombination (values, indices);
			return processor (valueCombination);
		}
	);

	return result;
}

bool CombineValues (ValueCombinationMode combinationMode, const std::vector<ValueConstPtr>& values,
					const std::function<bool (const ValueCombination&)>& processor)
{
	std::vector<IListValueConstPtr> listValues;
	for (const ValueConstPtr& value : values) {
		IListValueConstPtr listValue = CreateListValue (value);
		if (DBGERROR (listValue == nullptr || listValue->GetSize () == 0)) {
			return false;
		}
		listValues.push_back (listValue);
	}

	if (combinationMode == ValueCombinationMode::Shortest) {
		return EnumerateShortestCombinations (listValues, processor);
	} else if (combinationMode == ValueCombinationMode::Longest) {
		return EnumerateLongestCombinations (listValues, processor);
	} else if (combinationMode == ValueCombinationMode::CrossProduct) {
		return EnumerateCrossProductCombinations (listValues, processor);
	}

	DBGBREAK ();
	return false;
}

}
