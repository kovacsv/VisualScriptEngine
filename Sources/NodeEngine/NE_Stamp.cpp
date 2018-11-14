#include "NE_Stamp.hpp"

namespace NE
{

Stamp::Stamp () :
	counter (0)
{
}

Stamp::~Stamp ()
{
}

void Stamp::Update ()
{
	counter++;
}

bool Stamp::operator== (const Stamp& rhs) const
{
	return counter == rhs.counter;
}

bool Stamp::operator!= (const Stamp& rhs) const
{
	return !operator== (rhs);
}

}
