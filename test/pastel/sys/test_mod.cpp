// Description: Testing for mod
// DocumentationOf: mod.h

#include "test/test_init.h"

#include <pastel/sys/math/mod.h>
#include <pastel/sys/math/powers.h>

TEST_CASE("SignedMod (mod)")
{
	for (integer i = 1; i < 10;++i)
	{
		integer k = 0;
		for (integer j = -5 * i;j <= 5 * i;++j)
		{
			REQUIRE(mod(j, i) == k);
			++k;
			if (k == i)
			{
				k = 0;
			}
		}
	}

	for (integer n = 0; n < 5;++n)
	{
		integer i = (1 << n);
		integer k = 0;
		for (integer j = -5 * i;j <= 5 * i;++j)
		{
			REQUIRE(modPowerOfTwo(j, n) == k);
			++k;
			if (k == i)
			{
				k = 0;
			}
		}
	}
}

TEST_CASE("UnsignedMod (mod)")
{
	for (integer n = 0; n < 5;++n)
	{
		uinteger i = powerOfTwo<uinteger>(n);
		uinteger k = 0;
		for (uinteger j = 0;j <= 5 * i;++j)
		{
			REQUIRE(modPowerOfTwo(j, n) == k);
			++k;
			if (k == i)
			{
				k = 0;
			}
		}
	}
}

TEST_CASE("-infinity (mod)")
{
	REQUIRE(mod(-(integer)Infinity() - 1, (integer)4) == 0);
	REQUIRE(mod(-(integer)Infinity() - 1 + 1, (integer)4) == 1);
	REQUIRE(mod(-(integer)Infinity() - 1 + 2, (integer)4) == 2);
	REQUIRE(mod(-(integer)Infinity() - 1 + 3, (integer)4) == 3);
	REQUIRE(mod(-(integer)Infinity() - 1 + 4, (integer)4) == 0);
}
