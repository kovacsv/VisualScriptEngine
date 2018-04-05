#include "SimpleTest.hpp"
#include "NE_Checksum.hpp"

using namespace NE;

namespace ChecksumTest
{

TEST (EmptyChecksumTest)
{
	Checksum c1;
	Checksum c2;
	ASSERT (c1 == c2);
}

TEST (OneIsEmptyChecksum)
{
	Checksum c1;
	Checksum c2;
	c1.Add (1);
	ASSERT (c1 != c2);
}

TEST (SameChecksum)
{
	Checksum c1;
	Checksum c2;
	c1.Add (1);
	c2.Add (1);
	ASSERT (c1 == c2);
	c1.Add (2);
	c2.Add (2);
	ASSERT (c1 == c2);
}

TEST (ChecksumOrderTest)
{
	Checksum c1;
	Checksum c2;
	c1.Add (1); c1.Add (2);
	c2.Add (2); c2.Add (1);
	ASSERT (c1 != c2);
}

TEST (ChecksumCharTest)
{
	Checksum c1;
	Checksum c2;
	c1.Add ('a');
	c2.Add ('b');
	ASSERT (c1 != c2);
}

TEST (ChecksumCharOrderTest)
{
	Checksum c1;
	Checksum c2;
	c1.Add ('a'); c1.Add ('b');
	c2.Add ('b'); c2.Add ('a');
	ASSERT (c1 != c2);
}

TEST (ChecksumDoubleTest)
{
	Checksum c1;
	Checksum c2;
	c1.Add (100.0);
	c2.Add (200.0);
	ASSERT (c1 != c2);
}

TEST (ChecksumDoubleTest2)
{
	Checksum c1;
	Checksum c2;
	c1.Add (1000000000000000.0);
	c2.Add (2000000000000000.0);
	ASSERT (c1 != c2);
}

TEST (ChecksumDoubleTest3)
{
	Checksum c1;
	Checksum c2;
	c1.Add (1.01);
	c2.Add (1.02);
	ASSERT (c1 != c2);
}

TEST (ChecksumStringTest)
{
	Checksum c1;
	Checksum c2;
	c1.Add ("apple");
	c2.Add ("appel");
	ASSERT (c1 != c2);
}

}
