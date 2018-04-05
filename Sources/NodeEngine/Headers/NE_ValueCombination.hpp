#ifndef VALUECOMBINATION_HPP
#define VALUECOMBINATION_HPP

#include "NE_Value.hpp"

#include <functional>
#include <vector>

namespace NE
{

enum class ValueCombinationMode
{
	Shortest,
	Longest,
	CrossProduct
};

class ValueCombination
{
public:
	ValueCombination ();
	~ValueCombination ();

	virtual size_t				GetSize () const = 0;
	virtual const ValuePtr&		GetValue (size_t index) const = 0;
};

bool CombineValues (ValueCombinationMode combinationMode, const std::vector<ValuePtr>& values,
					const std::function<void (const ValueCombination&)>& processor);

}

#endif
