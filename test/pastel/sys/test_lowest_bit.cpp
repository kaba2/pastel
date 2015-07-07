// Description: Testing for lowest bit.
// DocumentationOf: lowest_bit.h

#include "test/test_init.h"

#include <pastel/sys/integer/multi_integer.h>
#include <pastel/sys/bit/lowest_bit.h>
#include <pastel/sys/bit/highest_bit.h>
#include <pastel/sys/concept.h>

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			test<uint8>();
			test<uint16>();
			test<uint32>();
			test<uint64>();
			test<Signed_Integer<113, uint8>>();
			test<Unsigned_Integer<113, uint8>>();
		}

		template <typename Finite_Integer>
		void test()
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
	};

	TEST_CASE("lowest_bit", "[lowest_bit]")
	{
	}

}
