#include "NE_Checksum.hpp"
#include <memory.h>
#include <cstddef>

namespace NE
{

static_assert (sizeof (int) == 4, "invalid size for int");
static_assert (sizeof (double) == 8, "invalid size for double");

Checksum::Checksum () :
	checksum (0),
	counter (0)
{

}

Checksum::~Checksum ()
{

}

void Checksum::Add (char val)
{
	AddInteger ((int) val);
}

void Checksum::Add (wchar_t val)
{
	AddInteger ((int) val);
}

void Checksum::Add (int val)
{
	AddInteger (val);
}

void Checksum::Add (double val)
{
	int64_t intVal = 0;
	memcpy (&intVal, &val, sizeof (intVal));
	AddInteger ((int) (intVal & 0xffffffff));
	AddInteger ((int) ((intVal >> 32) & 0xffffffff));
}

void Checksum::Add (const std::string& val)
{
	for (char ch : val)	{
		Add (ch);
	}
}

void Checksum::Add (const std::wstring& val)
{
	for (wchar_t ch : val)	{
		Add (ch);
	}	
}

bool Checksum::operator== (const Checksum& rhs) const
{
	return checksum == rhs.checksum && counter == rhs.counter;
}

bool Checksum::operator!= (const Checksum& rhs) const
{
	return !operator== (rhs);
}

void Checksum::AddInteger (int val)
{
	// BSD checksum algorithm
	checksum = (checksum >> 1) + ((checksum & 1) << 15);
	checksum = (checksum + val) & 0xffffffff;
	counter++;
}

}
