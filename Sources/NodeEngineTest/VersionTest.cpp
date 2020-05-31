#include "SimpleTest.hpp"
#include "NUIE_Version.hpp"

using namespace NUIE;

namespace VersionTest
{

TEST (VersionCompatisonTest)
{
	Version v1 (0, 1, 8);
	Version v2 (0, 2, 5);
	Version v3 (3, 2, 5);
	Version v4 (0, 1, 8);

	ASSERT (v1 != v2);
	ASSERT (v1 == v4);
	ASSERT (v1 <= v4);
	ASSERT (v1 >= v4);

	ASSERT (v1 < v2);
	ASSERT (v1 <= v2);
	ASSERT (v2 < v3);
	ASSERT (v1 < v3);
	ASSERT (!(v1 > v2));
	ASSERT (!(v2 > v3));
	ASSERT (!(v1 > v3));

	ASSERT (v2 > v1);
	ASSERT (v2 >= v1);
	ASSERT (v3 > v2);
	ASSERT (v3 > v1);
	ASSERT (!(v2 < v1));
	ASSERT (!(v3 < v2));
	ASSERT (!(v3 < v1));
}

}
