#ifndef NUIE_HASHUTILS_HPP
#define NUIE_HASHUTILS_HPP

#include <cstddef>

struct EnumHash
{
	template <typename EnumType>
	std::size_t operator() (EnumType enumVal) const
	{
		return static_cast<std::size_t> (enumVal);
	}
};

#endif
