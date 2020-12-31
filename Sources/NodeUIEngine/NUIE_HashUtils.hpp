#ifndef NUIE_HASHUTILS_HPP
#define NUIE_HASHUTILS_HPP

struct EnumHash
{
	template <typename EnumType>
	size_t operator() (EnumType enumVal) const
	{
		return static_cast<size_t> (enumVal);
	}
};

#endif
