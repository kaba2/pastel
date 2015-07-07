// Description: Testing for lowest bit.
// DocumentationOf: lowest_bit.h

#include "test/test_init.h"

#include <pastel/sys/integer/multi_integer.h>
#include <pastel/sys/bit/lowest_bit.h>
#include <pastel/sys/bit/highest_bit.h>
#include <pastel/sys/concept.h>

namespace
{

	template <typename Finite_Integer>
	void testCase()
	{
		REQUIRE(lowestBit(Finite_Integer(0)) == -1);
		REQUIRE(highestBit(Finite_Integer(0)) == -1);

		Finite_Integer a = 1;
		for (integer i = 0;i < bits(a);++i)
		{
			REQUIRE(lowestBit(a) == i);
			REQUIRE(highestBit(a) == i);
			a <<= 1;
		}
	}

}

TEST_CASE("lowest_bit (lowest_bit)")
{
	testCase<uint8>();
	testCase<uint16>();
	testCase<uint32>();
	testCase<uint64>();
	testCase<Signed_Integer<113, uint8>>();
	testCase<Unsigned_Integer<113, uint8>>();
}

