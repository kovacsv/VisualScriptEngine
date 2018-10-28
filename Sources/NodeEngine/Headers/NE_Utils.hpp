#ifndef NE_UTILS_HPP
#define NE_UTILS_HPP

#include <string>

namespace NE
{

template <typename T>
class ValueGuard
{
public:
	ValueGuard (T& variable, const T& newValue) :
		variable (variable),
		origValue (variable)
	{
		variable = newValue;
	}

	~ValueGuard ()
	{
		variable = origValue;
	}

private:
	T&	variable;
	T	origValue;
};

}

#endif
