// Description: Testing for binary search
// DocumentationOf: binary_search.h

#include "test/test_init.h"

#include <pastel/sys/sequence/binary_search.h>
#include <pastel/sys/sequence/exponential_binary_search.h>
#include <pastel/sys/indicator/predicate_indicator.h>
#include <pastel/sys/integer/multi_integer.h>

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testBinarySearch<int8>();
			testBinarySearch<int16>();
			testBinarySearch<int32>();
			testBinarySearch<integer>();

			testBinarySearch<Signed_Integer<8>>();
			testBinarySearch<Signed_Integer<9>>();
			testBinarySearch<Signed_Integer<10>>();
			testBinarySearch<Signed_Integer<11>>();

			testBinarySearch<uint8>();
			testBinarySearch<uint16>();
			testBinarySearch<uint32>();
			testBinarySearch<uinteger>();

			testBinarySearch<Unsigned_Integer<8>>();
			testBinarySearch<Unsigned_Integer<9>>();
			testBinarySearch<Unsigned_Integer<10>>();
			testBinarySearch<Unsigned_Integer<11>>();
		}

		template <typename Integer>
		void testBinarySearch()
		{
			{
				Integer min = std::is_signed<Integer>::value ? -128 : 0;
				Integer max = std::is_signed<Integer>::value ? 127 : 255;

				for (Integer b = min;b < max;++b)
				{
					Integer a = binarySearch(
						(Integer)min, (Integer)max, [&](Integer n) {return n >= b;});

					REQUIRE(a == b);
				}
			}
			{
				Integer a = binarySearch(
					(Integer)5, (Integer)infinity<Integer>(), [&](Integer n) {return n >= infinity<Integer>() - 1;});

				REQUIRE(a == (Integer)infinity<Integer>() - 1);
			}
			{
				Integer a = binarySearch(
					(Integer)5, (Integer)infinity<Integer>(), [&](Integer n) {return false;});

				REQUIRE(a == (Integer)infinity<Integer>());
			}
			{
				Integer min = std::is_signed<Integer>::value ? -128 : 0;
				Integer max = std::is_signed<Integer>::value ? 127 : 255;

				for (Integer b = min;b < max;++b)
				{
					Integer a = exponentialBinarySearch(
						(Integer)min, (Integer)max, [&](Integer n) {return n >= b;});

					REQUIRE(a == (Integer)b);
				}
			}
			{
				Integer a = exponentialBinarySearch(
					(Integer)5, (Integer)infinity<Integer>(), [&](Integer n) {return n >= infinity<Integer>() - 1;});

				REQUIRE(a == (Integer)infinity<Integer>() - 1);
			}
			{
				Integer a = exponentialBinarySearch(
					(Integer)5, (Integer)infinity<Integer>(), [&](Integer n) {return false;});

				REQUIRE(a == (Integer)infinity<Integer>());
			}
		}
	};

	TEST_CASE("binarySearch", "[binarySearch]")
	{
	}

}
