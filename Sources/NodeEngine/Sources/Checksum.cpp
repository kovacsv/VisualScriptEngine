#include "Checksum.hpp"
#include <memory.h>
#include <cstddef>

namespace NE
{

static_assert (sizeof (int) == 4, "");
static_assert (sizeof (double) == 8, "");

Checksum::Checksum () :
	checksum (0)
{

}

Checksum::~Checksum ()
{

}

void Checksum::Add (char val)
{
	Add ((int) val);
}

void Checksum::Add (wchar_t val)
{
	Add ((int) val);
}

void Checksum::Add (int val)
{
	// BSD checksum algorithm
	checksum = (checksum >> 1) + ((checksum & 1) << 31);
	checksum = (checksum + val) & 0xffffffff;
}

void Checksum::Add (double val)
{
	int64_t intVal = 0;
	memcpy (&intVal, &val, sizeof (intVal));
	Add ((int) (intVal & 0xffffffff));
	Add ((int) ((intVal >> 32) & 0xffffffff));
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
	return checksum == rhs.checksum;
}

bool Checksum::operator!= (const Checksum& rhs) const
{
	return !operator== (rhs);
}

}
