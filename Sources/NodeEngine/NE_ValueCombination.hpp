#ifndef NE_VALUECOMBINATION_HPP
#define NE_VALUECOMBINATION_HPP

#include "NE_Value.hpp"

#include <functional>
#include <vector>

namespace NE
{

enum class ValueCombinationMode
{
	Shortest		= 0,
	Longest			= 1,
	CrossProduct	= 2
};

class ValueCombination
{
public:
	ValueCombination ();
	virtual ~ValueCombination ();

	virtual size_t					GetSize () const = 0;
	virtual const ValueConstPtr&	GetValue (size_t index) const = 0;
};

bool CombineValues (ValueCombinationMode combinationMode, const std::vector<ValueConstPtr>& values,
					const std::function<bool (const ValueCombination&)>& processor);

}

#endif
