#ifndef NE_STAMP_HPP
#define NE_STAMP_HPP

#include <cstddef>

namespace NE
{

class Stamp
{
public:
	Stamp ();
	~Stamp ();

	void Update ();

	bool operator== (const Stamp& rhs) const;
	bool operator!= (const Stamp& rhs) const;

private:
	size_t counter;
};

}

#endif
