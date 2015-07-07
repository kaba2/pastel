// Description: Testing for integer complexity
// DocumentationOf: integer_complexity.h

#include "test/test_init.h"

#include <pastel/sys/bit/integer_complexity.h>
#include <pastel/sys/sequence/pairwise_all_of.h>

TEST_CASE("integerComplexity (integerComplexity)")
{
	auto f = [&](uint8 x, integer correct)
	{
		return integerComplexity(x) == correct;
	};

	REQUIRE(f(0, 0));
	REQUIRE(f(1, 1));
	REQUIRE(f(2, 2));
	REQUIRE(f(3, 1));
	REQUIRE(f(4, 2));
	REQUIRE(f(5, 3));
	REQUIRE(f(6, 2));
	REQUIRE(f(7, 1));
	REQUIRE(f(8, 2));
	REQUIRE(f(9, 3));
	REQUIRE(f(10, 4));
	REQUIRE(f(11, 3));
	REQUIRE(f(12, 2));
	REQUIRE(f(13, 3));
	REQUIRE(f(14, 2));
	REQUIRE(f(15, 1));
	REQUIRE(f(16, 2));
	REQUIRE(f(17, 3));
	REQUIRE(f(18, 4));
	REQUIRE(f(19, 3));
	REQUIRE(f(20, 4));
	REQUIRE(f(21, 5));
	REQUIRE(f(22, 4));
	REQUIRE(f(23, 3));
	REQUIRE(f(24, 2));
	REQUIRE(f(25, 3));
	REQUIRE(f(26, 4));
	REQUIRE(f(27, 3));
	REQUIRE(f(28, 2));
	REQUIRE(f(29, 3));
	REQUIRE(f(30, 2));
	REQUIRE(f(31, 1));
	REQUIRE(f(255, 1));
	REQUIRE(f(254, 2));
	REQUIRE(f(128, 2));
	REQUIRE(f(127, 1));

	uint8 numberSet[] = 
	{
		0, 1, 3, 2, 7, 6, 4, 5, 15, 14, 12, 13, 8, 9, 11, 10
	};

	REQUIRE(pairwiseAllOf(range(numberSet), 
		[](uint8 left, uint8 right) 
		{
			return lessComplexInteger(left, right) &&
				!lessComplexInteger(right, left);
		}));
}

